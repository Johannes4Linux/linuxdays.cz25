/* Required Header */
#include <linux/i2c.h>
#include <linux/kobject.h>

/* Global variables to keep driver simple and avoid dynamic memory allocation */
struct kobject * my_kobj;
struct i2c_client *dev_ptr;

static ssize_t button_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	int status = i2c_smbus_read_byte(dev_ptr);
	return sprintf(buffer, "Button is %spressed\n", status & 1 ? "not " : "");
}

static struct kobj_attribute button_attr = __ATTR(button, 0440, button_show, NULL);

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	u8 val = 0xff;
	int i;

	if (count < 3)
		return -EINVAL;


	for (i=0; i<3; i++) {
		if (buffer[i] == '1')
			val &= ~(1 << (i + 1));
	}

	i2c_smbus_write_byte(dev_ptr, val);

	return count;
}
static struct kobj_attribute led_attr = __ATTR(led, 0220, NULL, led_store);

/* List all compatible devices */
static struct i2c_device_id my_ids[] = {
	{"rgb_brd"},
	{} /* empty element signalizes end of list*/
};
MODULE_DEVICE_TABLE(i2c, my_ids);

/* function will be called, when a compatible device is added to the system. */
static int my_probe(struct i2c_client *client)
{
	int status;

	dev_ptr = client;

	/* TODO: Rename the kobject and therefore the /sys/ folder */
	my_kobj = kobject_create_and_add("rgb_brd", my_kobj);
	if (!my_kobj) {
		printk("Error creating kernel object\n");
		return -ENOMEM;
	}

	status = sysfs_create_file(my_kobj, &led_attr.attr);
	if (status) {
		printk("Error creating /sys/rgb_brd/led\n");
		goto rm_kobj;
	}

	status = sysfs_create_file(my_kobj, &button_attr.attr);
	if (status) {
		printk("Error creating /sys/rgb_brd/button\n");
		goto rm_attr_kobj;
	}

	return 0;

rm_attr_kobj:
	sysfs_remove_file(my_kobj, &led_attr.attr);
rm_kobj:
	kobject_put(my_kobj);
	return status;
}

/* function will be called when a compatible device is removed from the system */
static void my_remove(struct i2c_client *client)
{
	i2c_smbus_write_byte(client, 0xff);
	sysfs_remove_file(my_kobj, &button_attr.attr);
	sysfs_remove_file(my_kobj, &led_attr.attr);
	/* Optional TODO: Remove file for the button */
	kobject_put(my_kobj);
}

/* Bundle list of compatible devices, probe- and remove function into a driver struct */
static struct i2c_driver my_driver = {
	.probe = my_probe,
	.remove = my_remove,
	.id_table = my_ids,
	.driver = {
		.name = "my-i2c-driver",
	}
};

/* Register the driver */
module_i2c_driver(my_driver);

/* Informatione about the driver  */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(""); /* TODO: Add your name here */
MODULE_DESCRIPTION(""); /* TODO: Add a small description for the driver */
