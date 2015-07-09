obj-m := test.o
PWD := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build/

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	rm -rf *.o
	rm -rf *.ko
	rm -rf *.symvers
	rm -rf *.order
	rm -rf *.mod.c
	rm -rf *.mod.o