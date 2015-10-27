#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>//kmalloc
#include <linux/vmalloc.h>//vmalloc()
#include <linux/types.h>//ssize_t
#include <linux/fs.h>//file_operaiotns
#include <linux/uaccess.h>//copy_from_user

#define MEM_MALLOC_SIZE 4096 ////缓冲区大小
#define MEM_MAJOR	240 ////主设备号
#define MEM_MINOR	0

char *mem_spvm = NULL; ////缓冲区指针，指向内存区
struct cdev *mem_cdev = NULL; //字符设备对象指针
struct class *mem_class = NULL; //设备类指针

static int __init mem_init(void);
static void __exit mem_exit(void);
static int mem_open(struct inode *inode,struct file *filp);
static int mem_release(struct inode *inode, struct file *filp);
static ssize_t mem_read(struct file *filp,char __user *buf,size_t count,loff_t *fpos);
static ssize_t mem_write(struct file *filp, const char __user *buf,size_t count ,loff_t *fops);

static const struct file_operations mem_fops={
	.owner = THIS_MODULE,
	.open = mem_open,
	.release = mem_release,
	.read = mem_read,
	.write = mem_write,
};

static int __init mem_init(void)
{
	int ret;
	//创建设备号 主次设备号
	int devno = MKDEV(MEM_MAJOR,MEM_MINOR);
	printk("mem_init initial...\n");

	//开辟内核内存缓冲区
	mem_spvm = (char *)vmalloc(MEM_MALLOC_SIZE);
	if(mem_spvm == NULL)
	{
		printk("vmalloc mem_spvm error\n");
		return -ENOMEM;//
	}

	//
	mem_cdev = cdev_alloc();
	if(mem_cdev == NULL)
	{
		printk("cdev_alloc error\n");
		return -ENOMEM;
	}
	cdev_init(mem_cdev,&mem_fops);
	mem_cdev->owner = THIS_MODULE;
	ret = cdev_add(mem_cdev,devno,1);//将字符设备键入内核系统
	if(ret)
	{
		cdev_del(mem_cdev);
		mem_cdev = NULL;
		printk("cdev_add error\n");
		return -1;
	}

	//
	mem_class = class_create(THIS_MODULE,"ywx_class_char");
	if(IS_ERR(mem_class))
	{
		printk("class_create error..\n");
		return -1;
	}
	device_create(mem_class,NULL,MKDEV(MEM_MAJOR,MEM_MINOR),NULL,"ywx_device_char");

	printk("init finished..\n");
	return 0;
}

static void __exit mem_exit(void)
{
	printk("mem_exit starting..\n");
	if(mem_cdev != NULL)
		cdev_del(mem_cdev);
	printk("cdev_del ok\n");

	device_destroy(mem_class,MKDEV(MEM_MAJOR,MEM_MINOR));
	class_destroy(mem_class);

	if(mem_spvm != NULL)
		vfree(mem_spvm);

	printk("vfree ok\n");
	printk("mem_exit finished..\n");
}

static int mem_open(struct inode *inode,struct file *filp)
{
	printk("open vmalloc space..\n");
	try_module_get(THIS_MODULE);//模块引用计数器自加
	printk("open vamlloc space ok..\n");
	return 0;
}
static int mem_release(struct inode *inode, struct file *filp)
{
	printk("close vmalloc space..\n");
	module_put(THIS_MODULE);//模块引用计数器自减
	return 0;
}
static ssize_t mem_read(struct file *filp,char __user *buf,size_t count,loff_t *fpos)
{
	int ret = -1;
	char *tmp;
	printk("copy data to the user space\n");
	tmp = mem_spvm;
	if(count > MEM_MALLOC_SIZE)
		count = MEM_MALLOC_SIZE;
	if(tmp != NULL)//将内核数据写入到用户空间
		ret = copy_to_user(buf,tmp,count);
	if(ret == 0)
	{
		printk("read copy data success\n");
		return count;
	}
	else
	{
		printk("read copy data error\n");
		return 0;
	}
}
static ssize_t mem_write(struct file *filp, const char __user *buf,size_t count ,loff_t *fops)
{
	int ret = -1;
	char *tmp;
	printk("read data from the user space.\n");
	tmp = mem_spvm;
	if(count > MEM_MALLOC_SIZE)
		count = MEM_MALLOC_SIZE;
	if(tmp != NULL)
		ret = copy_from_user(tmp,buf,count);
	if(ret == 0)
	{
		printk("write copy data success.\n");
		return count;
	}
	else
	{
		printk("write copy data error.\n");
		return 0;	
	}
}

MODULE_LICENSE("GPL");
module_init(mem_init);
module_exit(mem_exit);
