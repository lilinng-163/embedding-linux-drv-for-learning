/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-22 19:04:48
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-23 17:38:13
 * @FilePath: \driver\leds\drv_led.c
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
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>


#include "led_opr.h"

#define LED_NUM 2   //LED数量

#define MIN(a, b) (a < b ? a : b)

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
static struct class* led_class;
static led_opeartions* led_opr;

static int led_open(struct inode *inode, struct file *file)
{
    //根据次设备号初始化配置硬件
    int minor = iminor(inode);   //获取次设备号

    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

    led_opr->init(minor);

    return 0;
}
static ssize_t led_read (struct file * file, char __user *buf, size_t size, loff_t *offset)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}
static ssize_t led_write(struct file *file, const char __user *buf,
			                size_t size, loff_t *ppos)
{
    char status;
    //操作硬件
    //判断次设备号，根据次设备号控制不同的LED
    struct inode* inode = file_inode(file);
    int minor = iminor(inode) & 0x0F;   //获取次设备号

    copy_from_user(&status, buf, MIN(1024,size));   //从用户空间获取数据，控制LED状态

    led_opr->ctrl(minor,status);

    return 1;
}
//结构体
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
};
/*入口函数*/
static int __init led_init(void)
{
    int i;
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    //注册file_operations结构体
    led_major = register_chrdev(led_major, "led", &fops);

    //创建设备节点
    led_class = class_create(THIS_MODULE, "my_led");
    for(i = 0;i < LED_NUM; i++)
    {
        //设备号由主设备号和次设备号组成，次设备号从0开始递增
        device_create(led_class, NULL, MKDEV(led_major, i), NULL, "my_led%d",i);    //  /dev/my_led0  /dev/my_led1
    }
    //获取LED操作接口
    led_opr = get_board_led_opr();
	return 0;
}
/*出口函数*/
static void __exit led_exit(void)
{
    int i;
    //销毁设备节点
    for(i = 0;i < LED_NUM; i++)
    {
        device_destroy(led_class, MKDEV(led_major, i));
    }
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