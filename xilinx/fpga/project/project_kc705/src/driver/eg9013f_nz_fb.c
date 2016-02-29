/*
 *    Filename: eg9013f_nz_fb.c
 *     Version: 0.1.0
 * Description: eg9013f_nz LCD framebuffer driver
 *     License: GPLv2
 *     Depends: eg9013f_nz
 *
 *      Author: xiaofei
 *        Date: 2016-02-29
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/fb.h>

#include "pcie.h"
#include "utils/xiaofei_debug.h"
#include "eg9013f_nz_fb.h"

#define EG9013F_NZ_FB_NAME "eg9013f_nz"

static struct fb_fix_screeninfo eg9013f_nz_fb_fix = {
	.id = "eg9013f_nz",
	.type = FB_TYPE_PACKED_PIXELS,
	.visual = FB_VISUAL_MONO10,
	.xpanstep = 0,
	.ypanstep = 0,
	.ywrapstep = 0,
	.line_length = EG9013F_NZ_WIDTH / 8,
	.accel = FB_ACCEL_NONE,
};

static struct fb_var_screeninfo eg9013f_nz_fb_var = {
	.xres = EG9013F_NZ_WIDTH,
	.yres = EG9013F_NZ_HEIGHT,
	.xres_virtual = EG9013F_NZ_WIDTH,
	.yres_virtual = EG9013F_NZ_HEIGHT,
	.bits_per_pixel = 1,
	.red = { 0, 1, 0 },
      	.green = { 0, 1, 0 },
      	.blue = { 0, 1, 0 },
	.left_margin = 0,
	.right_margin = 0,
	.upper_margin = 0,
	.lower_margin = 0,
	.vmode = FB_VMODE_NONINTERLACED,
};

static int eg9013f_nz_fb_mmap(struct fb_info *info, struct vm_area_struct *vma) {
	return remap_vmalloc_range(vma, info->screen_base, 0);
}

static struct fb_ops eg9013f_nz_fb_ops = {
	.owner = THIS_MODULE,
	.fb_read = fb_sys_read,
	.fb_write = fb_sys_write,
	.fb_fillrect = sys_fillrect,
	.fb_copyarea = sys_copyarea,
	.fb_imageblit = sys_imageblit,
	.fb_mmap = eg9013f_nz_fb_mmap,
};

int eg9013f_nz_fb_add(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
 	struct fb_info *info;
	int size;

	size = EG9013F_NZ_SIZE;
	//size = PAGE_ALIGN(EG9013F_NZ_SIZE);
	//if(unlikely(size == 0)) {
	//	ret = -EINVAL;
	//	goto invalid_size;
	//}

	kc705_pci_dev->eg9013f_nz_buffer = (char *)vzalloc(size);
	if(kc705_pci_dev->eg9013f_nz_buffer == NULL) {
		mydebug("alloc frame buffer memory failed.\n");
		ret = -1;
		goto alloc_fb_memory_failed;
	}

 	info = framebuffer_alloc(0, &(kc705_pci_dev->pdev->dev));
	if (info == NULL) {
		ret = -1;
		goto alloc_framebuffer_failed;
	}

	info->screen_base = (char __iomem *)kc705_pci_dev->eg9013f_nz_buffer;
	info->screen_size = EG9013F_NZ_SIZE;
	info->fbops = &eg9013f_nz_fb_ops;
	info->fix = eg9013f_nz_fb_fix;
	info->var = eg9013f_nz_fb_var;
	info->pseudo_palette = NULL;
	info->par = NULL;
	info->flags = FBINFO_FLAG_DEFAULT;

	if (register_framebuffer(info) < 0) {
		ret = -1;
		goto register_framebuffer_failed;
	}

	kc705_pci_dev->fb_info = info;
	mydebug("%s frame buffer device\n", info->fix.id);

	return ret;

	unregister_framebuffer(info);
register_framebuffer_failed:
	framebuffer_release(info);
alloc_framebuffer_failed:
	vfree(kc705_pci_dev->eg9013f_nz_buffer);
	kc705_pci_dev->eg9013f_nz_buffer = NULL;
alloc_fb_memory_failed:
//invalid_size:
	return ret;
}

int eg9013f_nz_fb_remove(kc705_pci_dev_t *kc705_pci_dev) {
	if (kc705_pci_dev->fb_info) {
		unregister_framebuffer(kc705_pci_dev->fb_info);
		framebuffer_release(kc705_pci_dev->fb_info);
		kc705_pci_dev->fb_info = NULL;
	}
	if(kc705_pci_dev->eg9013f_nz_buffer != NULL) {
		vfree(kc705_pci_dev->eg9013f_nz_buffer);
		kc705_pci_dev->eg9013f_nz_buffer = NULL;
	}

	return 0;
}
