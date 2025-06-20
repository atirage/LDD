#include <linux/module.h>
#include <linux/module.h>
#include <linux/fs.h>

static int my_init(void);
static void my_exit(void);
static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o);

static int major;

static struct file_operations fops = {.read = my_read};

static int __init my_init(void) {
	major = register_chrdev(0, "hello_cdev", &fops);
	if (major < 0) {
		printk("hello_cdev - Error registering char device!\n");
		return major;
	}
	printk("hello_cdev - Major device number: %d\n", major);
	return 0;
}

static void __exit my_exit(void){
	unregister_chrdev(major, "hello_cdev");
}

static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o) {
	printk("hello_cdev - Read is called\n");
	return 0;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("Character Device Driver Demo");

