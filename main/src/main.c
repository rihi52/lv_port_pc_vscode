
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
static lv_obj_t * ta;
static lv_obj_t * ta2;
static char *text = "Hello";

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void btn_event_cb(lv_event_t * e);
static void btn_event_cb1(lv_event_t * e);
static void btn_event_cb2(lv_event_t * e);
static void main_screen();
static void second_screen();
static void textarea_event_handler(lv_event_t * e);
static void textarea_event_handler1(lv_event_t * e);

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

  main_screen();

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

/**** START MOVE TO AND FROM VS CODE PROJECT ****/

static void btn_event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target(e);
  lv_display_t * disp1 = lv_obj_get_display(btn);
  if (code == LV_EVENT_CLICKED){
    lv_obj_clean(scr1);
    second_screen();
  }
  return;
}

static void btn_event_cb1(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED){
    //text = lv_textarea_get_text(ta);
    lv_strcpy(text, lv_textarea_get_text(ta));
    lv_textarea_add_text(ta2, text);
  }
}

static void btn_event_cb2(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target(e);
  lv_display_t * disp1 = lv_obj_get_display(btn);
  if (code == LV_EVENT_CLICKED){
    lv_obj_clean(scr2);
    main_screen();
  }
  return;
}

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta1 = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_READY){
      lv_strcpy(text, lv_textarea_get_text(ta));
      lv_textarea_add_text(ta2, text);
      LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
    }
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void textarea_event_handler1(lv_event_t * e)
{
    //lv_obj_t * ta1 = lv_event_get_user_data(e);
    //lv_event_code_t code = lv_event_get_code(e);
    //LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void main_screen(void)
{
  lv_screen_load(scr1);
  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Home Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_size(btn, 25, 25);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t * btn1 = lv_button_create(lv_screen_active());
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 50);
  lv_obj_set_size(btn1, 50, 25);
  //lv_obj_add_event_cb(btn1, btn_event_cb1, LV_EVENT_ALL, NULL);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, ">");
  lv_obj_center(btn_label);
  lv_obj_t * btn_label1 = lv_label_create(btn1);
  lv_label_set_text(btn_label1, "Enter");
  lv_obj_center(btn_label1);

  ta = lv_textarea_create(lv_screen_active());
  lv_obj_align(ta, LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_one_line(ta, true);
  lv_textarea_set_placeholder_text(ta, "Placeholder text");
  // lv_textarea_add_text(ta, text);
  lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
  lv_obj_add_state(ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

  ta2 = lv_textarea_create(lv_screen_active());
  lv_obj_align(ta2, LV_ALIGN_CENTER, 0, -100);
  lv_textarea_set_one_line(ta2, true);

  return;
}

static void second_screen(void)
{
  scr2 = lv_obj_create(NULL);
  lv_screen_load(scr2);

  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "New Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_set_size(btn, 25, 25);
  lv_obj_add_event_cb(btn, btn_event_cb2, LV_EVENT_ALL, NULL);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "<");
  lv_obj_center(btn_label);

  lv_obj_t * ta1 = lv_textarea_create(lv_screen_active());
  lv_obj_align(ta1, LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_one_line(ta1, true);
  //lv_textarea_set_placeholder_text(ta1, "Placeholder text");
  lv_textarea_add_text(ta1, text);
  lv_obj_add_event_cb(ta1, textarea_event_handler1, LV_EVENT_READY, ta1);
  lv_obj_add_state(ta1, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

  return;
}

/**** END MOVE TO AND FROM VS CODE PROJECT ****/
