CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
TARGET1 = md
obj-m := $(TARGET1).o

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

load:
	sudo insmod md.ko
remove:
	sudo rmmod md.ko

ls:
	lsmod | grep md
dmesg:
	dmesg > dmesg_log.txt
log:
	cat /proc/analyzer >log.txt
tracer:
	gcc main.c -o app

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm *.ko *.symvers *.mod


# sudo insmod md.ko
# lsmod | grep md
# sudo dmesg
# sudo rmmod md.ko
