I2C_ADDR = 0x20

obj-m += rgb_brd.o

rgb_brd.ko: rgb_brd.c
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

apply: rgb_brd.ko
	sudo insmod rgb_brd.ko
	echo "rgb_brd $(I2C_ADDR)" | sudo tee /sys/bus/i2c/devices/i2c-1/new_device

remove:
	echo "$(I2C_ADDR)" | sudo tee /sys/bus/i2c/devices/i2c-1/delete_device
	sudo rmmod rgb_brd

	
