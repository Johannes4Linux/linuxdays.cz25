/* Required Header */
#include <linux/i2c.h>

/* List all compatible devices */
static struct i2c_device_id my_ids[] = {
	/* TODO: Add a compatible device*/
	{} /* empty element signalizes end of list*/
};
MODULE_DEVICE_TABLE(i2c, my_ids);

/* function will be called, when a compatible device is added to the system. */
static int my_probe(struct i2c_client *client)
{
	/* TODO: Add code here */
	return 0;
}

/* function will be called when a compatible device is removed from the system */
static void my_remove(struct i2c_client *client)
{
	/* TODO: Add code here */
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
