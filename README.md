# AESD-Device_Driver

This repository demonstrates the following features in device drivers:  
1) GPIO Interrupts  
2) Timer  
3) IOCTL  
4) Sysfs  
5) Custom program to combine GPIO interrupt, timer and sysfs  

All the modules are run on Raspbian OS Version_ID 11.  
The Raspbian OS can be setup using the Raspberry Pi Imager software that is available on the website: https://www.raspberrypi.com/software/  
Once is image is burnt onto an SD card, it can be inserted into the Rpi.  
Clone this repository, navigate into the module you wish to implement and execute the instructions mentioned in the respective Readme files.  

Furthermore, this has been extended to work on custom Linux image using Buildroot  
The links to the package and init script are as follows
package: https://github.com/cu-ecen-aeld/final-project-DeekshithPatil/tree/main/base_external/package/extra  
init script: https://github.com/cu-ecen-aeld/final-project-DeekshithPatil/tree/main/base_external/rootfs_overlay/etc/init.d  
