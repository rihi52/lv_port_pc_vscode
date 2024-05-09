
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <string.h>
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
const char *playlist_name[] = {"Workout", "Relax", "Study", "Sleep", "Driving", "Party"};

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
int monitor_hor_res = 800, monitor_ver_res = 480;
static lv_display_t  * disp1;
static lv_obj_t * scr1;
static lv_obj_t * list1;
static lv_obj_t * list2;
lv_obj_t * window;
static char *text = "Hello";

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void main_screen(void);
static void second_screen(void);
static void sidebar_list(void);
static void home_list(void);
static void browse_list(void);
static void search_list(void);
static void podcast_list(void);
static void playlist_list(void);
static void albums_list(void);
static void artists_list(void);
static void event_handler(lv_event_t * e);
static void event_handler_home(lv_event_t * e);
static void event_handler_browse(lv_event_t * e);
static void event_handler_search(lv_event_t * e);
static void event_handler_playlist(lv_event_t * e);
static void event_handler_choose_playlist(lv_event_t * e);
static void event_handler_podcast(lv_event_t * e);
static void event_handler_artists(lv_event_t * e);
static void event_handler_albums(lv_event_t * e);

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
  lv_display_t * disp = hal_init(monitor_hor_res, monitor_ver_res);

  main_screen();

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

static void main_screen(void)
{
  lv_screen_load(scr1);
  //lv_obj_set_style_bg_color(lv_screen_active(),lv_color_hex(0x181818), LV_PART_MAIN);

  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Home Screen");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  sidebar_list();
  home_list();
  // podcast_list();
  // playlist_list();

  return;
}

static void sidebar_list(void)
{
  /*Create a list*/
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list1 = lv_list_create(lv_screen_active());
  lv_obj_add_style(list1, &style, LV_STATE_DEFAULT);
  lv_obj_set_size(list1, lv_pct(20), monitor_ver_res);
  lv_obj_align(list1,LV_ALIGN_LEFT_MID, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * btn, * btn_playlist, * btn_podcast;
  btn = lv_list_add_button(list1, LV_SYMBOL_HOME, "Home");
  lv_obj_add_event_cb(btn, event_handler_home, LV_EVENT_ALL, NULL);

  btn = lv_list_add_button(list1, LV_SYMBOL_EYE_OPEN, "Search");
  lv_obj_add_event_cb(btn, event_handler_search, LV_EVENT_ALL, NULL);

  btn = lv_list_add_button(list1, LV_SYMBOL_DIRECTORY, "Browse");
  lv_obj_add_event_cb(btn, event_handler_browse, LV_EVENT_ALL, NULL);

  lv_list_add_text(list1, "User");
  btn = lv_list_add_button(list1, LV_SYMBOL_LIST, "Playlists");
  lv_obj_add_event_cb(btn, event_handler_playlist, LV_EVENT_ALL, NULL);

  btn = lv_list_add_button(list1, LV_SYMBOL_LIST, "Podcasts");
  lv_obj_add_event_cb(btn, event_handler_podcast, LV_EVENT_ALL, NULL);

  btn = lv_list_add_button(list1, LV_SYMBOL_LIST, "Artists");
  lv_obj_add_event_cb(btn, event_handler_artists, LV_EVENT_ALL, NULL);

  btn = lv_list_add_button(list1, LV_SYMBOL_LIST, "Albums");
  lv_obj_add_event_cb(btn, event_handler_albums, LV_EVENT_ALL, NULL);
}

/* Sidebar List Button Event Handlers */
static void event_handler_home(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    home_list();
  }
}
static void event_handler_browse(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    browse_list();
  }
}
static void event_handler_search(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    search_list();
  }
}

static void event_handler_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    playlist_list();
  }
}
static void event_handler_podcast(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    podcast_list();
  }
}
static void event_handler_artists(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    artists_list();
  }
}
static void event_handler_albums(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    albums_list();
  }
}

/* Large Menus */
static void home_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Home");

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0x115588));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);
  lv_style_set_border_width(&style_btn, 2);
  lv_style_set_border_color(&style_btn, lv_color_black());

  // Window content
  lv_obj_t * cont = lv_win_get_content(window);

  lv_obj_t * btn;
  btn = lv_btn_create(cont);
  lv_obj_add_style(btn, &style_btn, 0);
  lv_obj_align(btn,LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_size(btn, 150, 150);

  lv_obj_t * label = lv_label_create(btn);
  lv_obj_t * label1 = lv_label_create(btn);
  lv_label_set_text(label, "New Releases\n\n");
  lv_label_set_text(label1, LV_SYMBOL_AUDIO);
  lv_obj_center(label);
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 20);

  btn = lv_btn_create(cont);
  lv_obj_add_style(btn, &style_btn, 0);
  lv_obj_align(btn,LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_size(btn, 150, 150);

  label = lv_label_create(btn);
  label1 = lv_label_create(btn);
  lv_label_set_text(label, "Recently Played\n\n");
  lv_label_set_text(label1, LV_SYMBOL_AUDIO);
  lv_obj_center(label);
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 20);

  btn = lv_btn_create(cont);
  lv_obj_add_style(btn, &style_btn, 0);
  lv_obj_align(btn,LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_size(btn, 150, 150);

  label = lv_label_create(btn);
  label1 = lv_label_create(btn);
  lv_label_set_text(label, "Your Updates\n\n");
  lv_label_set_text(label1, LV_SYMBOL_AUDIO);
  lv_obj_center(label);
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 20);

}
static void browse_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Browse");
}
static void search_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Search");
}
static void podcast_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Podcasts");
}
static void playlist_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Playlists");

  lv_obj_t * cont = lv_win_get_content(window);

  /*Create a list*/
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list2 = lv_list_create(cont);
  lv_obj_add_style(list2, &style, LV_STATE_DEFAULT);
  lv_obj_set_size(list2, lv_pct(100), lv_pct(100));
  lv_obj_align(list2,LV_ALIGN_BOTTOM_MID, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * btn, * btn_playlist, * btn_podcast;

  int n = 6;
  for (int i = 0; i < n; i++){
    btn = lv_list_add_button(list2, LV_SYMBOL_LIST, playlist_name[i]);
    lv_obj_add_event_cb(btn, event_handler_choose_playlist, LV_EVENT_ALL, NULL);
  }

}
/* Need to go to playlist still */
static void event_handler_choose_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  LV_UNUSED(obj);
}
static void artists_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Artists");
}
static void albums_list(void)
{
  window = lv_win_create(lv_screen_active());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Albums");
}

/**** END MOVE TO AND FROM VS CODE PROJECT ****/
