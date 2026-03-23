/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-23 17:46:40
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-23 22:16:57
 * @FilePath: \driver\leds_for_boards\led_board.c
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
#include <linux/gfp.h>

#include "led_opr.h"

static volatile unsigned int* CCM_CCGR1 = NULL;
static volatile unsigned int* IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = NULL;
static volatile unsigned int* GPIO5_GDIR = NULL;
static volatile unsigned int* GPIO5_DR = NULL;

static int led_init (int which)
{
    unsigned int val;
    if(which == 0)
    {
        if(    !CCM_CCGR1 
            || !IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 
            || !GPIO5_GDIR 
            || !GPIO5_DR)
        {
            CCM_CCGR1 = ioremap(0x020C406C, 4);
            IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
            GPIO5_GDIR = ioremap(0X020AC004, 4);
            GPIO5_DR = ioremap(0x020AC000, 4);
        }
        
        //使能GPIO5时钟
        *CCM_CCGR1 |= (3 << 30);   //设置为11，时钟常开
        //配置GPIO5_3为GPIO功能和输出
        *IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
        val = *IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
        val &= ~0xF;   //清除原来的配置
        val |= 0x5;    //设置为GPIO功能
        *IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;    //设置为GPIO功能
        *GPIO5_GDIR |= (1 << 3);   //设置GPIO5_3为输出
    }
    /*后续有多个led加在此处*/
    return 0;
}
static int led_ctrl (int which, char status)
{
    if(which == 0)
    {
        if(status)  //on
        {
            *GPIO5_DR &= ~(1 << 3);  //点亮LED
            
        }
        else    //off
        {
            *GPIO5_DR |= (1 << 3);  //熄灭LED
        }
    }
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
}
static void led_exit(void)
{   
    iounmap(CCM_CCGR1);
    iounmap(IOMUXC_SWVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
    iounmap(GPIO5_GDIR);
    iounmap(GPIO5_DR);
}
static led_opeartions led_opr = 
{
    .num = 1,
    .init = led_init,
    .ctrl = led_ctrl,
    .exit = led_exit,
};
led_opeartions* get_board_led_opr(void)
{
    return &led_opr;
}