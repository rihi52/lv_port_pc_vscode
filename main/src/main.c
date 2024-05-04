
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/
lv_obj_t * scr1;
lv_obj_t * scr2;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void btn_event_cb(lv_event_t * e);
static void btn_event_cb2(lv_event_t * e);
static void new_screen();
static void new_screen1();

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  lv_display_t * disp = hal_init(480, 272);

  new_screen();

  // lv_demo_widgets();

  /*lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Home Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Next Screen");
  lv_obj_center(btn_label);*/

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
    usleep(5 * 1000);
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  scr1 = lv_obj_create(NULL);
  lv_screen_load(scr1);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}

static void btn_event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target(e);
  lv_display_t * disp1 = lv_obj_get_display(btn);
  if (code == LV_EVENT_CLICKED){
    lv_obj_clean(scr1);
    new_screen1();
  }
  return;
}

static void btn_event_cb2(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target(e);
  lv_display_t * disp1 = lv_obj_get_display(btn);
  if (code == LV_EVENT_CLICKED){
    lv_obj_clean(scr2);
    new_screen();
  }
  return;
}

static void new_screen(void)
{
  lv_screen_load(scr1);
  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Home Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Next Screen");
  lv_obj_center(btn_label);

  return;
}

static void new_screen1(void)
{
  scr2 = lv_obj_create(NULL);
  lv_screen_load(scr2);

  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "New Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, btn_event_cb2, LV_EVENT_ALL, NULL);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Previous Screen");
  lv_obj_center(btn_label);

  return;
}
