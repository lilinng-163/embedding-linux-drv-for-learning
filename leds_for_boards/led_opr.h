/*** 
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-23 14:18:48
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-23 14:19:36
 * @FilePath: \driver\leds\led_opr.h
 * @Description: LED操作接口头文件
 * @
 * @Copyright (c) 2026 by wangyixiang@163.com, All Rights Reserved. 
 */
#ifndef __LED_OPR_H__
#define __LED_OPR_H__

typedef struct led_opeartion
{
    int (*init) (int which);
    int (*ctrl) (int which, char status);
} led_opeartions;

led_opeartions* get_board_led_opr(void);

#endif