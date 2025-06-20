#include <linux/module.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

/* labels in device tree */
#define GPIO_LED	"gpio21"
#define GPIO_BUTTON	"gpio20"

static struct gpio_desc *led, *button;

static int my_init(void);
static void my_exit(void);
static irqreturn_t gpio_irq_handler(int irq, void *dev_id);

static struct gpio_desc *led, *button;
static int irq_pin;

static int __init my_init(void) {
	int status;

	led = gpiod_get(NULL, GPIO_LED, GPIOD_OUT_LOW);
	if (IS_ERR(led)) {
		printk(KERN_ERR "gpioctrl - Error getting pin!\n");
		return PTR_ERR(led);
	}

	button = gpiod_get(NULL, GPIO_BUTTON, GPIOD_IN);
	if (IS_ERR(button)) {
		printk(KERN_ERR "gpioctrl - Error getting pin!\n");
		return PTR_ERR(button);
	}

	/* set up interrupt - optional */
	irq_pin = gpiod_to_irq(button);
	if(irq_pin < 0) {
		printk(KERN_ERR "Cannot set ISR for %d\n", irq_pin);
		gpiod_put(button);
		return irq_pin;
	}
	status = request_irq(irq_pin, gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL);
	if (status != 0) {
		printk(KERN_ERR "Cannot set ISR for %d\n", irq_pin);
		gpiod_put(button);
		return status;
	}

	gpiod_set_value(led, 1);
	printk(KERN_INFO "gpioctrl - Button is %spressed\n", gpiod_get_value(button)?"":"not ");

	return 0;
}

static void __exit my_exit(void){
	gpiod_set_value(led, 0);
	free_irq(irq_pin, NULL);
	gpiod_put(led);
	gpiod_put(button);
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id) {
	printk(KERN_INFO "gpioctrl - GPIO interrupt triggered.");
	return 0;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("GPIO Module Demo without device tree");

