#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/device.h>

/* probe functions proto */
static int dt_probe(struct i2c_client *client);
static void dt_remove(struct i2c_client *client);
/* sysfs functions proto */
static ssize_t my_attr_show(struct device *dev,
                            struct device_attribute *attr,
                            char *buf);
static ssize_t my_attr_store(struct device *dev,
                             struct device_attribute *attr,
                             const char *buf, size_t count);

static DEVICE_ATTR(my_attr, 0664, my_attr_show, my_attr_store);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight, myphat",
	}, 
	{/* sentinel */}
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct i2c_device_id my_phat[] = {
	{"my_phat", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, my_phat);

static struct i2c_driver my_phat_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.id_table = my_phat,
	.driver = {
		.name = "my_phat",
		.of_match_table = my_driver_ids,
	},
};

static int dt_probe(struct i2c_client *client) {
	int ret = 0;

    printk(KERN_INFO "dt_i2c - Probing!\n");
    if(client->addr != 0x2c) {
        printk(KERN_ERR "dt_i2c - Wrong I2C address");
        return -1;
    }

    ret = device_create_file(&client->dev, &dev_attr_my_attr);
    if (ret)
        printk(KERN_ERR "Failed to create sysfs entry! \n");

    return ret;
}

static void dt_remove(struct i2c_client *client) {
	device_remove_file(&client->dev, &dev_attr_my_attr);
    printk(KERN_INFO "dt_i2c - Removing!\n");
}

static ssize_t my_attr_show(struct device *dev,
                            struct device_attribute *attr,
                            char *buf) {
    struct i2c_client *client = to_i2c_client(dev);
    return sprintf(buf, "%X", i2c_smbus_read_byte_data(client, 0xFF));
}

static ssize_t my_attr_store(struct device *dev,
                             struct device_attribute *attr,
                             const char *buf, size_t count) {
    int led_cmd = 0;
    struct i2c_client *client = to_i2c_client(dev);
    
    printk(KERN_INFO "dt_i2c - Received from sysfs:%.*s",(int)count, buf);
    if (kstrtoint(buf, 10, &led_cmd) == 0) {
        (void)i2c_smbus_write_byte_data(client, 0x74, (char)led_cmd);
    }
    return count;
}

module_i2c_driver(my_phat_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AtCs");
MODULE_DESCRIPTION("I2C via DT Kernel Module Demo");

