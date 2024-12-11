
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
#include <string.h>
#include <time.h>

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
static void main_screen(void);
static void back_event_handler(lv_event_t * e);
static void settings_menu(lv_event_t * e);
static void anim_x_cb(void * var, int32_t v);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_display_t  * disp1;
static lv_display_t  * disp1;
static lv_obj_t * scr1;
static lv_obj_t * list1;
static lv_obj_t * list_playlist;
static lv_obj_t * list_artists;
static lv_obj_t * list_albums;
static lv_obj_t * window;
char * new_playlist_name;
static lv_obj_t * playlist_name_msg;
static lv_obj_t * time_obj;
static lv_obj_t * timer_bar;
static lv_obj_t * play_btn;
static uint32_t time_act = 0;

int monitor_hor_res = 800;
int monitor_ver_res = 480;

const char *playlist_names[] = {"Workout", "Relax", "Study", "Sleep", "Driving", "Party"}; /* need to have as linked list of some kind for user modification */
int number_playlists = 6;  /* Update if more playlists are added to playlist_names[]  -  need to have as linked list of some kind for user modification*/

const char *artist_name[] = {"The Beatles", "Chevelle", "Catholic Lofi", "Nickelback", "Underoath", "Our Last Night"}; /* need to have as linked list of some kind for user modification */
int number_artists = 6; /* Make Variable and related to linked list */

const char *album_name[] = {"Abbey Road", "Revolver", "This Sacred Heart", "Silver Side Up", "They're Only Chasing Safety", "The Ghosts Among Us", "Vena Sera"}; /* need to have as linked list of some kind for user modification */
int number_albums = 7; /* Make Variable and related to linked list */

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define MAX_PLAYLISTS 50

/**********************
 *      TYPEDEFS
 **********************/
typedef struct playlist_t{
  char *name;
  struct playlist_t * next;
}playlist_t;

typedef struct tracker{
  lv_obj_t * btn;
  lv_obj_t * window;
}tracker;

typedef struct node_list_t {
    struct playlist_t * head;
    struct playlist_t * tail;  // optional
    int size;
} node_list_t;

/**********************
 *      VARIABLES
 **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

static int tick_thread(void * data);
static void memory_monitor(lv_timer_t * param);
static void main_screen(void);
static void second_screen(void);
static void sidebar_list(void);
static void home_list(void);
static void browse_list(void);
static void search_list(void);
static void podcast_list(void);
static void window_playlist(void);
static void window_albums(void);
static void window_artists(void);
static void window_now_playing(void);

static void event_handler(lv_event_t * e);
static void event_handler_home(lv_event_t * e);
static void event_handler_browse(lv_event_t * e);
static void event_handler_search(lv_event_t * e);
static void event_handler_playlist(lv_event_t * e);
static void event_handler_podcast(lv_event_t * e);
static void event_handler_artists(lv_event_t * e);
static void event_handler_albums(lv_event_t * e);

static void event_handler_new_playlist(lv_event_t * e);
static void event_handler_create_playlist(lv_event_t * e);
static void event_handler_cancel_playlist(lv_event_t * e);
static void name_new_playlist(void);
static void event_handler_del_playlist(lv_event_t * e);

static void event_handler_choose_playlist(lv_event_t * e);
static void event_handler_choose_artist(lv_event_t * e);
static void event_handler_choose_album(lv_event_t * e);

static void play_event_click_cb(lv_event_t * e);
static void play_music(void);
static void pause_music(void);

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
  hal_init(800, 480);

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

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_scr_act()); /*Create an image object for the cursor */
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
  lv_screen_load(lv_scr_act());
  lv_obj_set_style_bg_color(lv_scr_act(),lv_color_hex(0x181818), LV_PART_MAIN);

  lv_obj_t * label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Home Screen");
  lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  sidebar_list();
  home_list();
  podcast_list();
  window_playlist();

  return;
}

static void sidebar_list(void)
{
  lv_screen_load(lv_scr_act());
  /*Create a list*/
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list1 = lv_list_create(lv_scr_act());
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
    window_playlist();
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
    window_artists();
  }
}
static void event_handler_albums(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_delete(window);
    window_albums();
  }
}

/* Large Menus */
static void home_list(void)
{
  lv_anim_t a;
  lv_anim_init(&a);

  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Home");

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0x115588));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);
  lv_style_set_border_width(&style_btn, 2);
  lv_style_set_border_color(&style_btn, lv_color_black());

  /* Window content */
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

  timer_bar = lv_slider_create(cont);
  lv_obj_align(timer_bar, LV_ALIGN_BOTTOM_MID, 0, -30);
  lv_slider_set_value(timer_bar, 0, LV_ANIM_ON);
  lv_obj_set_style_anim_duration(timer_bar, 100, 0);

  lv_anim_set_var(&a, timer_bar);

  time_obj = lv_label_create(cont);
  lv_obj_set_style_text_color(time_obj, lv_color_hex(0x8a86b8), 0);
  lv_label_set_text(time_obj, "0:00");
  lv_obj_align(time_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
  //lv_obj_add_event_cb(time_obj, del_counter_timer_cb, LV_EVENT_DELETE, NULL);

  play_btn = lv_btn_create(cont);
  lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 30, 0);
  lv_obj_set_size(play_btn, 20, 15);
  // lv_obj_add_event_cb(play_btn, play_event_click_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CHECKABLE);
}

static void play_event_click_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
      play_music();

    }
    else {
      pause_music();
    }
}

static void play_music()
{
  while (lv_obj_has_state(play_btn, LV_STATE_CHECKED)){
    lv_slider_set_value(timer_bar, time_act, LV_ANIM_ON);
    time_act++;
  }
}

static void pause_music()
{
  lv_obj_remove_state(play_btn, LV_STATE_CHECKED);
}

static void browse_list(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Browse");
}

static void search_list(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Search");
}

static void podcast_list(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Podcasts");
}

/* Playlists */
static void window_playlist(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Playlists");

  lv_obj_t * cont = lv_win_get_content(window);

  lv_obj_t * new_btn = lv_btn_create(cont);
  lv_obj_set_size(new_btn, 120, lv_pct(10));
  lv_obj_align(new_btn, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_add_event_cb(new_btn, event_handler_new_playlist, LV_EVENT_ALL, NULL);

  lv_obj_t * del_btn = lv_btn_create(cont);
  lv_obj_set_size(del_btn, 120, lv_pct(10));
  lv_obj_align(del_btn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_add_event_cb(del_btn, event_handler_del_playlist, LV_EVENT_ALL, NULL);

  lv_obj_t * new_label = lv_label_create(new_btn);
  lv_obj_t * del_label = lv_label_create(del_btn);
  lv_label_set_text(new_label, "New Playlist");
  lv_label_set_text(del_label, "Delete Playlist");
  lv_obj_center(new_label);
  lv_obj_center(del_label);

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list_playlist = lv_list_create(cont);
  lv_obj_add_style(list_playlist, &style, LV_STATE_DEFAULT);
  lv_obj_set_size(list_playlist, lv_pct(100), lv_pct(85));
  lv_obj_align(list_playlist,LV_ALIGN_BOTTOM_MID, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * btn, * btn_playlist, * btn_podcast;

  for (int i = 0; i < number_playlists; i++){
    btn = lv_list_add_button(list_playlist, LV_SYMBOL_LIST, playlist_names[i]);
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
/* Code to add a playlist */
static void event_handler_new_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED){
    name_new_playlist();
  }
}

static void name_new_playlist(void)
{
  playlist_name_msg = lv_msgbox_create(NULL);
  lv_obj_set_size(playlist_name_msg, lv_pct(50), lv_pct(35));
  lv_msgbox_add_title(playlist_name_msg, "New Playlist");
  lv_msgbox_add_close_button(playlist_name_msg);

  lv_obj_t * cont = lv_msgbox_get_content(playlist_name_msg);

  lv_obj_t * new_playlist_name_textarea = lv_textarea_create(cont);
  lv_textarea_set_password_mode(new_playlist_name_textarea, false);
  lv_obj_set_size(new_playlist_name_textarea, lv_pct(100), lv_pct(20));
  lv_textarea_set_one_line(new_playlist_name_textarea, true);

  lv_obj_t * create_playlist_btn = lv_msgbox_add_footer_button(playlist_name_msg, "Create");
  lv_obj_align(create_playlist_btn, LV_ALIGN_BOTTOM_LEFT, -20, 20);
  lv_obj_add_event_cb(create_playlist_btn, event_handler_create_playlist, LV_EVENT_ALL, new_playlist_name_textarea);

  lv_obj_t * cancel_playlist_btn = lv_msgbox_add_footer_button(playlist_name_msg, "Cancel");
  lv_obj_add_event_cb(cancel_playlist_btn, event_handler_cancel_playlist, LV_EVENT_ALL, NULL);
}

static void event_handler_create_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  lv_obj_t * ta = lv_event_get_user_data(e);
  if (code == LV_EVENT_CLICKED){
    const char * new_playlist_name = lv_textarea_get_text(ta);
    lv_obj_t * btn;
    btn = lv_list_add_button(list_playlist, LV_SYMBOL_LIST, new_playlist_name);
    lv_msgbox_close(playlist_name_msg);
  }
}

static void event_handler_cancel_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED){
    lv_msgbox_close(playlist_name_msg);
  }
}

/* Code to delete a playlist - Needs done, probably requires linked list or something similar I don't know about*/
static void event_handler_del_playlist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  LV_UNUSED(obj);
}

/* Artists */
static void window_artists(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Artists");

  lv_obj_t * cont = lv_win_get_content(window);

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list_artists = lv_list_create(cont);
  lv_obj_add_style(list_artists, &style, LV_STATE_DEFAULT);
  lv_obj_set_size(list_artists, lv_pct(100), lv_pct(100));
  lv_obj_align(list_artists,LV_ALIGN_BOTTOM_MID, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * btn, * btn_playlist, * btn_podcast;

  for (int i = 0; i < number_artists; i++){
    btn = lv_list_add_button(list_artists, LV_SYMBOL_DRIVE, artist_name[i]);
    lv_obj_add_event_cb(btn, event_handler_choose_artist, LV_EVENT_ALL, NULL);
  }
}
/* Need to go to artist still */
static void event_handler_choose_artist(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  LV_UNUSED(obj);
}

/* Albums */
static void window_albums(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Albums");

  lv_obj_t * cont = lv_win_get_content(window);

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 0);

  list_albums = lv_list_create(cont);
  lv_obj_add_style(list_albums, &style, LV_STATE_DEFAULT);
  lv_obj_set_size(list_albums, lv_pct(100), lv_pct(100));
  lv_obj_align(list_albums,LV_ALIGN_BOTTOM_MID, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * btn, * btn_playlist, * btn_podcast;

  for (int i = 0; i < number_albums; i++){
    btn = lv_list_add_button(list_albums, LV_SYMBOL_IMAGE, album_name[i]);
    lv_obj_add_event_cb(btn, event_handler_choose_album, LV_EVENT_ALL, NULL);
  }
}
/* Need to go to album still */
static void event_handler_choose_album(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  LV_UNUSED(obj);
}

/*static void window_now_playing(void)
{
  window = lv_win_create(lv_scr_act());
  lv_obj_set_size(window, lv_pct(80), monitor_ver_res);
  lv_obj_align(window,LV_ALIGN_RIGHT_MID, 0, 0);
  lv_win_add_title(window, "Now Playing");

  lv_obj_t * cont = lv_win_get_content(window);

  lv_obj_t * timer_bar = lv_slider_create(cont);
}*/

/**** END MOVE TO AND FROM VS CODE PROJECT ****/
