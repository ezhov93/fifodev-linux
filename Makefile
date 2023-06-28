CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/misc

TARGET = iodev

obj-m += $(TARGET).o 

$(TARGET)-objs := iodev_module.o byte_queue.o iodev_data.o

all: module prog

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

prog: test_iodev sample

test_iodev:	iodev.h test_iodev.c
	gcc test_iodev.c -o test_iodev

sample:	iodev.h sample.c
	gcc sample.c -o sample

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *.symvers *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm -f *.ko test_iodev sample
