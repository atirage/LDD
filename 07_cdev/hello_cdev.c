#include <linux/module.h>
#include <linux/module.h>
#include <linux/fs.h>

static int my_init(void);
static void my_exit(void);
static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o);
static ssize_t my_write(struct file *f, const char __user *u, size_t l, loff_t *o);
static int my_open(struct inode *node, struct file *fp);
static int my_release(struct inode *node, struct file *fp);

static int major;
static char text[64];

static struct file_operations fops = {
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_release
};

static int __init my_init(void) {
	major = register_chrdev(0, "hello_cdev", &fops);
	if (major < 0) {
		printk(KERN_ERR "hello_cdev - Error registering char device!\n");
		return major;
	}
	printk(KERN_INFO "hello_cdev - Major device number: %d\n", major);
	return 0;
}

static void __exit my_exit(void){
	unregister_chrdev(major, "hello_cdev");
}

static ssize_t my_read(struct file *filp, char __user *ub, size_t len, loff_t *offs) {
	int not_copied, to_copy = len;
	if ((*offs + len) > sizeof(text)) {
		to_copy = sizeof(text) - *offs;
	}
	not_copied = copy_to_user(ub, text + *offs, to_copy);
	*offs += to_copy - not_copied;
	printk(KERN_INFO "hello_cdev - Read %d bytes.", to_copy - not_copied);
	return to_copy - not_copied;
}

static ssize_t my_write(struct file *filp, const char __user *ub, size_t len, loff_t *offs) {
	int not_written, to_write = len;
	if ((*offs + len) > sizeof(text)) {
		to_write = sizeof(text) - *offs;
	}
	not_written = copy_from_user(text + *offs, ub, to_write);
	*offs += to_write - not_written;
	printk(KERN_INFO "hello_cdev - Written %d bytes.", to_write - not_written);
	return to_write - not_written;
}

static int my_open(struct inode *inode, struct file *fp) {
	printk(KERN_INFO "hello_cdev - Major: %d, Minor: %d\n", imajor(inode), iminor(inode));
	printk(KERN_INFO "hello_cdev - Flags: 0x%x\n", fp->f_flags);
	return 0;
}

static int my_release(struct inode *inode, struct file *fp) {
	printk(KERN_INFO "hello_cdev - file closed\n");
	return 0;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("Character Device Driver Demo");

