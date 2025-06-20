#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *my_thread;

static int thread_fn(void *data) {
    int count = 0;
    printk(KERN_INFO "kthread_example: Thread started\n");

    while (!kthread_should_stop()) {
        printk(KERN_INFO "kthread_example: Running... %d\n", count++);
        ssleep(1);  // Sleep for 1 second
    }

    printk(KERN_INFO "kthread_example: Thread stopping\n");
    return 0;
}

static int __init kthread_example_init(void) {
    printk(KERN_INFO "kthread_example: Module loaded\n");

    my_thread = kthread_run(thread_fn, NULL, "my_kthread");
    if (IS_ERR(my_thread)) {
        printk(KERN_ERR "kthread_example: Failed to create thread\n");
        return PTR_ERR(my_thread);
    }

    return 0;
}

static void __exit kthread_example_exit(void) {
    if (my_thread) {
        kthread_stop(my_thread);
        printk(KERN_INFO "kthread_example: Thread stopped\n");
    }

    printk(KERN_INFO "kthread_example: Module unloaded\n");
}

module_init(kthread_example_init);
module_exit(kthread_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("A simple kernel thread example");
