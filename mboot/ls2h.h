

#define OSC_IN 100000000
#define SYS_CLK 125000000
#define UART_BAUD 115200

#define CFG_FLASH_BASE  0xFC000000
#define CFG_DRAM_BASE   0x00000000
#define CFG_L2SRAM_BASE 0x40000000


/* UART0 */
#define UART0_BASE  0xbfe80000

#define URBR		REG(0xbfe80000)
#define UTHR		REG(0xbfe80000)
#define UDLB		REG(0xbfe80000)
#define UIER		REG(0xbfe80001)
#define UDMB		REG(0xbfe80001)
#define UIIR		REG(0xbfe80002)
#define UFCR		REG(0xbfe80002)
#define UAFR		REG(0xbfe80002)
#define ULCR		REG(0xbfe80003)
#define UMCR		REG(0xbfe80004)
#define ULSR		REG(0xbfe80005)
#define UMSR		REG(0xbfe80006)
#define USCR		REG(0xbfe80007)

/* GPIO */
#define GPIOCFG		REG(0xbfd000c0)
#define GPIODIR		REG(0xbfd000c4)
#define GPIOIN		REG(0xbfd000c8)
#define GPIOOUT		REG(0xbfd000cc)

/* SYSTEM CONFIG */
#define CHIP_CFG0	REG(0xbfd00200)
#define CHIP_CFG1	REG(0xbfd00204)
#define CHIP_CFG2	REG(0xbfd00208)
#define CHIP_CFG3	REG(0xbfd0020c)
#define CHIP_SPL0	REG(0xbfd00210)
#define CHIP_SPL1	REG(0xbfd00214)
#define CHIP_SPL2	REG(0xbfd00218)
#define CHIP_SPL3	REG(0xbfd0021c)
#define CLK_CTRL0	REG(0xbfd00220)
#define CLK_CTRL1	REG(0xbfd00224)
#define CLK_CTRL2	REG(0xbfd00228)
#define CLK_CTRL3	REG(0xbfd0022c)
#define PCLK0_CTRL0	REG(0xbfd00230)
#define PCLK0_CTRL1	REG(0xbfd00234)
#define PCLK1_CTRL0	REG(0xbfd00238)
#define PCLK1_CTRL1	REG(0xbfd0023c)
#define DMA_CONFIG  REG(0xbfd00100)

/* SPI */
#define SPI_SPCR	REG8(0xbfe70000)
#define SPI_SPSR	REG8(0xbfe70001)
#define SPI_FIFO	REG8(0xbfe70002)
#define SPI_SPER	REG8(0xbfe70003)
#define SPI_PARAM	REG8(0xbfe70004)
#define SPI_SOFTCS	REG8(0xbfe70005)
#define SPI_TIMING	REG8(0xbfe70006)


/* RTC */
#define RTC_TOYTRIM	REG(0xbfef8020)
#define RTC_TOY0W	REG(0xbfef8024)
#define RTC_TOY1W	REG(0xbfef8028)
#define RTC_TOY0R	REG(0xbfef802c)
#define RTC_TOY1R	REG(0xbfef8030)
#define RTC_CTRL	REG(0xbfef8040)
#define RTC_TRIM	REG(0xbfef8060)


/* NAND */
#define NAND_CMD    REG(0xbfee0000)
#define NAND_ADDR_C REG(0xbfee0004)
#define NAND_ADDR_R REG(0xbfee0008)
#define NAND_TIMING REG(0xbfee000c)
#define NAND_ID_L   REG(0xbfee0010)
#define NAND_STATUS REG(0xbfee0014)
#define NAND_PARAM  REG(0xbfee0018)
#define NAND_OPNUM  REG(0xbfee001c)
#define NAND_MAP    REG(0xbfee0020)
#define NAND_DMA    REG(0xbfee0040)





#ifndef __ASSEMBLY__

typedef struct {
	u32 next;
	u32 src_addr;
	u32 dst_addr;
	u32 length;
	u32 step_length;
	u32 step_times;
	u32 cmd;
	u32 unuse;
}DMA_DESC;

#endif
