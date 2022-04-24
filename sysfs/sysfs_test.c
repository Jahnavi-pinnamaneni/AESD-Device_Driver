#include <linux/module.h>
#include <linux//init.h>
#include <linux/kobject.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jahnavi Pinnamaneni");
MODULE_DESCRIPTION("Kernel module to demostrate sysfs");

static struct kobject *test_kobj;

static ssize_t test_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	return sprintf(buffer, "You have read from /sys/kernel/%s/%s\n", kobj->name, attr->attr.name);
}

static ssize_t test_store(struct kobject *kobj, struct kobj_attribute * attr, const char *buffer, size_t count)
{
	printk("sysfs: You wrote '%s' to /sys/kernel/%s%s\n", buffer, kobj->name, attr->attr.name);
	return count;
}

static struct kobj_attribute test_attr = __ATTR(test, 0660, test_show, test_store);

static int __init my_init(void)
{
		printk("sysfs: Creating /sys/kernel/my_class/test\n");
		
		test_kobj = kobject_create_and_add("my_class", kernel_kobj);
		if(!test_kobj)
		{
			printk("sysfs: Error creating /sys/kernel/my_class/test\n");
			return -ENOMEM;
		}
		
		if(sysfs_create_file(test_kobj, &test_attr.attr))
		{
			printk("sysfs: Error creating /sys/kernel/my_class/test\n");
			kobject_put(test_kobj);
			return -ENOMEM;
		}
		
		return 0;
}

static void __exit my_exit(void)
{
	printk("sysfs: Deleting /sys/kernel/my_class/test\n");
	sysfs_remove_file(test_kobj, &test_attr.attr);
	kobject_put(test_kobj);
}

module_init(my_init);
module_exit(my_exit);
