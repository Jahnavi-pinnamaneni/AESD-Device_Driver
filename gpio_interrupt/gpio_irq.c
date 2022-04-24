#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamaneni");
MODULE_DESCRIPTION("Kernel Module to demonstrate GPIO interrupts");

unsigned int irq_number;

static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	printk("gpio_irq: Interrupt was triggered and ISR was called\n");
	return (irq_handler_t) IRQ_HANDLED;
}

static int __init ModuleInit(void)
{
	printk("gpio_irq: Loading module");
	
	if(gpio_request(17, "rpi-gpio-17"))
	{
		printk("Error: Cannot allocate GPIO 17\n");
		return -1;
	}
	
	if(gpio_direction_input(17))
	{
		printk("Error: Cannot set GPIO 17 to input\n");
		gpio_free(17);
		return -1;
	}
	
	irq_number = gpio_to_irq(17);
	
	if(request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL)!= 0)
	{
		printk("Error: Cannot request interupt no. %d\n",irq_number);
		gpio_free(17);
		return -1;
	}
	
	printk("Done\n");
	printk("GPIO 17 is mapped to IRQ No: %d\n", irq_number);
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("gpio_irq: Unloading module...");
	free_irq(irq_number, NULL);
	gpio_free(17);
}

module_init(ModuleInit);
module_exit(ModuleExit);
