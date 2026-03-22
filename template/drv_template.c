/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-22 19:04:48
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-22 19:29:03
 * @FilePath: \led\led_drv.c
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
        
    }
    else if (val == 0)
    {
        
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