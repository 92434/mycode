#include "common.h"

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define regread(reg,data)		gt208l_read_reg(gt208l_client, reg, &data)
#define regwrite(reg,data)		gt208l_write_reg(gt208l_client, reg, data)

struct gt208l {
	struct i2c_client *client;
};

static struct i2c_client *gt208l_client;

static int gt208l_read_reg(struct i2c_client *client, u8 reg, u8 *data)
{
	int ret;
	struct i2c_msg msgs[2] = {
		{ client->addr, 0,        1, &reg  },
		{ client->addr, I2C_M_RD, 1, data },
	};

	ret = i2c_transfer(client->adapter, msgs, 2);
	if (ret != 2) {
		dev_err(&client->dev, "%s: read error (%d)\n", __func__, ret);
		return -EIO;
	}

	return 0;
}

static int gt208l_write_reg(struct i2c_client *client, u8 reg, u8 data)
{
	int ret;
	char buf[2] = { reg, data };

	ret = i2c_master_send(client, buf, sizeof(buf));
	if (ret != sizeof(buf)) {
		dev_err(&client->dev,
			"%s: err=%d addr=%02x, data=%02x\n",
			__func__, ret, buf[0], buf[1]);
		return -EIO;
	}

	return 0;
}

static ssize_t gt208l_proc_read(char* page, char** start, off_t off, int count, int* eof, void* data)
{
	int i;
	u8 val;
	u8 reg;	
	u8 bits[18];

	reg = *((u8*)data);
	regread(reg, val);

	for(i = 0; i < 4; i++) {
		if(val & (1<<(7-i)))
			bits[2*i] = '1';
		else
			bits[2*i] = '0';

		bits[2*i+1] = ' ';
	}
	bits[8] = ' ';
	for(i = 4; i < 8; i++) {
		if(val & (1<<(7-i)))
			bits[2*i+1] = '1';
		else
			bits[2*i+1] = '0';

		bits[2*i+2] = ' ';
	}
	bits[17] = '\0';

	*eof = 1;
	return sprintf(page, "%02x :  %s\n", val,bits);
}  

static ssize_t gt208l_proc_write(struct file* filp, const char __user *buff, unsigned long len, void* data)
{
	char sbuf[2+1];
	int val;
	u8 reg;	

	if (len > 3 || copy_from_user(sbuf, buff, len))
		return -EFAULT;

	sbuf[len] = 0;
	sscanf(sbuf, "%02x", &val);

	reg = *((u8*)data);
	regwrite(reg, (u8)val);

	return len;
}  

static void gt208l_hwinit(void)
{
	u8 sensitivity = 0x06;

	//regwrite(0x03, 0xFF); //default: enable all touch channels
	//regwrite(0x04, 0x55); //default: enable multi touch mode
	regwrite(0x05, 0xFF);	//enable all pwm channel, light on all leds
	regwrite(0x3A, 0x25);	//enable sensitivity control
	regwrite(0x3B, 0x30);	//pwm enable
	//regwrite(0x3D, 0x1A);	//

	regwrite(0x40, sensitivity);
	regwrite(0x41, sensitivity);
	regwrite(0x42, sensitivity);
	regwrite(0x43, sensitivity);
	regwrite(0x44, sensitivity);
	regwrite(0x45, sensitivity);
	regwrite(0x46, sensitivity);
	regwrite(0x47, sensitivity);
	
	regwrite(0x48, 0xFF); //set all leds highest brightness
	regwrite(0x49, 0xFF);
	regwrite(0x4A, 0xFF);
	regwrite(0x4B, 0xFF);
}

static const u8 gt208l_regs[] = 
{
	0x02,
	0x03,
	0x04,
	0x05,
	0x2A,
	0x39,
	0x3A,
	0x3B,
	0x3D,
	0x40,
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4F,
};

static void gt208l_create_proc(void)
{  
    struct proc_dir_entry* entry;  
	char path[64];
    int i;

	proc_mkdir("GT208L", NULL);

    for(i = 0; i < ARRAY_SIZE(gt208l_regs); i++) {
    	sprintf(path, "GT208L/%02x", gt208l_regs[i]);
    	entry = create_proc_entry(path, 0, NULL);
    	if(entry) { 
	        entry->read_proc = gt208l_proc_read;  
	        entry->write_proc = gt208l_proc_write;
	        entry->data = (void *)(&gt208l_regs[i]);
		}
    }
}  

static void gt208l_remove_proc(void) 
{
    int i;
	char path[64];

    for(i = 0; i < ARRAY_SIZE(gt208l_regs); i++) {
    	sprintf(path, "GT208L/%02x", gt208l_regs[i]);
    	remove_proc_entry(path, NULL);
    }
}

static int gt208l_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct gt208l *gt208l;

	gt208l = kzalloc(sizeof(struct gt208l), GFP_KERNEL);
	if (!gt208l)
		return -ENOMEM;

	gt208l->client = client;
	i2c_set_clientdata(client, gt208l);

	return 0;
}

static int __devexit gt208l_remove(struct i2c_client *client)
{
	struct gt208l *gt208l = i2c_get_clientdata(client);

	i2c_set_clientdata(client, NULL);
	kfree(gt208l);
	return 0;
}

static const struct i2c_device_id gt208l_id[] = {
	{ "GT208L", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, gt208l_id);

static struct i2c_driver gt208l_driver = {
	.driver = {
		.name = "GT208L",
		.owner = THIS_MODULE,
	},
	.probe = gt208l_probe,
	.remove = __devexit_p(gt208l_remove),
	.id_table = gt208l_id,
};

#define GT208L_ADDR			0xB8	//CTRL pin: OPEN

static struct i2c_board_info boardinfo = {
	I2C_BOARD_INFO("GT208L", GT208L_ADDR>>1),
	.platform_data = NULL,
};

static const unsigned short addr_list[] = {
	GT208L_ADDR>>1, I2C_CLIENT_END,
};

#define GT208L_ADAPTER_NUMBER	1
int gt208l_init(void)
{
	struct i2c_adapter *adap;

	i2c_add_driver(&gt208l_driver);

	adap = i2c_get_adapter(GT208L_ADAPTER_NUMBER);
	if (!adap) {
		printk(KERN_ERR "cannot get adapter %d\n", GT208L_ADAPTER_NUMBER);
		i2c_del_driver(&gt208l_driver);
		return -1;
	}
	
	gt208l_client = i2c_new_probed_device(adap, &boardinfo, addr_list);
	if (!gt208l_client) {
		printk(KERN_ERR "cannot detect new i2c device\n");
		i2c_del_driver(&gt208l_driver);
		return -1;
	}

	gt208l_hwinit();
	gt208l_create_proc();

	return 0;
}

void gt208l_exit(void)
{
	if (gt208l_client) {
		i2c_unregister_device(gt208l_client);
		kfree(gt208l_client);
	}
	i2c_del_driver(&gt208l_driver);
	gt208l_remove_proc();
}

u8 gt208l_read_key(void)
{
	u8 data;

	regread(0x2A, data);

	return data;
}

void gt280l_set_led(u8 i, int on)
{
	u8 reg;
	u8 val;

	printk("set led %d : %d\n", i, on);

	if(i > 6)
		return;

	i += 1;

	reg = 0x48 + i / 2;
	regread(reg, val);
	if(on) {
		val |= ((i % 2) ? 0xf0 : 0x0f);
	} else {
		val &= ((i % 2) ? 0x0f : 0xf0);
	}
	printk("write %02x= %02x\n", reg, val);
	regwrite(reg, val);
}
