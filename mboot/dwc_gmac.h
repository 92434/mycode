/*
 * (C) Copyright 2010
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _DW_ETH_H
#define _DW_ETH_H

#define CONFIG_TX_DESCR_NUM	16
#define CONFIG_RX_DESCR_NUM	16
#define CONFIG_ETH_BUFSIZE	2048
#define TX_TOTAL_BUFSIZE	(CONFIG_ETH_BUFSIZE * CONFIG_TX_DESCR_NUM)
#define RX_TOTAL_BUFSIZE	(CONFIG_ETH_BUFSIZE * CONFIG_RX_DESCR_NUM)

#define CONFIG_MACRESET_TIMEOUT	(3 * CONFIG_SYS_HZ)
#define CONFIG_MDIO_TIMEOUT	(3 * CONFIG_SYS_HZ)

struct eth_mac_regs {
	u32 conf;			/* 0x00 */
	u32 framefilt;		/* 0x04 */
	u32 hashtablehigh;	/* 0x08 */
	u32 hashtablelow;	/* 0x0c */
	u32 miiaddr;		/* 0x10 */
	u32 miidata;		/* 0x14 */
	u32 flowcontrol;	/* 0x18 */
	u32 vlantag;		/* 0x1c */
	u32 version;		/* 0x20 */
	u8  reserved_1[20];
	u32 intreg;			/* 0x38 */
	u32 intmask;		/* 0x3c */
	u32 macaddr0hi;		/* 0x40 */
	u32 macaddr0lo;		/* 0x44 */
};

#define GMAC ((volatile struct eth_mac_regs*)0xbfe10000)

/* MAC configuration register definitions */
#define FRAMEBURSTENABLE	(1 << 21)
#define MII_PORTSELECT		(1 << 15)
#define FES_100			(1 << 14)
#define DISABLERXOWN		(1 << 13)
#define FULLDPLXMODE		(1 << 11)
#define RXENABLE		(1 << 2)
#define TXENABLE		(1 << 3)

/* MII address register definitions */
#define MII_BUSY		(1 << 0)
#define MII_WRITE		(1 << 1)
#define MII_CLKRANGE_60_100M	(0)
#define MII_CLKRANGE_100_150M	(0x4)
#define MII_CLKRANGE_20_35M	(0x8)
#define MII_CLKRANGE_35_60M	(0xC)
#define MII_CLKRANGE_150_250M	(0x10)
#define MII_CLKRANGE_250_300M	(0x14)

#define MIIADDRSHIFT		(11)
#define MIIREGSHIFT		(6)
#define MII_REGMSK		(0x1F << 6)
#define MII_ADDRMSK		(0x1F << 11)


struct eth_dma_regs {
	u32 busmode;			/* 0x00 */
	u32 txpolldemand;		/* 0x04 */
	u32 rxpolldemand;		/* 0x08 */
	u32 rxdesclistaddr;		/* 0x0c */
	u32 txdesclistaddr;		/* 0x10 */
	u32 status;				/* 0x14 */
	u32 opmode;				/* 0x18 */
	u32 intenable;			/* 0x1c */
	u8  reserved[40];
	u32 currhosttxdesc;		/* 0x48 */
	u32 currhostrxdesc;		/* 0x4c */
	u32 currhosttxbuffaddr;	/* 0x50 */
	u32 currhostrxbuffaddr;	/* 0x54 */
};

#define GDMA ((volatile struct eth_dma_regs*)0xbfe11000)

#define DW_DMA_BASE_OFFSET	(0x1000)

/* Bus mode register definitions */
#define FIXEDBURST		(1 << 16)
#define PRIORXTX_41		(3 << 14)
#define PRIORXTX_31		(2 << 14)
#define PRIORXTX_21		(1 << 14)
#define PRIORXTX_11		(0 << 14)
#define BURST_1			(1 << 8)
#define BURST_2			(2 << 8)
#define BURST_4			(4 << 8)
#define BURST_8			(8 << 8)
#define BURST_16		(16 << 8)
#define BURST_32		(32 << 8)
#define RXHIGHPRIO		(1 << 1)
#define DMAMAC_SRST		(1 << 0)

/* Poll demand definitions */
#define POLL_DATA		(0xFFFFFFFF)

/* Operation mode definitions */
#define STOREFORWARD		(1 << 21)
#define FLUSHTXFIFO		(1 << 20)
#define TXSTART			(1 << 13)
#define TXSECONDFRAME		(1 << 2)
#define RXSTART			(1 << 1)

/* Descriptior related definitions */
#define MAC_MAX_FRAME_SZ	(1600)

typedef struct {
	u32 status;
	u32 length;
	u32 addr;
	u32 next;
	u32 desc4;
	u32 desc5;
	u32 desc6;
	u32 desc7;
}GMAC_DMA_DESC;

/*
 * txrx_status definitions
 */

/* tx status bits definitions */
#define TX_OWN			(1 << 31)
#define TX_INT			(1 << 30)
#define TX_LAST			(1 << 29)
#define TX_FIRST		(1 << 28)
#define TX_CRCDIS		(1 << 27)
#define TX_PADDIS		(1 << 26)
#define TX_AUTOCHK		(3 << 22)
#define TX_END			(1 << 21)
#define TX_CHAIN		(1 << 20)
#define TX_MSK			(0x1FFFF << 0)
#define TX_ERROR		(1 << 15)
/* tx control bits definitions */
#define TX_SIZEMASK		(0x1FFF << 0)
#define TX_SIZESHIFT	(0)


/* rx status bits definitions */
#define RX_OWN			(1 << 31)
#define RX_DSTAF		(1 << 30)
#define RX_LENMASK		(0x3FFF << 16)
#define RX_LENSHIFT		(16)
#define RX_ERROR		(1 << 15)
#define RX_DESCERR		(1 << 14)
#define RX_SRCAF		(1 << 13)
#define RX_LENERR		(1 << 12)
#define RX_OVERFLOW		(1 << 11)
#define RX_VLANTAG		(1 << 10)
#define RX_FIRST		(1 << 9)
#define RX_LAST			(1 << 8)
#define RX_IPC_GIANT	(1 << 7)
#define RX_COL			(1 << 6)
#define RX_ETHER		(1 << 5)
#define RX_WDT			(1 << 4)
#define RX_MIIERROR		(1 << 3)
#define RX_DRIBBLING	(1 << 2)
#define RX_CRC			(1 << 1)
#define RX_DA			(1 << 0)
/* rx control bits definitions */
#define RX_INTDIS		(1 << 31)
#define RX_END			(1 << 15)
#define RX_CHAIN		(1 << 14)
#define RX_SIZEMASK		(0x1FFF << 0)
#define RX_SIZESHIFT	(0)

#endif
