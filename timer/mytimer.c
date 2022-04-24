/*
@brief: This program toggles an LED with a period of one second.
@Author: Jahnavi Pinnamaneni
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamaneni");
MODULE_DESCRIPTION("Kernel module to demonstrate timers");

int flag = 0;

static struct timer_list my_timer;

/*
@brief: This is interrupt handler.
	The functions toggles the LED and calls itself recursively
*/
void timer_callback(struct timer_list * data)
{
	if(flag)
	{
		flag = 0;
		gpio_set_value(4,0);
	}
	else
	{
		flag = 1;
		gpio_set_value(4,1);
	}
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
}

/*
@brief: Module Initialization
	This function initializes the GPIO pin and timer.
*/
static int __init ModuleInit(void)
{
	printk("Hello Kernel\n");
	
	if(gpio_request(4, "rpi-gpio-4"))
	{
		printk("Can not allocate GPIO 4\n");
		return -1;
	}
	
	if(gpio_direction_output(4, 0))
	{
		printk("Can not set GPIO 4 to output\n");
		gpio_free(4);
		return -1;
	}
	
	gpio_set_value(4,1);
	
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
	flag = 1;
	return 0;
}

/*
@brief: Unloading the module
	This function frees the gpio and deletes the timer
*/
static void __exit ModuleExit(void)
{
	printk("GoodBye\n");
	gpio_free(4);
	del_timer(&my_timer);
}

module_init(ModuleInit);
module_exit(ModuleExit);
