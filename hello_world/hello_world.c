#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

/*1、确定主设备号*/
static int hello_major = 0; //主设备号为0，表示由系统自动分配,也可以自己指定
static char kernel_buf[1024] = {0}; /*内核缓冲区*/
static struct class *hello_class; /*设备类指针*/
#define MIN(a, b) (a < b ? a : b)

/*3、实现open、read、write、release等函数并填入结构体(先定义函数才能传参)*/

/**
 * @description: 实现read函数，读取设备文件时会调用该函数,下方函数见名知义
 * @param {file *} file         //文件结构体指针，包含了设备文件的信息
 * @param {char __user} *buf    //用户空间的缓冲区地址
 * @param {size_t} size         //用户空间的缓冲区大小
 * @param {loff_t} *offset      //文件偏移量
 * @return {*}
 */
static ssize_t hello_drv_read (struct file * file, char __user *buf, size_t size, loff_t *offset)
{
    int err;
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);
    err = copy_to_user(buf, kernel_buf, MIN(1024,size)); /*从内核空间复制数据到用户空间*/
    return MIN(1024, size);
}
static ssize_t hello_drv_write (struct file * file, const char __user *buf, size_t size, loff_t *offset)
{
    int err;
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);
    err =copy_from_user(kernel_buf, buf, MIN(1024, size)); /*从用户空间复制数据到内核空间*/
    return MIN(1024, size);
}
static int hello_drv_open (struct inode *node, struct file *file)
{
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);
    return 0;
}
static int hello_drv_close (struct inode *node, struct file *file)
{
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/*2、定义自己的file_operations结构体*/
static struct file_operations hello_fops = 
{
    .owner = THIS_MODULE,
    .open = hello_drv_open,
    .read = hello_drv_read,
    .write = hello_drv_write,
    .release = hello_drv_close,
};

/*实现入口函数，当安装驱动程序时去调用入口函数去注册驱动程序*/
static int __init hello_init(void)
{   
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);

    int err;

    //注册字符设备驱动程序，返回主设备号
    hello_major = register_chrdev(hello_major, "hello", &hello_fops); /*注册字符设备驱动程序*/

    //创建类
    hello_class = class_create(THIS_MODULE, "hello_class"); /*创建设备类*/
    err = PTR_ERR(hello_class);
    if (IS_ERR(hello_class)) 
    {
        unregister_chrdev(hello_major, "hello"); /*注销字符设备驱动程序*/
        return -1;
    }
    /*创建设备节点，以/dev/hello访问驱动*/
    //判断返回值(可选)
    device_create(hello_class, NULL, MKDEV(hello_major, 0), NULL, "hello"); 
    return 0;
}
/*实现出口函数，当卸载驱动程序时去调用出口函数*/
static void __exit hello_exit(void)
{   
    printk("%s %s line%d\n",__FILE__, __FUNCTION__, __LINE__);
    
    device_destroy(hello_class, MKDEV(hello_major, 0)); /*销毁设备节点*/

    class_destroy(hello_class); /*销毁设备类*/

    unregister_chrdev(hello_major, "hello"); /*注销字符设备驱动程序*/
}
//指定入口函数和出口函数
module_init(hello_init);
module_exit(hello_exit);
//表明设备遵守GPL协议，允许被GPL协议的模块使用
MODULE_LICENSE("GPL");
/*完善其他：提供设备信息，自动创建设备节点*/