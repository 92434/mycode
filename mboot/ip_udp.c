/*
 * Simple TFTP program.
 *  tpu.
 */

#include "mboot.h"
#include "net.h"

u8 packet_buf[2048];
u8 tx_buf[2048];
u8 local_hwaddr[8] = {0x34, 0x33, 0x32, 0x31, 0x30, 0x00};
u8 server_hwaddr[8];
int local_port;
int server_port;


#define MAKE_IP(a, b, c, d)  (a | (b<<8) | (c<<16) | (d<<24))

/* local ip: 192.168.1.193 */
int local_ipaddr = MAKE_IP(192,168,1,193);

/* server ip: 192.168.1.3 */
int server_ipaddr = MAKE_IP(192,168,1,3);

int ip_id;

int check_sum(u8 *buf, int len)
{
    u16 *ptr = (u16 *)buf;
    u32 i, sum;

    if(len&1){
        buf[len] = 0;
        len++;
    }
    len>>=1;

    sum = 0;
    for(i=0; i<len; i++){
        sum += SWAP16(ptr[i]);
    }

    sum = (sum&0xffff)+(sum>>16);
    sum = (sum&0xffff)+(sum>>16);

    return sum&0xffff;
}

int packet_check(IP_t *iptr)
{
    int len, sum;
    int lip, sip;
    
    /* check ip checksum */
    len = (iptr->ip_hl_v&0x0f)<<2;
    sum = check_sum((u8 *)iptr, len);
    if(sum!=0xffff){
        printk(" ip checksum failed: sum=%04x\n", sum);
        //mem_dump(NULL, iptr, len);
        return -1;
    }

    /* check ip address */
    memcpy(&lip, &iptr->ip_dst, 4);
    memcpy(&sip, &iptr->ip_src, 4);
    if(lip!=local_ipaddr || sip!=server_ipaddr){
        return -1;
    }
    
    /* check udp checksum */
    if(iptr->ip_prot!=0x11)
        return -1;
    iptr->ip_ttl = 0;
    len = SWAP16(iptr->udp_len);
    iptr->ip_sum = iptr->udp_len;
    sum = check_sum(&iptr->ip_ttl, len+12);
    if(sum!=0xffff){
        printk(" udp checksum failed: sum=%04x\n", sum);
        mem_dump(NULL, &iptr->udp_src, len);
        return -1;
    }

    /* check udp port */
    if(iptr->udp_dst!=SWAP16(local_port)){
        printk(" udp target port failed: port=%04x\n", SWAP16(iptr->udp_dst));
        return -1;
    }
    
    return 0;
}

/* UDP packet receive */
int udp_recv(int timeout_ms)
{
	int len, prot;
	int now, end;
	Ethernet_t *ethdr = (Ethernet_t *)packet_buf;
	IP_t *ip_hdr = (IP_t *)(packet_buf+14);

	end = get_clock_tick_rate()/1000;
	end *= timeout_ms;
	reset_timer();

	while(1){
		now = get_timer();
		if(now>=end)
			return 0;
		len = eth_rx(packet_buf);
		if(!len)
			continue;

		prot = SWAP16(ethdr->et_protlen);
		if(prot==ARP_PKT){
			arp_ack();
			continue;
		}else if(prot!=IP_PKT){
			continue;
		}

		if(ip_hdr->ip_prot!=0x11)
			continue;
		if(packet_check(ip_hdr)){
			continue;
		}else{
			server_port = SWAP16(ip_hdr->udp_src);
			len = SWAP16(ip_hdr->udp_len)-8;
			break;
		}
	}

	return len;
}

int udp_send(char *buf, int len)
{
    Ethernet_t *tx_hdr = (Ethernet_t *)tx_buf;
    IP_t *iptr = (IP_t *)(tx_buf+14);
    int sum;
    
    /* build udp packet */
    memcpy(iptr->udp_data, buf, len);
    len += 8;

    iptr->udp_len = SWAP16(len);
    iptr->udp_xsum = 0;
    iptr->udp_src = SWAP16(local_port);
    iptr->udp_dst = SWAP16(server_port);

    iptr->ip_ttl = 0;
    iptr->ip_prot = 0x11;
    iptr->ip_sum = SWAP16(len);
    memcpy(&iptr->ip_src, &local_ipaddr, 4);
    memcpy(&iptr->ip_dst, &server_ipaddr, 4);
    sum = check_sum(&iptr->ip_ttl, len+12);
    iptr->udp_xsum = SWAP16((~sum)&0xffff);

    /* build ip packet */
    iptr->ip_hl_v = 0x45;
    iptr->ip_tos = 0;
    len += 20;
    iptr->ip_len = SWAP16(len);
    iptr->ip_id = ip_id;
    ip_id++;
    iptr->ip_off = SWAP16(0x4000);
    iptr->ip_ttl = 0x80;
    iptr->ip_prot = 0x11;
    iptr->ip_sum = 0;
    sum = check_sum((u8*)iptr, 20);
    iptr->ip_sum = SWAP16((~sum)&0xffff);

    /* Set Ethernet header */
    memcpy(tx_hdr->et_src, local_hwaddr, 6);
    memcpy(tx_hdr->et_dest, server_hwaddr, 6);
    tx_hdr->et_protlen = SWAP16(IP_PKT);

    len += 14;
    //printk("  UDP send:\n");
    //mem_dump(NULL, tx_buf, len);
    eth_send(tx_buf, len);

	return 0;
}


int arp_op;

int arp_req(void)
{
	int len, prot, i;
	int start_time, now;

    Ethernet_t *ethdr = (Ethernet_t *)packet_buf;
    Ethernet_t *tx_hdr = (Ethernet_t *)tx_buf;
    ARP_t *tx_arp = (ARP_t *)(tx_buf+14);
   
    /* Set Ethernet Header */
    memcpy(tx_hdr->et_src, local_hwaddr, 6);
    memset(tx_hdr->et_dest, 0xff, 6);
    tx_hdr->et_protlen = SWAP16(ARP_PKT);

    /* Set ARP Header */
    tx_arp->ar_hrd = SWAP16(0x0001);
    tx_arp->ar_pro = SWAP16(0x0800);
    tx_arp->ar_hln = 6;
    tx_arp->ar_pln = 4;
    tx_arp->ar_op  = SWAP16(0x0001);
    memcpy(tx_arp->ar_sha, local_hwaddr, 6);
    memcpy(tx_arp->ar_tha, server_hwaddr, 6);
    memcpy(tx_arp->ar_spa, (char*)&local_ipaddr, 4);
    memcpy(tx_arp->ar_tpa, (char*)&server_ipaddr, 4);


	/* ARP loop */
	arp_op = 0x0002;
	start_time = 0;
	now = get_clock_tick_rate()+1;

	while(arp_op){
		if((now-start_time)>get_clock_tick_rate()){
			printk(" Send ARP request ...\n");
			eth_send(tx_buf, 60);
			reset_timer();
			start_time = get_timer();
		}

		len = eth_rx(packet_buf);
		if(len){
			prot = SWAP16(ethdr->et_protlen);
			if(prot==ARP_PKT){
				arp_ack();
			}
		}

		now = get_timer();
	}

	printk(" ARP_ACK: Server address is ");
	for(i=0; i<6; i++){
		printk("%02x", server_hwaddr[i]);
		if(i!=5)
			printk(":");
	}
	printk("\n");

    return 0;
}

void arp_ack(void)
{
    Ethernet_t *tx_hdr = (Ethernet_t *)tx_buf;
    ARP_t *rx_arp = (ARP_t *)(packet_buf+14);
    ARP_t *tx_arp = (ARP_t *)(tx_buf+14);
    int lip;
    
    memcpy(&lip, rx_arp->ar_tpa, 4);
    if(lip != local_ipaddr)
        return;

    memcpy(server_hwaddr, rx_arp->ar_sha, 6);
    if(rx_arp->ar_op==SWAP16(arp_op)){
        arp_op = 0;
        return;
    }
    
    memcpy(tx_hdr->et_src, local_hwaddr, 6);
    memcpy(tx_hdr->et_dest, server_hwaddr, 6);
    tx_hdr->et_protlen = SWAP16(0x0806);

    tx_arp->ar_hrd = SWAP16(0x0001);
    tx_arp->ar_pro = SWAP16(0x0800);
    tx_arp->ar_hln = 6;
    tx_arp->ar_pln = 4;
    tx_arp->ar_op  = SWAP16(0x0002);

    memcpy(tx_arp->ar_sha, local_hwaddr, 6);
    memcpy(tx_arp->ar_tha, server_hwaddr, 6);
    memcpy(tx_arp->ar_spa, (char*)&local_ipaddr, 4);
    memcpy(tx_arp->ar_tpa, (char*)&server_ipaddr, 4);

    eth_send(tx_buf, 60);
}

