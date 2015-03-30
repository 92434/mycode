


#include "mboot.h"

void cpu_freq_show(void)
{
	u32 sec, old, cnt;

	RTC_CTRL = 0x00002900;
	RTC_TOYTRIM = 0;
	RTC_TRIM = 0;

	old = RTC_TOY0R>>4;
	do{
		sec = RTC_TOY0R>>4;
	}while(sec==old);

	reset_timer();

	old = sec;
	do{
		sec = RTC_TOY0R>>4;
	}while(sec==old);
	cnt = get_timer();

	printk("cp0 count: %08x\n", cnt);

	cnt /= 10000;
	cnt *= 20000;
	printk("cpu  freq: %d\n", cnt);

}

void set_cpu_freq(int div)
{
	u32 config, tmp;
	int i;

	config = 0x30ca0040;
	config |= (div&0x07)<<8;
	printk("\nnew config: %08x\n", config);

	// cpu pll bypass
	CLK_CTRL0 = 0x00000000;

	// cpu pll powerdown
	CLK_CTRL0 = config|0x0800;
	for(i=0; i<1000; i++){
		tmp = CHIP_SPL0;
	}

	// cpu pll powerup
	CLK_CTRL0 = config;
	for(i=0; i<1000; i++){
		tmp = CHIP_SPL0;
	}

	// cpu pll set
	CLK_CTRL0 = config|0x1001;
	CLK_CTRL0 = config|0x1000;
	for(i=0; i<1000; i++){
		tmp = CHIP_SPL0;
	}
}

void l2m_config(int master, int id, u64 base, u64 mask, u64 mmap)
{
	u32 ma;

	ma = 0xbfd80000+master*0x0100;
	if(master==6)
		ma += 0x00002000;

	*(u64*)(ma+0x00+id*8) = base;
	*(u64*)(ma+0x40+id*8) = mask;
	*(u64*)(ma+0x80+id*8) = mmap;
}

void cpu_l2_config(void)
{
	// CPU0
	l2m_config(0, 2, 0x000000001fe40000, 0xfffffffffffe0000, 0x0000000000000082);
	l2m_config(0, 3, 0x0000000010000000, 0xfffffffff8000000, 0x0000000010000084);
	l2m_config(0, 4, 0x0000000018000000, 0xfffffffff8000000, 0x0000000018000081);
	l2m_config(0, 5, 0x0000000040000000, 0xffffffffc0000000, 0x0000000040000084);

	// SBridge         |      .|      .    |      .|      .    |      .|      .
	l2m_config(1, 0, 0x0000000000000000, 0xfffffffff0000000, 0x00000000400000f3);
	l2m_config(1, 1, 0x0000000000000000, 0xffffffffc0000000, 0x00000001400000f3);
	l2m_config(1, 2, 0x0000000040000000, 0xffffffffc0000000, 0x00000002400000f3);
	l2m_config(1, 3, 0x0000000080000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(1, 4, 0x00000000c0000000, 0xffffffffc0000000, 0x00000004400000f3);
	l2m_config(1, 5, 0xffffffff80000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(1, 6, 0xffffffffc0000000, 0xffffffffc0000000, 0x00000004400000f3);

	// GPU
	l2m_config(2, 0, 0x0000000000000000, 0xfffffffff0000000, 0x00000000400000f3);
	l2m_config(2, 1, 0x0000000000000000, 0xffffffffc0000000, 0x00000001400000f3);
	l2m_config(2, 2, 0x0000000040000000, 0xffffffffc0000000, 0x00000002400000f3);
	l2m_config(2, 3, 0x0000000080000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(2, 4, 0x00000000c0000000, 0xffffffffc0000000, 0x00000004400000f3);
	l2m_config(2, 5, 0xffffffff80000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(2, 6, 0xffffffffc0000000, 0xffffffffc0000000, 0x00000004400000f3);

	// PCIE
	l2m_config(4, 0, 0x0000000000000000, 0xfffffffff0000000, 0x00000000400000f3);
	l2m_config(4, 1, 0x0000000000000000, 0xffffffffc0000000, 0x00000001400000f3);
	l2m_config(4, 2, 0x0000000040000000, 0xffffffffc0000000, 0x00000002400000f3);
	l2m_config(4, 3, 0x0000000080000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(4, 4, 0x00000000c0000000, 0xffffffffc0000000, 0x00000004400000f3);
	l2m_config(4, 5, 0xffffffff80000000, 0xffffffffc0000000, 0x00000003400000f3);
	l2m_config(4, 6, 0xffffffffc0000000, 0xffffffffc0000000, 0x00000004400000f3);

	// m6              |      .|      .    |      .|      .    |      .|      .
	l2m_config(6, 0, 0x0000000040000000, 0xffffffffc0000000, 0x00000000000000f0);
	l2m_config(6, 1, 0x0000000140000000, 0xffffffffc0000000, 0x00000001000000f0);
	l2m_config(6, 2, 0x0000000240000000, 0xffffffffc0000000, 0x00000001400000f0);
	l2m_config(6, 3, 0x0000000340000000, 0xffffffffc0000000, 0x00000001800000f0);
	l2m_config(6, 4, 0x0000000440000000, 0xffffffffc0000000, 0x00000001c00000f0);
	l2m_config(6, 5, 0x0000000000000000, 0x0000000000000000, 0x00000000000000f0);

}


void l2m_show(int master)
{
	int i;
	u32 ma;
	u64 base, mask, mmap;

	ma = 0xbfd80000+master*0x0100;
	if(master==6)
		ma += 0x00002000;

	printk("master %d:\n", master);
	for(i=0; i<8; i++){
		base = *(u64*)(ma+0x00+i*8);
		mask = *(u64*)(ma+0x40+i*8);
		mmap = *(u64*)(ma+0x80+i*8);
		printk("  base=%08x_%08x mask=%08x_%08x mmap=%08x_%08x\n",
				(u32)(base>>32), (u32)(base),
				(u32)(mask>>32), (u32)(mask),
				(u32)(mmap>>32), (u32)(mmap)
		);
	}
}

void cpu_l2_show(void)
{
	l2m_show(0);
	l2m_show(1);
	l2m_show(2);
	l2m_show(3);
	l2m_show(4);
	l2m_show(5);
	l2m_show(6);
}


static void f81216_write(int addr, int data)
{   
	REG8(0xbff0004e) = addr;
	REG8(0xbff0004f) = data;
}

static void f81216_init()
{   
	REG8(0xbff0004e) = 0x77;
	REG8(0xbff0004e) = 0x77;

	f81216_write(0x25, 0x01); // select 48M
	f81216_write(0x07, 0x00); // select UART1
	f81216_write(0x60, 0x03); // UART1 at 03f8
	f81216_write(0x61, 0xf8); //
	f81216_write(0x70, 0x04); // use IRQ4
	f81216_write(0x30, 0x01); // enable

	f81216_write(0x07, 0x01); // select UART2
	f81216_write(0x60, 0x02); // UART2 at 02f8
	f81216_write(0x61, 0xf8); //
	f81216_write(0x70, 0x03); // use IRQ3
	f81216_write(0x30, 0x01); // enable

	f81216_write(0x07, 0x02); // select UART3
	f81216_write(0x60, 0x03); // UART3 at 03e8
	f81216_write(0x61, 0xe8); //
	f81216_write(0x70, 0x09); // use IRQ9
	f81216_write(0x30, 0x01); // enable

	f81216_write(0x07, 0x03); // select UART4
	f81216_write(0x60, 0x02); // UART4 at 02e8
	f81216_write(0x61, 0xe8); //
	f81216_write(0x70, 0x05); // use IRQ5
	f81216_write(0x30, 0x01); // enable

	REG8(0xbff0004e) = 0xaa;
}



void cpu_init(void)
{
	u32 status;

	cpu_l2_config();
	tlb_init();
	fpu_init();

	// RTC
	RTC_CTRL = 0x00002900;
	RTC_TOYTRIM = 0;
	RTC_TRIM = 0;

	// GPIO
	GPIOCFG = 0x00010000;
	GPIODIR = 0x000008fd;
	GPIOOUT = 0x00007202;

	// LPC UART
	f81216_init();

}


