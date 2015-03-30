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
static uint32_t speed = 500000;
static uint16_t delay = 0;

struct st_cmd
{
	uint8_t cmd;
	const char *cmd_str;
};

#if 0
static const struct st_cmd cmds[] =
{
	{0x00, "Quit"},
	{0x06, "Write enable"},
	{0x3A, "Enter OTP mode"},
	{0x04, "Write disable / Exit OTP"},
	{0x05, "read status register"},
	{0x01, "write status register"},
	{0x90, "Manufacture ID / Device ID"},
	{0x9F, "Read ID"},
	{0x03, "Read data"},
	{0x02, "page program"},
	{0x20, "Sector Erase / OTP Erase"},
};
#else
/* KH25L8006E */
static const struct st_cmd cmds[] =
{
	{0x00, "Quit"},
	{0x06, "Write enable"},
	{0x04, "Write disable"},
	{0x05, "read status register"},
	{0x01, "write status register"},
	{0xB1, "Enter OTP mode"},
	{0xC1, "Exit OTP mode"},
	{0x90, "Manufacture ID / Device ID"},
	{0x9F, "Read ID"},
	{0x03, "Read data"},
	//{0x02, "page program"},
	//{0x20, "Sector Erase / OTP Erase"},
};
#endif

static void do_cmd(int fd, int c)
{
	printf("c:%d\n", c);
	if(c >= ARRAY_SIZE(cmds)) {
		return;
	}

	int ret;
	uint8_t tx[128+4] = {0,};
	uint8_t rx[ARRAY_SIZE(tx)] = {0,};
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	int cmd_len = ARRAY_SIZE(tx);

	tx[0] = cmds[c].cmd;

	switch(tx[0]) {
		case 0x03:
		case 0x20:
		case 0x02:
			{
				unsigned int addr;
				printf("input addr:");
				if(scanf("%x", &addr) != 1) {
					scanf("%*s");
					return;
				}
				tx[1] = (unsigned char)(addr >> 16);
				tx[2] = (unsigned char)(addr >> 8);
				tx[3] = (unsigned char)(addr >> 0);
				cmd_len = 4;

				if(tx[0] == 0x20) {
					tr.len = 4;
				} else if(tx[0] == 0x02) {
					unsigned int value;
					int i;
					for(i = cmd_len; i < ARRAY_SIZE(tx); i++) {
						printf("input value:");
						if(scanf("%x", &value) != 1) {
							scanf("%*s");
							break;
						} else {
							tx[i] = value;
						}
					}
					tr.len = i;
				}
			}
			break;
		case 0x05:
		case 0x06:
		case 0x04:
		case 0xB1:
		case 0xC1:
		case 0x9F:
			cmd_len = 1;
			if(tx[0] == 0x05) {
				tr.len = 2;
			} else if(tx[0] == 0x9f) {
				tr.len = 4;
			} else {
				tr.len = 1;
			}
			break;
		case 0x90:
			{
				unsigned int type;
				printf("input type:(0=manufacture first,1=device first)");
				if(scanf("%x", &type) != 1) {
					scanf("%*s");
					return;
				}
				tx[3] = (unsigned char)(type >> 0);
				cmd_len = 4;
				tr.len = 6;
			}
			break;
		case 0x01:
			{
				unsigned int status;
				printf("input status:");
				if(scanf("%x", &status) != 1) {
					scanf("%*s");
					return;
				}
				tx[1] = (unsigned char)(status >> 0);
				cmd_len = 2;
				tr.len = 2;
			}
			break;
		default:
			pabort("unspport spi message");
			break;
	}


	for (ret = 0; ret < tr.len; ret++) {
		if (!(ret % 16))
			puts("");
		printf("%.2X ", tx[ret]);
	}

	puts("");

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	for (ret = 0; ret < cmd_len; ret++) {
		if (!(ret % 16))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	for (ret = cmd_len; ret < tr.len; ret++) {
		if (!((ret - cmd_len) % 16))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	puts("");

	{
#include <stdio.h>
		FILE *f = fopen("otp.bin", "w");
		fwrite(rx + cmd_len, sizeof(uint8_t), tr.len - cmd_len, f);
		fclose(f);
	}
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
		for(i = 0; i < ARRAY_SIZE(cmds); i++)
		{
			printf("%d. %s \n", i, cmds[i].cmd_str);
		}
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
