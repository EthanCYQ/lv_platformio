/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "lvgl.h"
#include "app_hal.h"

#include "demo.h"
#include "lv_fs_if/lv_fs_if.h"

int main(void)
{
	lv_init();

  lv_fs_if_init();

	hal_setup();

	demo_create();

	hal_loop();
}
