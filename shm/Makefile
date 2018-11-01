%.o : %.c
	$(CC) --sysroot=$(SYSROOT) -c $< 

all: shmread shmwrite

shmread: shmread.o shmutil.o
	$(CC) -B/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysroots/x86_64-spirentsdk-linux/usr/lib --sysroot=$(SYSROOT) -L/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysrootsx86_64-spirent-linux/lib64 -L/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysrootsx86_64-spirent-linux/lib64 -o $@ $? 	 

shmwrite: shmwrite.o  shmutil.o
	$(CC) -B/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysroots/x86_64-spirentsdk-linux/usr/lib --sysroot=$(SYSROOT) -L/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysrootsx86_64-spirent-linux/lib64 -L/export/crosstools/yocto-1.5/423/changeling-appsec-vm/sysrootsx86_64-spirent-linux/lib64 -o $@ $? 	 


.PHONY: clean
clean:
	rm -f shmread shmwrite
	rm -f *.o
