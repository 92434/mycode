

#include "mboot.h"

/**************************************************************/

void spi_init(void)
{
	SPI_SPSR   = 0xc0;
	SPI_PARAM  = 0x40;
	SPI_SPER   = 0x05;
	SPI_TIMING = 0x01;
	SPI_SPCR   = 0x50;
}

void spi_cs(int val)
{
	val = (val!=0)? 0x11 : 0x01;
	SPI_SOFTCS = val;
}

int spi_byte(int data)
{
	SPI_FIFO = data;
	while(SPI_SPSR&0x01);

	return SPI_FIFO;
}

/**************************************************************/

int sf_status(void)
{
	int status;

	spi_cs(0);
	spi_byte(0x05);
	status = spi_byte(0);
	spi_cs(1);

	return status;
}

int sf_write_sr(int data)
{
	spi_cs(0);
	spi_byte(0x01);
	spi_byte(data);
	spi_cs(1);

	return 0;
}

void sf_wen(int en)
{
	spi_cs(0);
	if(en)
		spi_byte(0x06);
	else
		spi_byte(0x04);
	spi_cs(1);
}

int sf_wait(void)
{
	int status;

	while(1){
		status = sf_status();
		if((status&1)==0)
			break;
	}

	return status;
}

int sf_readid(void)
{
	int mid, pid;

	spi_init();

	sf_wen(1);
	sf_write_sr(0);

	spi_cs(0);

	spi_byte(0x90);
	spi_byte(0);
	spi_byte(0);
	spi_byte(0);

	mid = spi_byte(0);
	pid = spi_byte(0);

	spi_cs(1);

	return (mid<<8)|pid;
}

int sf_sector_erase(int addr)
{
	int stat;

	sf_wen(1);
	spi_cs(0);

	spi_byte(0x20);
	spi_byte((addr>>16)&0xff);
	spi_byte((addr>> 8)&0xff);
	spi_byte((addr>> 0)&0xff);

	spi_cs(1);
	stat = sf_wait();

	return stat;
}

int sf_byte_write(int addr, int data)
{
	sf_wen(1);

	spi_cs(0);
	spi_byte(0x02);
	spi_byte((addr>>16)&0xff);
	spi_byte((addr>> 8)&0xff);
	spi_byte((addr>> 0)&0xff);
	spi_byte(data);
	spi_cs(1);

	sf_wait();
	return 0;
}

int sf_page_write(int addr, u8 *buf)
{
	int i;

	for(i=0; i<256; i++){
		sf_byte_write(addr+i, buf[i]);
	}

	return 0;
}


int sf_write(int addr, int len, u8 *buf)
{
	int sector, page;

	printk("spi flash write: addr=%08x len=%08x\n", addr, len);

	for(sector=0; sector<len; sector+=4096){
		sf_sector_erase(sector+addr);
		for(page=0; page<4096; page+=256){
			sf_page_write(addr+sector+page, buf+sector+page);
			printk("  %08x  \r", sector+page);
		}
	}

	printk("  %08x  \n", len);
	return 0;
}

int sf_read(int addr, int len, u8 *buf)
{
	int i;

	spi_cs(0);

	spi_byte(0x0b);
	spi_byte((addr>>16)&0xff);
	spi_byte((addr>> 8)&0xff);
	spi_byte((addr>> 0)&0xff);
	spi_byte(0x00);

	for(i=0; i<len; i++){
		buf[i] = spi_byte(0);
	}

	spi_cs(1);
	sf_wait();

	return 0;
}


