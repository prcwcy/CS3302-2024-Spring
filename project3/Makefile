.PHONY: all clean insmod rmmod

obj-m := umem.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod: all
	sudo insmod umem.ko

rmmod:
	sudo rmmod umem
