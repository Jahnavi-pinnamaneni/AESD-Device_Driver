/*
@brief: This code demonstrates GPIO interrupt in Linux device driver. 
		GPIO pin 17 is used as input pin to trigger an interrupt.

@Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#define GPIO_INT 17

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamaneni");
MODULE_DESCRIPTION("Kernel Module to demonstrate GPIO interrupts");

unsigned int irq_number;

/*
@brief: interrupt handler for GPIO
@desc: this handler notifies that the interrupt was triggered.
*/
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	printk("gpio_irq: Interrupt was triggered and ISR was called\n");
	return (irq_handler_t) IRQ_HANDLED;
}

/*
@brief: module initialization
@desc: The GPIO 17 is requested from the kernel, the direction is set to input and assigned to an irq
*/
static int __init ModuleInit(void)
{
	printk("gpio_irq: Loading module");
	
	if(gpio_request(GPIO_INT, "rpi-gpio-17"))
	{
		printk("Error: Cannot allocate GPIO 17\n");
		return -1;
	}
	
	if(gpio_direction_input(17))
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
	return 0;
}

/*
@brief: unloading of module
@desc: the irq number and gpio is freed
*/
static void __exit ModuleExit(void)
{
	printk("gpio_irq: Unloading module...");
	free_irq(irq_number, NULL);
	gpio_free(GPIO_INT);
}

module_init(ModuleInit);
module_exit(ModuleExit);
