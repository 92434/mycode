
CROSS = mips-linux-
CC = $(CROSS)gcc
LD = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump

# where do you want the text to execute? Define this to move
TEXT_START = 0x8fc00000
STACK_SIZE = 0x00004000

# define options for compilation
CFLAGS = -Wall -Os -fno-builtin -march=loongson2f -G0 -mno-abicalls


# define options for linkage. Prevent the inclusion of standard start
# code and libraries.
LDFLAGS = -march=loongson2f -fno-builtin -nostartfiles -nodefaultlibs -T ld.S
LDFLAGS += -Wl,--defsym,TEXT_START=$(TEXT_START) -Wl,--defsym,STACK_SIZE=$(STACK_SIZE)


# define options for the objdump
DUMPFLAGS = -xds

# use variables to refer to init code in case it changes
MIPSINIT  = mipsinit.o main.o tiny_xm.o spi.o cpu.o printk.o string.o
MIPSINIT += dwc_gmac.o
MIPSINIT += ip_udp.o tftp.o
MIPSINIT += bootparam.o memtest.o

#
# define build targets
#
all: go.bin

clean:
	rm -f *.o *.elf *.bin *.dump

# build s-record with init code and c files linked together
go.bin: $(MIPSINIT)
	$(LD) $(LDFLAGS) -o go.elf $(MIPSINIT) $(C_OBJS)
	$(OBJDUMP) $(DUMPFLAGS) go.elf > go.dump
	$(OBJCOPY) -O binary -R .comment go.elf go.bin
	cp go.bin /hgfs
	
# handle compilation of C files
%.o:%.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -c $<
%.o:%.c
	$(CC) $(CFLAGS) -c $<


