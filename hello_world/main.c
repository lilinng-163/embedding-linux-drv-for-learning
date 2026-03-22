/*
 * @Author: lilinng-163 wangyixiang051129@163.com
 * @Date: 2026-03-22 11:55:04
 * @LastEditors: lilinng-163 wangyixiang051129@163.com
 * @LastEditTime: 2026-03-22 13:29:26
 * @FilePath: \hello_world\main.c
 * @Description: 
 * 
 * Copyright (c) 2026 by lilinng-163 wangyixiang051129@163.com, All Rights Reserved. 
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char** argv)
{
    int fd;
    char buf[1024];
    int len;
    memset(buf,0,sizeof(buf));

    if(argc < 2)
    {
        printf("Usage: %s -w <string>\n", argv[0]);
        printf("       %s -r\n", argv[0]);
        return -1;
    }
    else
    {
        fd = open("/dev/hello", O_RDWR);

        if(fd == -1)
        {
            perror("open");
            return -1;
        }

        if((strcmp(argv[1], "-w") == 0) && (argc == 3))
        {
            len = strlen(argv[2] + 1);
            len = len < 1024 ? len : 1024;
            write(fd, argv[2], len);
        }
        else
        {
            len = read(fd,buf,1024);
            buf[1024] = '\0';
            printf("APP read :%s\n",buf);
        }
        close(fd);
    }
}