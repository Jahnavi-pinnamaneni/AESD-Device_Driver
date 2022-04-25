#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl_test.h"

int main()
{
	int answer;
	struct mystruct test = {4, "Jahnavi"};
	int dev = open("/dev/aesd_ioctl", O_WRONLY);
	if(dev == -1)
	{
		printf("Opening was not possible\n");
		return -1;
	}
	
	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is %d\n", answer);
	
	answer = 123;
	
	ioctl(dev, WR_VALUE, &answer);
	ioctl(dev, RD_VALUE, &answer);
	printf("The answe is now %d\n", answer);
	
	ioctl(dev, GREETING, &test);
	
	close(dev);
	return 0;
	
}
