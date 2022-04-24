#Instructions to Build and run this module

1. make  
2. sudo insmod sysfs_test.ko  
3. sudo su  
4. echo "This is a test to my kernel module" > /sys/kernel/my_class/test  
5. dmesg | tail -n 10  
6. cat /sys/kernel/my_class/test | head -n 1  
**Output expected:** When we echo a string, we observe in the dmesg that the strinf has been written and when we cat on the file, we observe the string that we passed in the code.  

6. sudo rmmod sysfs_test  
7. exit  

![Output](Images/sysfs_output.jpg)
