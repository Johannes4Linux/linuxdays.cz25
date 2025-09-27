/* Required Header */
#include <linux/i2c.h>
#include <linux/kobject.h>

/* Global variables to keep driver simple and avoid dynamic memory allocation */
struct kobject * my_kobj;
/* TODO: Add global pointer variable to i2c device to keep driver simple */

/* Otpional TODO: Implement show function to read out the state of the button */
static ssize_t rgb_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	return sprintf(buffer, "Hello world!\n");
}

/* TODO: Implement store function to control the RGB LED */
static ssize_t rgb_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	printk("I got %s\n", buffer);
	return count;
}

/* TODO: Rename attribute for led */
static struct kobj_attribute mydev_attr = __ATTR(my_attr, 0660, rgb_show, rgb_store);

/* Optional TODO: Add attribute for the button */

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

	/* TODO: Rename the kobject and therefore the /sys/ folder */
	my_kobj = kobject_create_and_add("my_kobj", my_kobj);
	if (!my_kobj) {
		printk("Error creating kernel object\n");
		return -ENOMEM;
	}

	status = sysfs_create_file(my_kobj, &mydev_attr.attr);
	if (status) {
		printk("Error creating /sys/my_kobj/my_attr\n");
		goto rm_kobj;
	}

	/* Optional TODO: Create file for the button */
	return 0;

rm_kobj:
	kobject_put(my_kobj);
	return status;
}

/* function will be called when a compatible device is removed from the system */
static void my_remove(struct i2c_client *client)
{
	sysfs_remove_file(my_kobj, &mydev_attr.attr);
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
