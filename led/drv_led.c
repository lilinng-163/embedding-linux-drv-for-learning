/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-22 19:04:48
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-22 21:47:24
 * @FilePath: \led\drv_led.c
 * @Description: 
 * 
 * Copyright (c) 2026 by wangyixiang@163.com, All Rights Reserved. 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/timer.h>
#include <linux/wait.h>

//寄存器地址,在linux中需要将物理地址映射为虚拟地址
//IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 addr: 0X02290000 + 0X14
static volatile unsigned int* IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

//GPIO5_GDIR addr: 0X020AC000 + 0x4
//配置GPIO5_3为输出
static volatile unsigned int* GPIO5_GDIR;

//GPIO5_DR addr: 0X020AC000 + 0x0
//控制GPIO5_3的输出电平
static volatile unsigned int* GPIO5_DR;

//主设备号
static int led_major = 0;
//类
static struct class *led_class;
//结构体
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .write = led_write,
};
static int led_open(struct inode *inode, struct file *filp)
{
    //配置硬件
    //使能GPIO5,默认使能
    
    //配置GPIO5_3为GPIO功能和输出
    *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xF;   //清除原来的配置
    *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;    //设置为GPIO功能
    *GPIO5_GDIR |= (1 << 3);   //设置GPIO5_3为输出
    return 0;
}
static ssize_t led_write(struct file *filp, const char __user *buf,
			                size_t count, loff_t *ppos)
{
    char val;
    //操作硬件
    //1、映射虚拟地址   map virtual address(可以在入口函数进行映射)

    //2、获取用户空间数据 get data from app
    copy_from_user(&val,buf,1);  //从用户空间复制数据到内核空间
    //3、操作硬件   operate hardware
    if (val == 1)
    {
        *GPIO5_DR &= ~(1 << 3);  //GPIO5_3输出低电平，点亮LED
    }
    else if (val == 0)
    {
        *GPIO5_DR |= (1 << 3);   //GPIO5_3输出高电平，熄灭LED
    }
    return 1;
}
/*入口函数*/
static int __init led_init(void)
{
    printk("%s %s %d\n",__FILE__,FUNCTION__,__LINE__);
    //注册file_operations结构体
    led_major = register_chrdev(led_major, "led", &fops);

    //映射虚拟地址  ioremap
    //一参物理地址，二参映射长度(此处为4bytes)
    IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0X02290000 + 0X14, 4);
    GPIO5_GDIR = ioremap(0X020AC000 + 0x4, 4);
    GPIO5_DR = ioremap(0X020AC000 + 0x0, 4);
    //创建设备节点
    led_class = class_create(THIS_MODULE, "my_led_class");
    device_create(led_class, NULL, MKDEV(led_major, 0), NULL, "my_led");
	return 0;
}
/*出口函数*/
static void __exit led_exit(void)
{
    //销毁设备节点
    device_destroy(led_class, MKDEV(led_major, 0));
    class_destroy(led_class);
	//卸载主设备
    //传入主设备号和名字
    unregister_chrdev(led_major, "led");
}
//注册
module_init(led_init);
module_exit(led_exit);
//协议
MODULE_LICENSE("GPL");