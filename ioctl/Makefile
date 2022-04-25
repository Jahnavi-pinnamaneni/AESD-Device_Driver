obj-m += ioctl.o

ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($(CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif

all: mod test

.PHONY: mod

mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES)
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf test
