/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-23 14:32:09
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-23 14:39:10
 * @FilePath: \driver\leds\led_interface.c
 * @Description: LED接口实现文件
 * 
 * Copyright (c) 2026 by wangyixiang@163.com, All Rights Reserved. 
 */
#include <linux/gfp.h>
#include "led_opr.h"
static int led_init (int which)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
}
static int led_ctrl (int which, char status)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
}
static led_opeartions led_opr = 
{
    .init = led_init,
    .ctrl = led_ctrl,
};
led_opeartions* get_board_led_opr(void)
{
    return &led_opr;
}