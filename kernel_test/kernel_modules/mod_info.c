#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/list.h>
#include<linux/cpumask.h>
#include <linux/ctype.h>

#define mydebug(format, ...) printk(KERN_DEBUG "[%s:%s:%d]:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)
#define myprintf(format, ...) printk(KERN_ERR format, ## __VA_ARGS__)

static int int_var = 0;
static const char *str_var = "default";
static int int_array[6];
int narr;

module_param(int_var, int, 0644);
MODULE_PARM_DESC(int_var, "A integer variable");

module_param(str_var, charp, 0644);
MODULE_PARM_DESC(str_var, "A string variable");

module_param_array(int_array, int, &narr, 0644);
MODULE_PARM_DESC(int_array, "A integer array");

static bool is_ascii_str(char *str) {
	bool rtn = true;

	while(*str) {
		if(!isprint(*str)) {
			rtn = false;
			break;
		}
		str++;
	}

	return rtn;
}

static int __init mod_info_init(void)
{
	struct module *mod, *relate;
	int i;

	mydebug("name:%s, state:%d\n", THIS_MODULE->name,THIS_MODULE->state);

	printk("int_var %d(0x%x).\n", int_var, int_var);
	printk("str_var %s.\n", str_var);

	for(i = 0; i < narr; i ++){
		printk("int_array[%d] = %d\n", i, int_array[i]);
	}

	list_for_each_entry(mod,THIS_MODULE->list.prev, list) {
		printk("=================================================================================\n");
		printk("name:%s state:%d refcnt:%lu\n", mod->name, mod->state, module_refcount(mod));

		//What modules depend on me?
		printk("used by:");
		if(!list_empty(&mod->source_list)) { 
			list_for_each_entry(relate, &mod->source_list, source_list) {
				if(is_ascii_str(relate->name)) {
					printk("%s ",relate->name);
				} else {
					printk("* ");
				}
			}
			printk("\n");
		} else {
			printk("NULL\n");
		}

		//What modules do I depend on?
		printk("depend on:");
		if(!list_empty(&mod->target_list)) { 
			list_for_each_entry(relate, &mod->target_list, target_list) {
				if(is_ascii_str(relate->name)) {
					printk("%s ",relate->name);
				} else {
					printk("* ");
				}
			}
			printk("\n");
		} else {
			printk("NULL\n");
		}

		if(strcmp(str_var, mod->name) == 0) {
			module_put(mod);
			mod->state = 0;
		}
		printk("name:%s state:%d refcnt:%lu\n",mod->name,mod->state,module_refcount(mod));
	}
	return 0;
}

static void __exit mod_info_exit(void)
{
	mydebug("name:%s state:%d\n",THIS_MODULE->name,THIS_MODULE->state);
}

module_init(mod_info_init);
module_exit(mod_info_exit);

MODULE_AUTHOR("xiaofei");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module info");

//insmod mod_info.ko int_var=100 str_var=hello int_array=100,200,300,400
