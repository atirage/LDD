#include <linux/module.h>
#include <linux/module.h>

static int my_init(void);
static void my_exit(void);

static int __init my_init(void) {
	printk(KERN_INFO "hello - Hello Kernel!\n");
	return 0;
}

static void __exit my_exit(void){
	printk(KERN_INFO "hello - Bye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("Kernel Module Demo");

