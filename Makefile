obj-m += driver_aleatorios.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.ko *.o *.mod.c modules.order Module.symvers

install:
	sudo insmod driver_aleatorios.ko
	sudo mknod /dev/Num_Aleatorios c $$(awk '/^[0-9]+.*Num_Aleatorios$$/ {print $$1}' /proc/devices) 0
	sudo chmod 777 /dev/Num_Aleatorios

remove:
	sudo rmmod driver_aleatorios
	sudo rm -f /dev/Num_Aleatorios