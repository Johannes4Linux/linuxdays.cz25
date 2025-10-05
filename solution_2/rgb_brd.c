/* Required Header */
#include <linux/i2c.h>

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

	status = i2c_smbus_write_byte(client, 0xf1);

	if (status) {
		printk("Error writing to I2C device 0x%x\n", client->addr);
		return status;
	}

	status = i2c_smbus_read_byte(client);
	printk("The button is %spressed\n", status & 1 ? "not " : "");

	printk("Hello from I2C device 0x%x\n", client->addr);
	return 0;
}

/* function will be called when a compatible device is removed from the system */
static void my_remove(struct i2c_client *client)
{
	i2c_smbus_write_byte(client, 0xff);
	printk("Bye from I2C device 0x%x\n", client->addr);
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
MODULE_AUTHOR("Johannes Roith <johannes@gnu-linux.rocks>");
MODULE_DESCRIPTION("Driver for the RGB PCB"); 
