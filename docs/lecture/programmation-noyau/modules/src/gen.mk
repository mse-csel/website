# Part executed when called from kernel build system
ifneq ($(KERNELRELEASE),)
obj-m += mymodule.o          ## name of the generated module
mymodule-objs := skeleton.o  ## list of objets needed by that module
CFLAGS_skeleton := -DDEBUG   ## to enable printing of debugging messages

# Part executed when called from standard make in the module source directory
else
CPU := arm64
KDIR := /workspace/buildroot/output/build/linux-5.8.6/
TOOLS := /workspace/buildroot/output/host/usr/bin/aarch64-linux-gnu-
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(CPU) CROSS_COMPILE=$(TOOLS) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) 

endif