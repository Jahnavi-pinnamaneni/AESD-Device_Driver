/*
@desc: This program aims at demonstrating GPIO interrupt, timer interrupt and sysfs together
		GPIO Pin 17 is configured as input to trigger the interrupt. GPIO Pin 4 is configured 
		as output to control an LED. 
		When an interrupt is triggered, a timer is set to go off every one second which in turn 
		toggles an LED.
@Author: Jahanvi Pinnamnaneni; japi8358@colorado.edu
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_INT 17
#define LED 4

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamaneni");
MODULE_DESCRIPTION("Kernel Module to demonstrate GPIO interrupt, timer and sysfs");

unsigned int irq_number;
static struct timer_list my_timer;
int flag = 0, time_flag =0;

/*
@brief: Interrupt handler for timer
@desc : This handler, toggles the LED and calls itself recursively
*/
void timer_callback(struct timer_list * data)
{
	if(!time_flag)
	{
		time_flag = 1;
		gpio_set_value(LED,0);
	}
	else{
		time_flag = 0;
		gpio_set_value(LED,1);		
	}	
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
}

/*
@brief: Interrupt handler for GPIO
@desc : This handler, sets the timer to trigger after one second.
*/
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	printk("gpio_irq: Interrupt was triggered and ISR was called\n");
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
	return (irq_handler_t) IRQ_HANDLED;
}

/*
@brief: Module initialization
@desc : This function initializes GPIO 17 to input and GPIO 4 to output. 
		The timer is setup.
*/
static int __init ModuleInit(void)
{
	printk("gpio_irq: Loading module");
	
	if(gpio_request(GPIO_INT, "rpi-gpio-17"))
	{
		printk("Error: Cannot allocate GPIO 17\n");
		return -1;
	}
	
	if(gpio_direction_input(GPIO_INT))
	{
		printk("Error: Cannot set GPIO 17 to input\n");
		gpio_free(GPIO_INT);
		return -1;
	}
	
	irq_number = gpio_to_irq(GPIO_INT);
	
	if(request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL)!= 0)
	{
		printk("Error: Cannot request interupt no. %d\n",irq_number);
		gpio_free(GPIO_INT);
		return -1;
	}
	
	printk("Done\n");
	printk("GPIO 17 is mapped to IRQ No: %d\n", irq_number);
	
	if(gpio_request(LED, "rpi-gpio-4"))
	{
		printk("Can not allocate GPIO 4\n");
		return -1;
	}
	
	if(gpio_direction_output(LED, 0))
	{
		printk("Can not set GPIO 4 to output\n");
		gpio_free(LED);
		return -1;
	}
	
	gpio_set_value(LED,1);
	
	gpio_export(LED, NULL);
	
	timer_setup(&my_timer, timer_callback, 0);
	
	
	return 0;
}

/*
@brief: Unloading Module
@desc: irq number and gpio are freed.
*/
static void __exit ModuleExit(void)
{
	printk("gpio_irq: Unloading module...");
	free_irq(irq_number, NULL);
	gpio_free(GPIO_INT);
	gpio_free(LED);
}

module_init(ModuleInit);
module_exit(ModuleExit);
