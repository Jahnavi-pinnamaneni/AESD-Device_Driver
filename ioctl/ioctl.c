#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#include "ioctl_test.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamnaneni");
MODULE_DESCRIPTION("A simple example for ioctl in a LKM");

static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "aesd_ioctl"
#define DRIVER_CLASS "MyModuleClass"

static int driver_open(struct inode *device_file, struct file *instance)
{
	printk("ioctl: open was called!\n");
	return 0;
}

static int driver_close(struct inode *device_file, struct file *instance)
{
	printk("ioctl: close was called\n");
	return 0;
}

int32_t answer = 42;

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	struct mystruct test;
	int i = 0;
	switch(cmd)
	{
		case WR_VALUE:
			if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer)))
				printk("ioctl: Error copying data from the user\n");
			else
				printk("ioctl: Update the variable answer to %d\n", answer);
			break;
		
		case RD_VALUE:
			if(copy_to_user((int32_t *)arg, &answer, sizeof(answer)))
				printk("ioctl: Error copying data to user\n");
			else
				printk("ioctl: answer was copied to userspace\n");
			break;
		case GREETING:
			if(copy_from_user(&test, (struct mystruct *) arg, sizeof(test)))
				printk("ioctl: Error copying data fromt the user\n");
			else
			{
				for(i =0; i< test.repeat; i++)
					printk("ioclt: Greetings to %s\n", test.name);
			}
			break;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.unlocked_ioctl = my_ioctl
};

static int __init ModuleInit(void)
{
	printk("Hello Kernel\n");
	
	if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0)
	{
		printk("ioctl: Device number could not be allocated\n");
		return -1;
	}
	
	printk("ioctl: Device number: Major: %d, Minor: %d was registered\n", my_device_nr >> 20, my_device_nr && 0xFFFFF);
	
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
	{
		printk("ioctl: Device class can not be created \n");
		goto ClassError;
	}
	
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME)== NULL)
	{
		printk("ioctl: Can not create device file\n");
		goto FileError;
	}
	
	cdev_init(&my_device, &fops);
	
	if(cdev_add(&my_device, my_device_nr, 1) == -1)
	{
		printk("ioctl: Registering of device to kernel failed\n");
		goto AddError;
	}
	
	return 0;
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;

}

static void __exit ModuleExit(void)
{
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

	
	
