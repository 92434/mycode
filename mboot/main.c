


#include "mboot.h"



int gets(char *buf, int len)
{
	int n, ch;

	puts("mboot> ");
	n = 0;
	while(1){
		while((ch=getc(1000))<0);
		if(ch==0x0d)
			break;
		putc(ch);
		buf[n] = ch;
		n++;
		if(n==len)
			break;
	}

	puts("\r\n");
	buf[n] = 0;
	return n;
}

int str2hex(char *str, int *hex)
{
	int n, p, ch;

	n = 0;
	p = 0;
	hex[0] = 0;
	while(1){
		ch = *str++;
		if(ch==' '){
			if(p){
				n++;
				hex[n] = 0;
				p = 0;
			}
			continue;
		}else if((ch>='0' && ch<='9')){
			ch -= '0';
		}else if((ch>='a' && ch<='f')){
			ch = ch-'a'+10;
		}else if((ch>='A' && ch<='F')){
			ch = ch-'A'+10;
		}else{
			if(p)
				n++;
			return n;
		}
		hex[n] = (hex[n]<<4)|ch;
		p++;
	}
}

int width, shift;

void dump(int addr)
{
	int i;

	i = 0;
	for(i=0; i<256; i+=width){
		if((i%16)==0){
			put_return();
			puthex(addr, 28);
			puts(": ");
		}
		if(width==1){
			puthex(*(u8*)(addr), shift);
		}else if(width==2){
			puthex(*(u16*)(addr), shift);
		}else if(width==4){
			puthex(*(u32*)(addr), shift);
		}
		addr += width;
		putc(' ');
	}
	put_return();
}

void download_run(void)
{
	u32 mboot_address = 0x8f000000;
	void (*go)(void) = (void*)mboot_address;

	printk("xmodem download at %08x ...\n", mboot_address);
	tiny_xmodem_recv((void*)mboot_address);
	printk("run from %08x ...\n", mboot_address);
	go();
}

int main(int arg0, char *argv[])
{
	int ch, argc;
	char str[128];
	int arg[4];

	width = 1;
	shift = 4;
	printk("mboot start!\n");

	cpu_init();

	printk("spi flash id: %04x\n", sf_readid());

	while(1){
		gets(str, 128);
		argc = str2hex(str+2, arg);

		if(str[1]=='b'){
			width = 1;
			shift = 4;
		}else if(str[1]=='w'){
			width = 2;
			shift = 12;
		}else if(str[1]=='d'){
			width = 4;
			shift = 28;
		}

/**************************************************************/
		if(str[0]=='d'){
			dump(arg[0]);
		}else if(str[0]=='r'){
			if(width==1){
				ch = *(u8*)arg[0];
			}else if(width==2){
				ch = *(u16*)arg[0];
			}else if(width==4){
				ch = *(u32*)arg[0];
			}
			puthex(arg[0], 4);
			putc(':');
			puthex(ch, shift);
			put_return();
		}else if(str[0]=='w'){
			puthex(arg[0], 4);
			putc('=');
			puthex(arg[1], shift);
			put_return();
			if(width==1){
				*(u8*)arg[0] = arg[1];
			}else if(width==2){
				*(u16*)arg[0] = arg[1];
			}else if(width==4){
				*(u32*)arg[0] = arg[1];
			}
/********************** cpu test ******************************/
		}else if(str[0]=='c'){
			if(argc==2){
				set_cpu_freq(arg[0]);
			}
			cpu_freq_show();
			cpu_l2_show();
/**************************************************************/

/********************** spi test ******************************/
        }else if(str[0]=='e'){
			printk("spi erase sector at %08x ...\n", arg[0]);
			sf_sector_erase(arg[0]);
        }else if(str[0]=='t'){
			printk("spi page %08x :\n", arg[0]);
			sf_read(arg[0], 256, (void*)0x80000000);
			dump(0x80000000);
        }else if(str[0]=='y'){
			int len;
			len = tiny_xmodem_recv((void*)0x80000000);
			sf_write(0, len, (void*)0x80000000);
		}else if(str[0]=='x'){
			download_run();
		}else if(str[0]=='z'){
			void (*go)(void) = (void*)0x8fc00000;
			go();
/********************** boot linux ****************************/
        }else if(str[0]=='p'){
			boot_linux();
/**************************************************************/
        }else if(str[0]=='m'){
			void mem_test(void);
			mem_test();
		}
	}
}

