
#include "mboot.h"
#include "bootparam.h"

/**************************************************************/

#define RAW_MEMSIZE (2032-1024)
#define LOW_MEMSIZE  240
#define HIGH_MEMSIZE (RAW_MEMSIZE-LOW_MEMSIZE)

#define SMBIOS_PHYSICAL_ADDRESS 0x8fffe000
#define BOOTPARAM_ADDRESS       0x8ffe0000
#define CMDLINE_ADDRESS         0x8fffd000


struct loongson_bootparam_all {
	struct boot_params bp;

	struct efi_memory_map_loongson g_map;
	struct efi_cpuinfo_loongson g_cpuinfo;
	struct system_loongson g_system;
	struct irq_source_routing_table g_irq;
	struct interface_info g_interface;
	struct board_devices g_board;
	struct loongson_special_attribute g_special;
};

struct loongson_bootparam_all *lbp_all = (void*)BOOTPARAM_ADDRESS;


struct linux_cmdline {
	char *argv[64];
	char cmdstr[1024];
};

struct linux_cmdline *lcmd = (void*)CMDLINE_ADDRESS;
int cmdline_argc;

/**************************************************************/

void init_memory_map(void)
{
	struct efi_memory_map_loongson *emap = &lbp_all->g_map;

	emap->nr_map = 6; 
	emap->mem_freq = 325000000; //325M

	emap->map[0].node_id = 0;
	emap->map[0].mem_type = 1;
	emap->map[0].mem_start = 0x01000000;
	emap->map[0].mem_size = LOW_MEMSIZE - 112;

	emap->map[1].node_id = 0;
	emap->map[1].mem_type = 2;
	emap->map[1].mem_start = 0x110000000;
	emap->map[1].mem_size = HIGH_MEMSIZE;
  
	emap->map[2].node_id = 0;
	emap->map[2].mem_type = 10;
	//emap->map[2].mem_start = SMBIOS_PHYSICAL_ADDRESS;
	emap->map[2].mem_start = 0;
	emap->map[2].mem_size = 0;

	emap->map[3].node_id = 0;
	emap->map[3].mem_type = 3;
	emap->map[3].mem_start = 0x120000000;
	emap->map[3].mem_size = 256;
}

void init_cpu_info(void)
{
	struct efi_cpuinfo_loongson *c = &lbp_all->g_cpuinfo;
	u32 available_core_mask = 0;

	c->processor_id = 0x6305;
	c->cputype  = Loongson_3A;
	c->cpu_clock_freq = 800000000;
	c->total_node = 1;
	c->nr_cpus = 1;
	c->cpu_startup_core_id = (available_core_mask >> 16) & 0xf;
}
 
void init_system_loongson(void)
{
	struct system_loongson *s = &lbp_all->g_system;
	s->ccnuma_smp = 1;
	s->ccnuma_smp = 0;
	s->sing_double_channel = 1;
}

void init_irq_source(void)
{
	struct irq_source_routing_table *irq_info = &lbp_all->g_irq;

	irq_info->PIC_type = 0;
	irq_info->ht_int_bit = 1<<24;
	irq_info->ht_enable = 0x0000d17b;
	irq_info->node_id = 0;
	irq_info->pci_mem_start_addr = 0x40000000ul;
	irq_info->pci_mem_end_addr = 0x7ffffffful;
}

void init_interface_info(void)
{
	struct interface_info *inter = &lbp_all->g_interface;
	int flashsize = 0x00400000;

	printk("  g_interface: %08x\n", inter);

	inter->vers = 0x0001;
	inter->size = flashsize/0x400;
	inter->flag = 1;
	strcpy(inter->description, "MBOOT_v0.1_20140519");
}

void board_devices_info(void)
{
	struct board_devices *bd = &lbp_all->g_board;

	printk("  g_bd: %08x\n", bd);

	strcpy(bd->name, "Consys CSM361 board (ls2h)");
	bd->num_resources = 10;
}


void init_special_info(void)
{
	struct loongson_special_attribute  *special = &lbp_all->g_special;
	char update[11] = "2014-05-16";

	strcpy(special->special_name, update);
	special->resource[0].flags = 0;
	special->resource[0].start = 0;
	special->resource[0].end = 16;
	strcpy(special->resource[0].name,"SPMODULE");
}

/**************************************************************/

void init_boot_param(void)
{
	struct loongson_params *lp;

	memset(lbp_all, 0, sizeof(*lbp_all));
	lp = &(lbp_all->bp.efi.smbios.lp);

	lp->memory_offset         = (u64)((u8*)&lbp_all->g_map       - (u8*)lp);
	lp->cpu_offset            = (u64)((u8*)&lbp_all->g_cpuinfo   - (u8*)lp);
	lp->system_offset         = (u64)((u8*)&lbp_all->g_system    - (u8*)lp);
	lp->irq_offset            = (u64)((u8*)&lbp_all->g_irq       - (u8*)lp);
	lp->interface_offset      = (u64)((u8*)&lbp_all->g_interface - (u8*)lp);
	lp->boarddev_table_offset = (u64)((u8*)&lbp_all->g_board     - (u8*)lp);
	lp->special_offset        = (u64)((u8*)&lbp_all->g_special   - (u8*)lp);

	lbp_all->bp.efi.smbios.vers = 0;
	lbp_all->bp.efi.smbios.vga_bios = 0;

	init_memory_map();
	init_cpu_info();
	init_system_loongson();
	init_irq_source();
	init_interface_info();
	board_devices_info();
	init_special_info();

}

/**************************************************************/

char default_cmdline[] = "g root=/dev/sda1 console=ttyS0,115200 udevdebug udevtrace udevlog";
//char default_cmdline[] = "g root=/dev/sda1 console=ttyS0,115200";

void init_cmdline(void)
{
	char *p;
	int n;

	strcpy(lcmd->cmdstr, default_cmdline);
	p = lcmd->cmdstr;
	n = 0;

	while(*p){
		lcmd->argv[n] = p;
		n++;
		while((*p) && (*p!=' ')) p++;
		if(*p){
			*p = 0;
			p++;
		}
	}

	cmdline_argc = n;
}

/**************************************************************/

void find_linux_addr(u32 tmp_addr, u32 *load_addr, u32 *load_size, u32 *entry_addr)
{
	int i;

	for(i=0; i<0x00020000; i+=4){
		if(*(u32*)(tmp_addr+i)==0x016f2818){
			*load_addr  = *(u32*)(tmp_addr+i+4);
			*load_size  = *(u32*)(tmp_addr+i+8);
			*entry_addr = *(u32*)(tmp_addr+i+12);
			return;
		}
	}

	*load_addr  = 0x80200000;
	*load_size  = 0x01000000;
	*entry_addr = 0x80205950;
}


void boot_linux(void)
{
	u32 dl_addr = 0x82000000;
	u32 load_addr, load_size, entry_addr;
	void (*go_linux)(int argc, char *argv[], u32 bp_addr, u32 unuse);

	cmdline_argc = 0;

	printk("Start TFTP download at %08x ...\n", dl_addr);
	tftp_receive((void*)dl_addr, 0x02000000, "vmlinuz.bin");

	find_linux_addr(dl_addr, &load_addr, &load_size, &entry_addr);
	load_size -= load_addr;
	printk("vmlinuz.bin: load_addr=%08x load_size=%08x entry_addr=%08x\n", load_addr, load_size, entry_addr);
	go_linux = (void*)entry_addr;
	memcpy((void*)load_addr, (void*)dl_addr, load_size);

	printk("Init boot param ... %08x\n", sizeof(*lbp_all));
	init_boot_param();
	printk("Init linux command line ...\n");
	init_cmdline();

	printk("Jump to vmlinuz ...\n");
	go_linux(cmdline_argc, lcmd->argv, (u32)lbp_all, 0);
}



/**************************************************************/

