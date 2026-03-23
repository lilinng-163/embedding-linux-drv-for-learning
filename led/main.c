/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-22 22:22:21
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-22 23:13:01
 * @FilePath: \driver\led\main.c
 * @Description: led_app测试
 * 
 * Copyright (c) 2026 by wangyixiang@163.com, All Rights Reserved. 
 */

 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <string.h>

 /*cmd:
 * ledtest /dev/my_led on  
 * ledtest /dev/my_led off
 */

int main(int argc, char** argv)
{
    int fd;
    char status;

    if(argc != 3)
    {
        printf("Usage: %s <device> <on/off>\n", argv[0]);
        return -1;
    }
    //open
    fd = open(argv[1], O_RDWR);
    if(fd == -1)
    {
        perror("open:");
        return -1;
    }

    //write
    if(strcmp(argv[2], "on") == 0)
    {
        status = 1;
    }
    write(fd, &status, 1);  //向设备写入数据，控制LED状态
    
    close(fd);
    return 0;
}