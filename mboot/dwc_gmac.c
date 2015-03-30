/*
 * (C) Copyright 2010
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Designware ethernet IP driver for u-boot
 */

#include "mboot.h"
#include "dwc_gmac.h"
#include "net.h"

#define TX_DESC_NUM 4
#define RX_DESC_NUM 4
#define ETH_BUFSIZE 2048

GMAC_DMA_DESC *tx_desc = (void*)0xaff00200;
GMAC_DMA_DESC *rx_desc = (void*)0xaff00300;
u32 tx_buffer = 0xaff00400;
u32 rx_buffer = 0xaff02400;
int tx_cur;
int rx_cur;
int phy_addr = -1;

u8 *temp_buf = (void*)0xaff04400;

int mii_read(int phyaddr, int reg)
{
	GMAC->miiaddr = 0;
	GMAC->miiaddr = (phyaddr<<11) | (reg<<6) | (3<<2) | (0<<1) | 1;
	while((GMAC->miiaddr)&1);
	return GMAC->miidata;
}

int mii_write(int phyaddr, int reg, int val)
{
	GMAC->miiaddr = 0;
	GMAC->miidata = val;
	GMAC->miiaddr = (phyaddr<<11) | (reg<<6) | (3<<2) | (1<<1) | 1;
	while((GMAC->miiaddr)&1);
	return 0;
}

int phy_check(int *speed, int *duplex)
{
	int link, advertise, lpa, lpa2, media;

	*speed = 10;
	*duplex = 0;

	/* check link first */
	link = mii_read(phy_addr, 1);
	if((link&0x0004)==0){
		printk("No Link.\n");
		return -1;
	}

	advertise = mii_read(phy_addr, 4);
	lpa       = mii_read(phy_addr, 5);
	lpa2      = mii_read(phy_addr, 10);
	media     = advertise&lpa;
	if(lpa2==0xffff)
		lpa2 = 0;

	*duplex = (media&0x0140)? 1: 0;
	if(lpa2&0x0800)
		*duplex = 1;

	if(lpa2&0x0c00)
		*speed = 1000;
	else if(media&0x0180)
		*speed = 100;
	else
		*speed = 10;

	return 0; 
}       

int wait_link(int *speed, int *duplex)
{   
	int timeout;

	timeout = 0;
	while(phy_check(speed, duplex)){
		udelay(1000);
		timeout++;
		if((timeout%1024)==0){
			printk("No Link Detect! Check Cable?\n");
		}
		if(timeout>20000){
			return -1;
		}
	}

	printk("Link speed: %3dM %s duplex\n", *speed, *duplex? "FULL" : "HALF" );
	udelay(10000);

	return 0;
}

static void descs_init(void)
{
	GMAC_DMA_DESC *desc;
	int i;

	for(i=0; i<TX_DESC_NUM; i++){
		desc = &tx_desc[i];
		desc->addr = VA_TO_PA(tx_buffer+(i*ETH_BUFSIZE));
		desc->next = VA_TO_PA(&tx_desc[i+1]);
		desc->status = TX_CHAIN;
		desc->length = 0;
	}
	desc->next = VA_TO_PA(&tx_desc[0]);

	for(i=0; i<RX_DESC_NUM; i++){
		desc = &rx_desc[i];
		desc->addr = VA_TO_PA(rx_buffer+i*ETH_BUFSIZE);
		desc->next = VA_TO_PA(&rx_desc[i+1]);
		desc->length = RX_CHAIN | 1600;
		desc->status = RX_OWN;
	}
	desc->next = VA_TO_PA(&rx_desc[0]);

	GDMA->txdesclistaddr = VA_TO_PA(&tx_desc[0]);
	GDMA->rxdesclistaddr = VA_TO_PA(&rx_desc[0]);
	tx_cur = 0;
	rx_cur = 0;

}

void eth_set_mac(u8 *mac_addr)
{
	GMAC->macaddr0hi = mac_addr[4] | (mac_addr[5]<<8);
	GMAC->macaddr0lo = mac_addr[0] | (mac_addr[1]<<8) | (mac_addr[2]<<16) | (mac_addr[3]<<24);
}

int eth_init(u8 *mac_addr)
{
	int speed, duplex;

	phy_addr = 0;

	GDMA->busmode |= DMAMAC_SRST;
	while((GDMA->busmode)&DMAMAC_SRST);

	eth_set_mac(mac_addr);

	descs_init();

	GDMA->busmode = (FIXEDBURST | PRIORXTX_41 | BURST_16);
	GDMA->opmode |= (FLUSHTXFIFO | STOREFORWARD);
	GDMA->opmode |= (RXSTART | TXSTART);

	wait_link(&speed, &duplex);
	if(speed<1000){
		GMAC->conf |= MII_PORTSELECT;
		if(speed==100)
			GMAC->conf |= FES_100;
	}
	if(duplex){
		GMAC->conf |= FULLDPLXMODE;
	}

	GMAC->conf |= (RXENABLE | TXENABLE);

	printk("eth_init done.\n");

	return 0;
}

int eth_send(void *packet, int length)
{
	GMAC_DMA_DESC *desc = &tx_desc[tx_cur];

	/* wait trans complete */
	while(desc->status&TX_OWN);

	memcpy((void*)PA_TO_UVA(desc->addr), packet, length);
	desc->length = length;

	desc->status &= ~(TX_MSK);
	desc->status |= TX_OWN | TX_FIRST | TX_LAST;

	tx_cur += 1;
	if(tx_cur>=TX_DESC_NUM)
		tx_cur = 0;

	/* Start the transmission */
	GDMA->txpolldemand = POLL_DATA;
	return 0;
}

int eth_rx(void *packet)
{
	u32 status;
	int length;
	GMAC_DMA_DESC *desc = &rx_desc[rx_cur];

	status = desc->status;
	if(status&RX_OWN){
		return 0;
	}

	length = (status&RX_LENMASK)>>RX_LENSHIFT;
	memcpy(packet, (void*)PA_TO_UVA(desc->addr), length);

	desc->status |= RX_OWN;

	rx_cur += 1;
	if(rx_cur>=RX_DESC_NUM)
		rx_cur = 0;

	//mem_dump("eth_rx", packet, length);

	return length;
}


void mem_dump(char *str, void *buf, int len)
{
	int i;

	if(str)
		printk("%s :\n", str);

	for(i=0; i<len; i++){
		if((i%16)==0){
			printk("\n%04x: ", i);
		}
		printk(" %02x", *(u8*)(buf+i));
	}
	printk("\n\n");
}

void eth_test(void)
{
	int i, len;
	u32 data;

	if(phy_addr==-1){
		phy_addr = 0;

		local_hwaddr[0] = 0x00;
		local_hwaddr[1] = 0x30;
		local_hwaddr[2] = 0x33;
		local_hwaddr[3] = 0x44;
		local_hwaddr[4] = 0x55;
		local_hwaddr[5] = 0x67;

		eth_init(local_hwaddr);
	}

	printk("phy_%02x: ", phy_addr);
	for(i=0; i<32; i++){
		data = mii_read(phy_addr, i);
		printk(" %04x", data);
	}
	printk("\n\n");

	arp_req();
}

