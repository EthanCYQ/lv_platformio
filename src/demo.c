/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"
#include "lv_fs_if/lv_fs_if.h"

void demo_create(void)
{
    lv_fs_if_init();
    lv_fs_file_t f;
    uint32_t f_size = 0;
    if (LV_FS_RES_OK != lv_fs_open(&f, "/F:\\1.txt", LV_FS_MODE_RD)) {
        printf("Open fail\n");
    } else {
        char rd_buf[100];
        uint32_t br = 0;
        lv_fs_read(&f, rd_buf, 20, &br);
        printf("rd_buf: %s, br: %d\n", rd_buf, br);
    }
}