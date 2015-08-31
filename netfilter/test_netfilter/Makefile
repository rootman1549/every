obj-m := testnetfilter.o
PWD := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build/

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	rm -rf *.o
	rm -rf *.ko
	rm -rf *.mod.c
	rm -rf Module.symvers
	rm -rf modules.order
