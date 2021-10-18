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

#define File_Size_Max   500*1024   //必须为4096的整数倍
#define SRAM_BUF_ADDR   0x80600000

static int do_upb(cmd_tbl_t *cmd, int flag, int argc, char * const argv[])
{
    char   cmd_str[100];
    int ret = 0;
	printf("====Start to uptate the tftp====\n");
    printf("====TFTP get u-boot-sunxi-with-spl.bin from %s====\n",getenv("serverip"));

    sprintf(cmd_str , "mw.b 0x%x 0xff 0x%x" , SRAM_BUF_ADDR , File_Size_Max);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "tftp 0x%x  %s" , SRAM_BUF_ADDR , getenv("upb_file"));
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

    sprintf(cmd_str , "sf erase 0x00 0x%x" , File_Size_Max);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "sf write 0x%x  0x%x 0x%x" , SRAM_BUF_ADDR , 0x00 , File_Size_Max);
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
