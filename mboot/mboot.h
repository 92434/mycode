

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;
typedef   signed char  s8;
typedef   signed short s16;
typedef   signed int   s32;
typedef   signed long long s64;

#define REG8(x)  (*(volatile  u8*)(x))
#define REG16(x) (*(volatile u16*)(x))
#define REG(x)   (*(volatile u32*)(x))
#define REG64(x) (*(volatile u64*)(x))

#define NULL ((void*)0)

#include "regs_mips.h"
#include "ls2h.h"


int  get_clock_tick_rate(void);
void reset_timer(void);
u32  get_timer(void);
void udelay(int us);



int  putc(int ch);
int  puts(char *str);
int  puthex(int data, int w);
int  put_return(void);

int  getc(int timeout_ms);


int  tiny_xmodem_recv(unsigned char *dest);

void *memset(void *s, int v, int n);
void *memcpy(void *to, void *from, int n);
char *strcpy(char *dst, char *src);
char *strncpy(char *dst, char *src, int n);
int  strcmp(char *s1, char *s2);
int  strncmp(char *s1, char *s2, int n);
int  strlen(char *s);
int  strtoul(char *str, char **endptr, int requestedbase, int *ret);

int  printk(char *fmt, ...);

void mem_dump(char *str, void *buf, int length);

int tftp_receive(char *dlbuf, int dlsize, char *filename);


u32  get_cp0_status(void);
void set_cp0_status(u32 status);
void cpu_freq_show(void);
void set_cpu_freq(int div);
void cpu_l2_show(void);
void cpu_l2_config(void);
void tlb_init(void);
void fpu_init(void);
void cpu_init(void);

int ddr3_init(void);



int sf_readid(void);
int sf_sector_erase(int addr);
int sf_write(int addr, int len, u8 *buf);
int sf_read(int addr, int len, u8 *buf);

int nand_init(void);
int nand_scan(void);
int nand_read_pages(int page, u8 *buf, int num);
int nand_write_page(int page, u8 *buf);
int nand_erase_block(int page);



