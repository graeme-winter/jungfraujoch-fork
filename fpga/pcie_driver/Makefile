obj-m += jfjoch.o
cflags-m=-std=c99

jfjoch-objs := jfjoch_drv.o jfjoch_ioctl.o jfjoch_memory.o jfjoch_pcie_setup.o jfjoch_function.o \
               jfjoch_miscdev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean