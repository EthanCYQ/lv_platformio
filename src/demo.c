/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"

LV_FONT_DECLARE(OPPOSans_50px);
LV_FONT_DECLARE(OPPOSans_20px);

static void screen_init(lv_obj_t *scr)
{ 
    lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_radius(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_size(scr, 240, 240);
    lv_obj_set_pos(scr, 0, 0);
}

static void virus_screen_init(lv_obj_t *scr)
{
    lv_obj_t *virus_report_img = NULL, *virus_report_label = NULL;

    screen_init(scr);

    virus_report_img = lv_img_create(scr, NULL);
    lv_img_set_src(virus_report_img, "/F:\\esp\\project\\lv_platformio\\src\\APP\\Resource\\Image\\virus_report.bin");
    lv_obj_set_pos(virus_report_img, 70, 50);

    virus_report_label = lv_label_create(scr, NULL);
    lv_label_set_text(virus_report_label, "Coronavirus Reports");
    lv_obj_set_style_local_text_font(virus_report_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &OPPOSans_20px);
    lv_obj_set_style_local_text_color(virus_report_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFF0));
    lv_obj_set_pos(virus_report_label, 17, 160);
}

static void weather_screen_init(lv_obj_t *scr)
{
    lv_obj_t *weather_img = NULL, *weather_label = NULL;

    screen_init(scr);

    weather_img = lv_img_create(scr, NULL);
    lv_img_set_src(weather_img, "/F:\\esp\\project\\lv_platformio\\src\\APP\\Resource\\Image\\weather.bin");
    lv_obj_set_pos(weather_img, 60, 50);

    weather_label = lv_label_create(scr, NULL);
    lv_label_set_text(weather_label, "Weather");
    lv_obj_set_style_local_text_font(weather_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &OPPOSans_20px);
    lv_obj_set_style_local_text_color(weather_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFF0));
    lv_obj_set_pos(weather_label, 80, 160);
}

void demo_create(void)
{
    lv_obj_t *virus_scr = lv_obj_create(lv_scr_act(), NULL);
    virus_screen_init(virus_scr);

    lv_obj_t *weather_scr = lv_obj_create(lv_scr_act(), NULL);
    weather_screen_init(weather_scr);
}