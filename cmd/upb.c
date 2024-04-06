/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <stdio_dev.h>

#define FILE_SIZE_MAX   (500*1024)   //uboot 固件文件最大大小
#define SRAM_BUF_ADDR   (0x80600000)
#define DEFAULT_UBOOT_FILE "u-boot-sunxi-with-spl.bin"

static int do_upb(cmd_tbl_t *cmd, int flag, int argc, char * const argv[])
{
    char cmd_str[100];
    char UPB_FILE[200];
    int ret = 0;
	printf("====Start to uptate the tftp, you can setenv upb_file to set the uboot file name====\n");

    //如果upb文件名未设置，则使用默认值DEFAULT_UBOOT_FILE
    if(getenv("upb_file") == NULL)
    {
        strcpy(UPB_FILE, DEFAULT_UBOOT_FILE);
        printf("[WARN] pUPB_FILE is NULL, use the default uboot name:%s\n",UPB_FILE);
    }
    else
    {
        strcpy(UPB_FILE, getenv("upb_file"));
    }

    printf("====TFTP get uboot file:%s from %s====\n",UPB_FILE ,getenv("serverip"));

    sprintf(cmd_str , "mw.b 0x%x 0xff 0x%x" , SRAM_BUF_ADDR , FILE_SIZE_MAX);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "tftp 0x%x %s" , SRAM_BUF_ADDR ,UPB_FILE);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "sf probe");
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    //erase the uboot
    sprintf(cmd_str , "sf erase 0x00 0x%x" , FILE_SIZE_MAX);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    //erase the uboot env args
    sprintf(cmd_str , "sf erase 0x%x 0x%x" , CONFIG_ENV_OFFSET , CONFIG_ENV_SIZE);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "sf write 0x%x  0x%x 0x%x" , SRAM_BUF_ADDR , 0x00 , FILE_SIZE_MAX);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "reset");
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    return 0;
}


/***************************************************/

U_BOOT_CMD(
	upb,	1 ,	1,	do_upb,
    "update the uboot",
	"run this cmd to update the uboot to SPI Flash"
);
