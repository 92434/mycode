/*
 * glyphy.c
 *
 * glyphy Driver
 *
 * Author: xiaofei.
 *
 */
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/cdev.h>
#include <linux/slab.h>
//#include <linux/fcntl.h>
//#include <linux/sysctl.h>
#include <linux/fs.h>
#include <linux/of_address.h>
//#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/mm.h>
//#include <linux/dma-mapping.h>
//#include <linux/dmaengine.h>
//#include <linux/amba/xilinx_dma.h>
//#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>

#include "glyphy.h"

#define GLYPHY_FPGA_STATUS_OFFSET (0 * 4) //status
#define GLYPHY_FPGA_CONTROL_OFFSET (1 * 4) //control
#define GLYPHY_FPGA_DATA_OFFSET (2 * 4) //data

#define GLYPHY_FPGA_STATUS_BIT_DONE BIT(0) //0:processing 1:done
#define GLYPHY_FPGA_STATUS_BIT_INT BIT(0) //0:processing 1:done

#define GLYPHY_FPGA_CONTROL_BIT_CS BIT(0) //0:not select 1:select
#define GLYPHY_FPGA_CONTROL_BIT_WR BIT(1) //0:read 1:write
#define GLYPHY_FPGA_CONTROL_BIT_AD BIT(2) //0:index 1:value
#define GLYPHY_FPGA_CONTROL_BIT_PRINT_ENABLE BIT(3) //0:notcare 1:fpga print
#define GLYPHY_FPGA_CONTROL_BIT_CLEAR_ISR BIT(4) //0:notcare 1:reset interrupt

#define GLYPHY_FPGA_OUT_PIX_WIDTH_INDEX 33
#define GLYPHY_FPGA_OUT_PIX_HEIGHT_INDEX 34

//#define SZ_4M 0x400000

struct mmap_info {
	void *user_buffer;
	void *kernel_buffer;
	ptrdiff_t user_buffer_offset;
	unsigned long map_size;
	struct page **pages;
};

struct glyphy_device {
	void __iomem *regs;
	struct device *dev;
	struct miscdevice glyphy_miscdev;
	struct mmap_info mmap_info;
	struct content_info content_info;
	int irq;
	struct tasklet_struct tasklet;    /* Cleanup work after irq */
	struct completion cmp;
};



static void do_tasklet(unsigned long data)
{
	struct glyphy_device *pdevice = (struct glyphy_device *)data;

	complete(&pdevice->cmp);
}

static int glyphy_open(struct inode *ip, struct file *filp)
{
	return 0;
}


static int set_content_info(struct glyphy_device *pdevice, struct content_info *pcontent_info) {
	int ret = 0;

	pdevice->content_info.width = pcontent_info->width;
	pdevice->content_info.height = pcontent_info->height;
	pdevice->content_info.glyphy_info = pcontent_info->glyphy_info;
	pdevice->content_info.dir = pcontent_info->dir;

	return ret;
}

static int fpga_out_glyphy_info(struct glyphy_device *pdevice, unsigned int out_pix_width, unsigned int out_pix_height) {
	int ret = 0;
	unsigned int control;

	control = GLYPHY_FPGA_CONTROL_BIT_CS | GLYPHY_FPGA_CONTROL_BIT_WR;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(GLYPHY_FPGA_OUT_PIX_WIDTH_INDEX, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);

	control |= GLYPHY_FPGA_CONTROL_BIT_AD;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(out_pix_width, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);


	control = GLYPHY_FPGA_CONTROL_BIT_CS | GLYPHY_FPGA_CONTROL_BIT_WR;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(GLYPHY_FPGA_OUT_PIX_HEIGHT_INDEX, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);

	control |= GLYPHY_FPGA_CONTROL_BIT_AD;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(out_pix_height, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);
	return ret;
}

static int fpga_out_glyphy_line(struct glyphy_device *pdevice, unsigned int index, unsigned int value) {
	int ret = 0;
	unsigned int control = GLYPHY_FPGA_CONTROL_BIT_CS | GLYPHY_FPGA_CONTROL_BIT_WR;

	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(index, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);
	control |= GLYPHY_FPGA_CONTROL_BIT_AD;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	writel(value, pdevice->regs + GLYPHY_FPGA_DATA_OFFSET);

	return ret;
}

static int out_one_glyphy_dir_row(struct glyphy_device *pdevice, int x, int y) {
	int ret = 0;
	struct content_info content_info = pdevice->content_info;
	struct glyphy_info glyphy_info = pdevice->content_info.glyphy_info;
	struct mmap_info mmap_info = pdevice->mmap_info;
	int hy, wx;

	int glyphy_width_by_byte = (glyphy_info.pix_width + (BIT_PER_BYTE - 1)) / BIT_PER_BYTE;

	int content_step_by_byte = glyphy_width_by_byte * content_info.width;
	int glyphy_width_out_count = (glyphy_width_by_byte + (FPGA_PROCESS_WIDTH_BY_BYTE - 1)) / FPGA_PROCESS_WIDTH_BY_BYTE;
	int glyphy_height_out_count = (glyphy_info.pix_height + (FPGA_PROCESS_HEIGHT - 1)) / FPGA_PROCESS_HEIGHT;

	for(hy = 0; hy < glyphy_height_out_count; hy++) {
		for(wx = 0; wx < glyphy_width_out_count; wx++) {
			int glyphy_current_width_by_byte;
			int glyphy_current_width_by_pix;
			int glyphy_current_height;
			int h, w;

			if(glyphy_width_by_byte <= (wx + 1) * FPGA_PROCESS_WIDTH_BY_BYTE) {
				glyphy_current_width_by_byte = glyphy_width_by_byte - wx * FPGA_PROCESS_WIDTH_BY_BYTE;
				glyphy_current_width_by_pix = glyphy_info.pix_width - wx * FPGA_PROCESS_WIDTH;
			} else {
				glyphy_current_width_by_byte = FPGA_PROCESS_WIDTH_BY_BYTE;
				glyphy_current_width_by_pix = FPGA_PROCESS_WIDTH;
			}

			if(glyphy_info.pix_height <= (hy + 1) * FPGA_PROCESS_HEIGHT) {
				glyphy_current_height = glyphy_info.pix_height - hy * FPGA_PROCESS_HEIGHT;
			} else {
				glyphy_current_height = FPGA_PROCESS_HEIGHT;
			}

			for(h = 0; h < glyphy_current_height; h++) {
				unsigned int value = 0;//length refer to fpga process width
				for(w = 0; w < glyphy_current_width_by_byte; w++) {
					char *start = mmap_info.kernel_buffer;

					start += content_step_by_byte * glyphy_info.pix_height * y;
					start += content_step_by_byte * hy * FPGA_PROCESS_HEIGHT;
					start += content_step_by_byte * h;

					start += x * glyphy_width_by_byte;
					start += (wx * FPGA_PROCESS_WIDTH_BY_BYTE) + w;
					//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
					//printk("xiaofei: start:%p\n", start);

					value += ((unsigned int)(*start)) << (w * BIT_PER_BYTE);
				}
				fpga_out_glyphy_line(pdevice, (unsigned int)h, value);
				//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
				//printk("xiaofei: h:%02d-->value:%08x\n", h, value);
			}
			fpga_out_glyphy_info(pdevice, (unsigned int)glyphy_current_width_by_pix, (unsigned int)glyphy_current_height);
		}
	}


	return ret;
}

static int out_one_glyphy(struct glyphy_device *pdevice, int x, int y, enum out_direction dir) {
	int ret = 0;
	if(dir == DIR_ROW) {
		out_one_glyphy_dir_row(pdevice, x, y);
	} else if(dir == DIR_COL) {
		//out_one_glyphy_dir_col(pdevice, x, y);
	} else {
	}
	return ret;
}

static int start_fpga_print(struct glyphy_device *pdevice) {
	int ret = 0;
	unsigned int control;

	control = GLYPHY_FPGA_CONTROL_BIT_PRINT_ENABLE;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	return ret;
}

static bool is_fpga_done(struct glyphy_device *pdevice) {
	unsigned int status;

	status = readl(pdevice->regs + GLYPHY_FPGA_STATUS_OFFSET);
	return (status & GLYPHY_FPGA_CONTROL_BIT_PRINT_ENABLE != 0);
}

static int fpga_stop(struct glyphy_device *pdevice) {
	int ret = 0;
	unsigned int control;

	control = 0;
	writel(control, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);
	return ret;
}


static irqreturn_t intr_handler(int irq, void *data)
{
	struct glyphy_device *pdevice = (struct glyphy_device *)data;
	u32 control;

	fpga_stop(pdevice);
	tasklet_schedule(&pdevice->tasklet);
	return IRQ_HANDLED;
}

static int wait_for_glyphy_intr(struct glyphy_device *pdevice, glyphy_result_t *result) {
	int ret = 0;
	unsigned long tmo = msecs_to_jiffies(1000);

	result->is_timeout = false;
	tmo = wait_for_completion_timeout(&pdevice->cmp, tmo);
	if(tmo == 0) {
		result->is_timeout = true;
	}

	return ret;
}

static int wait_for_glyphy_poll(struct glyphy_device *pdevice, glyphy_result_t *result) {
	int ret = 0;

	unsigned long tmo = msecs_to_jiffies(10);
	unsigned long expire = tmo + jiffies;
	result->is_timeout = false;
	while(!is_fpga_done(pdevice)) {
		if (time_after(jiffies, tmo)) {
			printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
			result->is_timeout = true;
			break;
		}
		usleep_range(50, 500);
	}

	return ret;
}

static int content_out_start(struct glyphy_device *pdevice) {
	int ret = 0;
	int x, y;

	struct content_info content_info = pdevice->content_info;

	for(y = 0; y < content_info.height; y++) {
		for(x = 0; x < content_info.width; x++) {
			glyphy_result_t result;

			out_one_glyphy(pdevice, x, y, content_info.dir);
			start_fpga_print(pdevice);
			wait_for_glyphy_poll(pdevice, &result);
			fpga_stop(pdevice);
			if(result.is_timeout) {
				ret = -1;
				break;
			}
		}
	}
	return ret;
}

static long glyphy_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct miscdevice *pmiscdev = file->private_data;
	struct glyphy_device *pdevice = container_of(pmiscdev, struct glyphy_device, glyphy_miscdev);

	switch (cmd) {
		case GLYPHY_IOCTL_SET_CONTENT_INFO:
			{
				struct content_info content_info;

				if(copy_from_user(&content_info, (u32 *)arg, sizeof(struct content_info))) {
					return -EFAULT;
				}
				set_content_info(pdevice, &content_info);
			}
			break;
		case GLYPHY_IOCTL_CONTENT_OUT_START:
			if(content_out_start(pdevice)) {
				return -EFAULT;
			}
			break;
		default:
			break;
	}
	return 0;
}

#if 1
static int free_mapped_buffer(struct glyphy_device *pdevice, struct vm_area_struct *vma) {
	int ret = 0;

	void *kernel_buffer = pdevice->mmap_info.kernel_buffer;
	void *user_buffer = pdevice->mmap_info.user_buffer;
	unsigned long map_size = pdevice->mmap_info.map_size;
	ptrdiff_t user_buffer_offset = pdevice->mmap_info.user_buffer_offset;
	struct page **pages = pdevice->mmap_info.pages;

	struct page **page;
	void *page_addr;

	if(vma == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(map_size == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(kernel_buffer == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(user_buffer == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(user_buffer_offset == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(pages == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	for(page_addr = kernel_buffer; page_addr < kernel_buffer + map_size; page_addr += PAGE_SIZE) {
		page = &pages[(page_addr - kernel_buffer) / PAGE_SIZE];
		if(*page == 0) {
			continue;
		}
		zap_page_range(vma, (uintptr_t)page_addr + user_buffer_offset, PAGE_SIZE, NULL);
		unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		//printk("*page:%p\n", *page);
		__free_page(*page);
	}
	kfree(pages);
	vfree(kernel_buffer);

	pdevice->mmap_info.user_buffer = 0;
	pdevice->mmap_info.kernel_buffer = 0;
	pdevice->mmap_info.user_buffer_offset = 0;
	pdevice->mmap_info.map_size = 0;
	pdevice->mmap_info.pages = 0;
}

static int glyphy_mmap(struct file *filp, struct vm_area_struct *vma) {
	int ret = 0;
	struct vm_struct *area;
	unsigned long size;
	void *start, *end;
	void *page_addr;
	struct page **page;

	struct miscdevice *pmiscdev = filp->private_data;
	struct glyphy_device *pdevice = container_of(pmiscdev, struct glyphy_device, glyphy_miscdev);


	if ((vma->vm_end - vma->vm_start) > SZ_4M)
		vma->vm_end = vma->vm_start + SZ_4M;

	size = vma->vm_end - vma->vm_start;
	if(size == 0) {
		return ret;
	}

	free_mapped_buffer(pdevice, vma);

	area = get_vm_area(size, VM_IOREMAP);
	if (area == NULL) {
		ret = -ENOMEM;
		goto get_vm_area_failed;
	}

	pdevice->mmap_info.user_buffer = vma->vm_start;
	pdevice->mmap_info.kernel_buffer = area->addr;
	pdevice->mmap_info.user_buffer_offset = pdevice->mmap_info.user_buffer - (uintptr_t)pdevice->mmap_info.kernel_buffer;

	start = pdevice->mmap_info.kernel_buffer;
	end = ((unsigned long)start + size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	pdevice->mmap_info.map_size = end - start;

	pdevice->mmap_info.pages = kzalloc(sizeof(pdevice->mmap_info.pages[0]) * ((end - start) / PAGE_SIZE), GFP_KERNEL);
	if (pdevice->mmap_info.pages == NULL) {
		ret = -ENOMEM;
		goto err_alloc_pages_failed;
	}

	for(page_addr = start; page_addr < end; page_addr += PAGE_SIZE) {
		int ret;
		struct vm_struct tmp_area;
		struct page **page_array_ptr;

		page = &pdevice->mmap_info.pages[(page_addr - start) / PAGE_SIZE];

		BUG_ON(*page);
		*page = alloc_page(GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);
		if (*page == NULL) {
			ret = -ENOMEM;
			pr_err("%d: alloc page failed at %p\n", __LINE__, page_addr);
			goto err_alloc_page_failed;
		}
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		//printk("*page:%p\n", *page);
		tmp_area.addr = page_addr;
		tmp_area.size = PAGE_SIZE + PAGE_SIZE /* guard page? */;
		page_array_ptr = page;
		ret = map_vm_area(&tmp_area, PAGE_KERNEL, &page_array_ptr);
		if (ret) {
			ret = -ENOMEM;
			pr_err("%d: failed to map page at %p in kernel\n", __LINE__, page_addr);
			__free_page(*page);
			goto err_map_kernel_failed;
		}


		ret = vm_insert_page(vma, (uintptr_t)page_addr + pdevice->mmap_info.user_buffer_offset, page[0]);
		if (ret) {
			ret = -ENOMEM;
			pr_err("%d: failed to map page at %lx in userspace\n", __LINE__, (uintptr_t)page_addr + pdevice->mmap_info.user_buffer_offset);
			unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
			__free_page(*page);
			goto err_vm_insert_page_failed;
		}
		/* vm_insert_page does not seem to increment the refcount */
	}

	return ret;

err_vm_insert_page_failed:
err_map_kernel_failed:
err_alloc_page_failed:
	free_mapped_buffer(pdevice, vma);
err_alloc_pages_failed:
	vfree(pdevice->mmap_info.user_buffer);
get_vm_area_failed:
	return ret;
}
#else
static int free_mapped_buffer(struct glyphy_device *pdevice, struct vm_area_struct *vma) {
	int ret = 0;
	void *kernel_buffer = pdevice->mmap_info.kernel_buffer;
	void *user_buffer = pdevice->mmap_info.user_buffer;
	unsigned long map_size = pdevice->mmap_info.map_size;

	if(vma == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(map_size == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(kernel_buffer == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	if(user_buffer == 0) {
		//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return ret;
	}

	//printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
	//printk("user_buffer:%p map_size:%08x\n", user_buffer, map_size);
	//printk("kernel_buffer:%p\n", kernel_buffer);
	zap_page_range(vma, (uintptr_t)user_buffer, map_size, NULL);
	vfree(kernel_buffer);

	pdevice->mmap_info.user_buffer = 0;
	pdevice->mmap_info.kernel_buffer = 0;
	pdevice->mmap_info.map_size = 0;
	return ret;
}

static int glyphy_mmap(struct file *filp, struct vm_area_struct *vma) {
	int ret = 0;
	unsigned long start = vma->vm_start;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long page, pos;

	struct miscdevice *pmiscdev = filp->private_data;
	struct glyphy_device *pdevice = container_of(pmiscdev, struct glyphy_device, glyphy_miscdev);

	if(size == 0) {
		return ret;
	}

	free_mapped_buffer(pdevice, vma);

	pos = vmalloc(size);
	if(pos == NULL) {
		ret = -ENOMEM;
		goto get_vm_area_failed;
	}

	pdevice->mmap_info.user_buffer = vma->vm_start;
	pdevice->mmap_info.kernel_buffer = pos;
	pdevice->mmap_info.map_size = size;

	while (size > 0) {
		page = vmalloc_to_pfn((void *)pos);
		if (remap_pfn_range(vma, start, page, PAGE_SIZE, PAGE_SHARED))
			return -EAGAIN;

		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		if (size > PAGE_SIZE)
			size -= PAGE_SIZE;
		else
			size = 0;
	}
	printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
	printk("user_buffer:%p map_size:%08x\n", pdevice->mmap_info.user_buffer, pdevice->mmap_info.map_size);
	printk("kernel_buffer:%p\n", pdevice->mmap_info.kernel_buffer);

	/* VM_IO | VM_DONTEXPAND | VM_DONTDUMP are set by remap_pfn_range() */

get_vm_area_failed:
	return ret;
}
#endif

static int glyphy_release(struct inode *ip, struct file *filp) {
	int ret = 0;
	struct miscdevice *pmiscdev = filp->private_data;
	struct glyphy_device *pdevice = container_of(pmiscdev, struct glyphy_device, glyphy_miscdev);

	void *user_buffer = pdevice->mmap_info.user_buffer;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	/* Find the first overlapping VMA */
	vma = find_vma(mm, user_buffer);
	if (!vma) {
		ret = -1;
		return ret;
	}


	if(vma == 0) {
		ret = -1;
		return ret;
	}

	free_mapped_buffer(pdevice, vma);
	return ret;
}

static const struct file_operations glyphy_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = glyphy_ioctl,
	.mmap = glyphy_mmap,
	.open = glyphy_open,
	.release = glyphy_release,
};

static int create_glyphy_cdev(struct glyphy_device *pdevice) {
	int ret;
	struct miscdevice *pmiscdev = &pdevice->glyphy_miscdev;
	pmiscdev->name = DRIVER_NAME;
	pmiscdev->minor = MISC_DYNAMIC_MINOR;
	pmiscdev->fops = &glyphy_fops;

	ret = misc_register(pmiscdev);
	if(ret != 0) {
		printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	return ret;
}

static int release_glyphy_cdev(struct glyphy_device *pdevice) {
	int ret = 0;
	struct miscdevice *pmiscdev = &pdevice->glyphy_miscdev;

	ret = misc_deregister(pmiscdev);
	if(ret != 0) {
		printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	return ret;
}

//static int __devinit glyphy_of_probe(struct platform_device *pdev)
static int glyphy_of_probe(struct platform_device *pdev)
{
	int ret;
	struct device_node *pnode;
	static struct glyphy_device *pdevice;
	struct resource *res;

	pdevice = devm_kzalloc(&pdev->dev, sizeof(*pdevice), GFP_KERNEL);
	if (!pdevice) {
		dev_err(&pdev->dev, "Not enough memory for device\n");
		ret = -ENOMEM;
		goto glyphy_alloc_failed;
	}

	pdevice->dev = &pdev->dev;

	pnode = pdev->dev.of_node;

	/* iomap registers */
	pdevice->regs = of_iomap(pnode, 0);
	//res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	//pdevice->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(pdevice->regs)) {
		dev_err(&pdev->dev, "unable to iomap registers\n");
		ret = -ENXIO;
		goto iomap_reg_failed;
	}

	writel(0, pdevice->regs + GLYPHY_FPGA_CONTROL_OFFSET);

	/* find the IRQ line, if it exists in the device tree */
	//pdevice->irq = irq_of_parse_and_map(pnode, 0);
	if(pdevice->irq != 0) {
		ret = request_irq(pdevice->irq, intr_handler, IRQF_SHARED,
				DRIVER_NAME, pdevice);
		if (ret) {
			dev_err(pdevice->dev, "unable to request IRQ %d\n", ret);
			goto request_irq_failed;
		}
		printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printk("xiaofei:%s irq is %x(%d)\n", dev_name(pdevice->dev), pdevice->irq, pdevice->irq);
		init_completion(&pdevice->cmp);
		tasklet_init(&pdevice->tasklet, do_tasklet, (unsigned long)pdevice);
	}

	ret = create_glyphy_cdev(pdevice);
	if(ret != 0) {
		printk("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto create_glyphy_cdev_failed;
	}

	platform_set_drvdata(pdev, pdevice);

	return ret;

create_glyphy_cdev_failed:
	if(pdevice->irq != 0) {
		free_irq(pdevice->irq, pdevice);
	}
request_irq_failed:
	if(pdevice->irq != 0) {
		irq_dispose_mapping(pdevice->irq);
	}
iomap_reg_failed:
	devm_kfree(&pdev->dev, pdevice);
glyphy_alloc_failed:
	return ret;
}

//static int __devexit glyphy_remove(struct platform_device *pdev)
static int glyphy_remove(struct platform_device *pdev)
{
	struct glyphy_device *pdevice = platform_get_drvdata(pdev);

	release_glyphy_cdev(pdevice);
	if(pdevice->irq != 0) {
		free_irq(pdevice->irq, pdevice);
		irq_dispose_mapping(pdevice->irq);
	}
	iounmap(pdevice->regs);
	devm_kfree(&pdev->dev, pdevice);

	return 0;
}

static const struct of_device_id glyphy_of_ids[] = {
	{ .compatible = "xlnx,myPrintWordip-v1-0-1.0" },
	{}
};

static struct platform_driver glyphy_of_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = glyphy_of_ids,
	},
	.probe = glyphy_of_probe,
	//.remove = __devexit_p(glyphy_remove),
	.remove = glyphy_remove,
};

static int __init glyphy_init(void)
{
	return platform_driver_register(&glyphy_of_driver);
}

static void __exit glyphy_exit(void)
{
	platform_driver_unregister(&glyphy_of_driver);
}

module_init(glyphy_init);
module_exit(glyphy_exit);

MODULE_DESCRIPTION("glyphy driver");
MODULE_LICENSE("GPL");
