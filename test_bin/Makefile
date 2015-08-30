obj-m := hello.o

KDIR := /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)

all :
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean :
	rm *.ko

.PHONY : all clean
