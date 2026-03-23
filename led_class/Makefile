# 配置交叉编译工具链
# vim ~/.bashrc 查看并配置

KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules
# 这里的make是进入了内核源码目录，执行了内核的Makefile文件，M=`pwd`表示当前目录是模块的目录
	$(CROSS_COMPILE)gcc -o ledtest main.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -rf ledtest

# 指定编译ko文件的依赖
led-y := drv_led.o led_board.o
obj-m += led.o
