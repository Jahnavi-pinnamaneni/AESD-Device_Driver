ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($(CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif

all: test
.DEFAULT_GOAL := test

test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES)

clean:
	rm -f *.o test
