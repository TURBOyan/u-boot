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

#define FILE_SIZE_MAX   (16*1024*1024)   //digicap 固件文件最大大小
#define SRAM_BUF_ADDR   (0x80600000)
#define DEFAULT_DIGICAP_FILE "digicap.dav"

char UPDATE_FILE[200];

static int get_tftp(char* file_name, int file_size_max, int sram_offset)
{
    int ret = 0;
    char cmd_str[100];
    printf("====TFTP get digicap file:%s from %s====\n",file_name ,getenv("serverip"));

    //erase the tmp sram buf
    sprintf(cmd_str , "mw.b 0x%x 0xff 0x%x" , sram_offset , file_size_max);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "tftp 0x%x %s" , sram_offset ,file_name);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -2;
    }

    return 0;
}

static int get_mmc(char* file_name, int file_size_max, int sram_offset)
{
    int ret = 0;
    char cmd_str[100];

    //erase the tmp sram buf
    sprintf(cmd_str , "mw.b 0x%x 0xff 0x%x" , sram_offset , file_size_max);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -1;
    }

    sprintf(cmd_str , "fatload mmc 0:1 0x%x %s" , sram_offset , file_name);
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

static int do_update(cmd_tbl_t *cmd, int flag, int argc, char * const argv[])
{
    int ret = 0;
    char cmd_str[100];
	printf("====Start to uptate the digicap.dav, you can setenv update_file to set the digicap file name====\n");

    //如果update_file文件名未设置，则使用默认值DEFAULT_DIGICAP_FILE
    if(getenv("update_file") == NULL)
    {
        strcpy(UPDATE_FILE, DEFAULT_DIGICAP_FILE);
        printf("[WARN] pUPB_FILE is NULL, use the default digicap name:%s\n",UPDATE_FILE);
    }
    else
    {
        strcpy(UPDATE_FILE, getenv("upb_file"));
    }

    /************************get digicap from tftp or mmc************************** */

    if(argv[1] != NULL && !strcmp(argv[1], "mmc"))
    {
        printf("====load digicap file from mmc====\n");
        ret = get_mmc(UPDATE_FILE, FILE_SIZE_MAX, SRAM_BUF_ADDR);
        if(ret != 0)
        {
            printf("#### ERROR:  <upb_mmc, ret:%d>  ####\n", ret);
            return -1;
        }
    }
    else
    {
        printf("====load digicap file from tftp====\n");
        ret = get_tftp(UPDATE_FILE, FILE_SIZE_MAX, SRAM_BUF_ADDR);
        if(ret != 0)
        {
            printf("#### ERROR:  <upb_tftp, ret:%d>  ####\n", ret);
            return -1;
        }
    }

    /************************write digicap to spi flash from ram************************** */
    sprintf(cmd_str , "sf probe");
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -3;
    }
    //erase the digicap
    sprintf(cmd_str , "sf erase 0x00 0x%x" , FILE_SIZE_MAX);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -4;
    }

    // //erase the digicap env args
    // sprintf(cmd_str , "sf erase 0x%x 0x%x" , CONFIG_ENV_OFFSET , CONFIG_ENV_SIZE);
    // printf("===cmd====< %s >====start=====\n",cmd_str);
    // ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    // if(ret != 0)
    // {
    //     printf("#### ERROR:  <%s>  ####\n", cmd_str);
    //     printf("#### upb out ####\n");
    //     return -5;
    // }

    sprintf(cmd_str , "sf write 0x%x  0x%x 0x%x" , SRAM_BUF_ADDR , 0x00 , FILE_SIZE_MAX);
    printf("===cmd====< %s >====start=====\n",cmd_str);
    ret = run_command (cmd_str, CMD_FLAG_REPEAT);
    if(ret != 0)
    {
        printf("#### ERROR:  <%s>  ####\n", cmd_str);
        printf("#### upb out ####\n");
        return -6;
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
	update,	3 ,	0,	do_update,
    "update the digicap from mmc or tftp",
	"run this cmd to update the digicap to SPI Flash, [upb mmc] or [upb tftp], default is tftp"
);
