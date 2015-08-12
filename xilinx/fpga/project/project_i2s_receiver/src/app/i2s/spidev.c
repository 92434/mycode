/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev1.0";
static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static uint16_t delay = 0;

struct st_cmd
{
	uint8_t cmd;
	const char *cmd_str;
};

#define READ_REG(x) (x | 0x80)
#define WRITE_REG(x) (x)
typedef enum _read_write {
	spi_read = 0,
	spi_write,
} read_write_t;

static const struct st_cmd cmds[] =
{
	{0, "exit"},
	{1, "reset"},
	{2, "read device id 0"},
	{3, "read device id 1"},
	{4, "read reg"},
	{5, "write reg"},
	{6, "change reg"},
	{7, "read all reg"},
	{8, "chip initialize"},
};

static int reg_op(int fd, unsigned char reg, unsigned char *pvalue, read_write_t read_write) {
	int ret = 0;
#define BUFFER_LEN 128
	uint8_t tx[BUFFER_LEN] = {0xff};
	uint8_t rx[BUFFER_LEN] = {0xff};
	struct spi_ioc_transfer tr[2] = {
		{
			.tx_buf = (unsigned long)tx,
			.rx_buf = 0,
			.len = 0,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		},
		{
			.tx_buf = 0,
			.rx_buf = (unsigned long)rx,
			.len = 0,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		}
	};
	struct spi_ioc_transfer *ptr_tx = tr, *ptr_rx = tr + 1;
	int msg_count = 0;

	switch(read_write) {
		case spi_write: {
					tx[ptr_tx->len] = WRITE_REG(reg);
					(ptr_tx->len)++;
					tx[ptr_tx->len] = *pvalue;
					(ptr_tx->len)++;
					msg_count = 1;
				}
				break;
		case spi_read: {
				       tx[ptr_tx->len] = READ_REG(reg);
				       (ptr_tx->len)++;
				       ptr_rx->len = ptr_tx->len;
				       msg_count = 2;
			       }
			       break;
		default:
			       pabort("unspport spi message");
			       break;
	}

	printf("%s", "send:");
	for (ret = 0; ret < ptr_tx->len; ret++) {
		if ((ret != 0) && (ret % 16 == 0))
			puts("");
		printf("%.2X ", tx[ret]);
	}

	puts("");

	ret = ioctl(fd, SPI_IOC_MESSAGE(msg_count), tr);
	if (ret < 1)
		pabort("can't send spi message");


	printf("%s", "receive:");
	for (ret = 0; ret < ptr_rx->len; ret++) {
		if ((ret != 0) && (ret % 16 == 0))
			puts("");
		printf("%.2X ", rx[ret]);
		*pvalue = rx[ret];
	}
	puts("");
	return ret;
}

static int write_reg(int fd, unsigned char reg, unsigned char value) {
	int ret = 0;
	reg_op(fd, reg, &value, spi_write);
	return ret;
}

static unsigned char read_reg(int fd, unsigned char reg) {
	int ret = 0;
	unsigned char value;
	reg_op(fd, reg, &value, spi_read);
	return value;
}

static int read_and_write_reg(int fd, unsigned char reg, unsigned char mask, unsigned char value) {
	int ret = 0;
	unsigned char temp_value = read_reg(fd, reg);
	temp_value = temp_value & (~mask);
	value = value & mask;
	write_reg(fd, reg, temp_value | value);
	return ret;
}

static unsigned char tty_get_char(void) {
	unsigned int c;

	while(scanf("%x", &c) != 1) {
		scanf("%*s"); 
		printf("input error!!! press [[enter]] retry!!!\n");
	}
	printf("get %x\n", c);

	return c;
}

static int do_cmd(int fd, int c)
{
#define BUFFER_LEN 128

	int ret = 0;

	printf("c:%d\n", c);
	if(c >= ARRAY_SIZE(cmds)) {
		return;
	}

	switch(c) {
		case 1:
			write_reg(fd, 0x00, 0);
			break;
		case 2:
			{
				unsigned char value;
				value = read_reg(fd, 0x00);
			}
			break;
		case 3:
			{
				unsigned char value;
				value = read_reg(fd, 0x01);
			}
			break;
		case 4:
			{
				unsigned char reg;
				unsigned char value;

				printf("input reg addr:");
				reg = tty_get_char();
				value = read_reg(fd, reg);
			}
			break;
		case 5:
			{
				unsigned char reg;
				unsigned char value;

				printf("input reg addr:");
				reg = tty_get_char();
				printf("input value:");
				value = tty_get_char();
				write_reg(fd, reg, value);
			}
			break;
		case 6:
			{
				unsigned char reg;
				unsigned char mask;
				unsigned char value;

				printf("input reg addr:");
				reg = tty_get_char();
				printf("input value mask:");
				mask = tty_get_char();
				printf("input value:");
				value = tty_get_char();
				read_and_write_reg(fd, reg, mask, value);
			}
			break;
		case 7:
			{
				unsigned char value;
				unsigned char reg = 0x00;
				for(reg = 0; reg < 0x1e; reg++) {
					printf("query reg:%02x\n", reg);
					value = read_reg(fd, reg);
					puts("");
				}
			}
			break;
		case 8: 
			{
				struct reg_default {
					unsigned int reg;
					unsigned int def;
				};

				static const struct reg_default wm8804_reg_defaults[] = {
					{ 3,  0x21 },     /* R3  - PLL1 */
					{ 4,  0xFD },     /* R4  - PLL2 */
					{ 5,  0x36 },     /* R5  - PLL3 */
					{ 6,  0x07 },     /* R6  - PLL4 */
					{ 7,  0x16 },     /* R7  - PLL5 */
					{ 8,  0x18 },     /* R8  - PLL6 */
					{ 9,  0xFF },     /* R9  - SPDMODE */
					{ 10, 0x00 },     /* R10 - INTMASK */
					{ 18, 0x00 },     /* R18 - SPDTX1 */
					{ 19, 0x00 },     /* R19 - SPDTX2 */
					{ 20, 0x00 },     /* R20 - SPDTX3 */
					{ 21, 0x71 },     /* R21 - SPDTX4 */
					{ 22, 0x0B },     /* R22 - SPDTX5 */
					{ 23, 0x70 },     /* R23 - GPO0 */
					{ 24, 0x57 },     /* R24 - GPO1 */
					{ 26, 0x42 },     /* R26 - GPO2 */
					{ 27, 0x06 },     /* R27 - AIFTX */
					{ 28, 0x06 },     /* R28 - AIFRX */
					{ 29, 0x80 },     /* R29 - SPDRX1 */
					{ 30, 0x07 },     /* R30 - PWRDN */
				};
				
				int count = sizeof(wm8804_reg_defaults) / sizeof(struct reg_default);
				int i;
				for(i = 0; i < count; i++) {
					write_reg(fd, wm8804_reg_defaults[i].reg, wm8804_reg_defaults[i].def);	
				}

				write_reg(fd, 0x15, 0x38);//txsrc is spdif
				write_reg(fd, 0x1c, 0x46);//master output
				//write_reg(fd, 0x1c, 0x06);//slave input
				write_reg(fd, 0x1e, 0x00);//spdif enable | pll enable
				write_reg(fd, 0x1b, 0x0a);//aiftx 24bit, i2s
				
				//write_reg(fd, 0x1e, 0x00);//spdif enable | pll enable
				//write_reg(fd, 0x15, 0x38);//txsrc is spdif
				//write_reg(fd, 0x1c, 0x46);//master output
			}
			break;
		default:
			pabort("unspport spi message");
			break;
	}

	return ret;
}

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
			"  -s --speed    max speed (Hz)\n"
			"  -d --delay    delay (usec)\n"
			"  -b --bpw      bits per word \n"
			"  -l --loop     loopback\n"
			"  -H --cpha     clock phase\n"
			"  -O --cpol     clock polarity\n"
			"  -L --lsb      least significant bit first\n"
			"  -C --cs-high  chip select active high\n"
			"  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
			case 'D':
				device = optarg;
				break;
			case 's':
				speed = atoi(optarg);
				break;
			case 'd':
				delay = atoi(optarg);
				break;
			case 'b':
				bits = atoi(optarg);
				break;
			case 'l':
				mode |= SPI_LOOP;
				break;
			case 'H':
				mode |= SPI_CPHA;
				break;
			case 'O':
				mode |= SPI_CPOL;
				break;
			case 'L':
				mode |= SPI_LSB_FIRST;
				break;
			case 'C':
				mode |= SPI_CS_HIGH;
				break;
			case '3':
				mode |= SPI_3WIRE;
				break;
			case 'N':
				mode |= SPI_NO_CS;
				break;
			case 'R':
				mode |= SPI_READY;
				break;
			default:
				print_usage(argv[0]);
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int fd;
	int i;
	int c;

	parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	while(1)
	{
		printf("\n\n\n========================================\n");
		for(i = 0; i < ARRAY_SIZE(cmds); i++)
		{
			printf("%d. %s \n", i, cmds[i].cmd_str);
		}
		printf("========================================\n");
		printf(">>");
		if(scanf("%d", &c) != 1) {
			scanf("%*s");
			continue;
		}
		if(c == 0)
			break;
		else
			do_cmd(fd, c);
	}

	close(fd);

	return ret;
}
