/*** 
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-23 14:18:48
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-23 21:15:57
 * @FilePath: \driver\leds_for_boards\led_opr.h
 * @Description: LED操作接口头文件
 * @
 * @Copyright (c) 2026 by wangyixiang@163.com, All Rights Reserved. 
 */
#ifndef __LED_OPR_H__
#define __LED_OPR_H__

typedef struct led_opeartion
{
    int num;
    int (*init) (int which);
    int (*ctrl) (int which, char status);
    void (*exit)(void);
} led_opeartions;

led_opeartions* get_board_led_opr(void);

#endif