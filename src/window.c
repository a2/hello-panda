#include "window.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_panda;
static GFont s_res_font_bree_extrabold_28;
static BitmapLayer *s_bitmaplayer_panda;
static TextLayer *s_textlayer_time;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_image_panda = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PANDA);
  s_res_font_bree_extrabold_28 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BREE_EXTRABOLD_28));
  // s_bitmaplayer_panda
  s_bitmaplayer_panda = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_panda, s_res_image_panda);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_panda);
  
  // s_textlayer_time
  s_textlayer_time = text_layer_create(GRect(0, 2, 144, 28));
  text_layer_set_background_color(s_textlayer_time, GColorClear);
  text_layer_set_text(s_textlayer_time, "9:41 AM");
  text_layer_set_text_alignment(s_textlayer_time, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_time, s_res_font_bree_extrabold_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_time);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_panda);
  text_layer_destroy(s_textlayer_time);
  gbitmap_destroy(s_res_image_panda);
  fonts_unload_custom_font(s_res_font_bree_extrabold_28);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window *window) {
  destroy_ui();
  tick_timer_service_unsubscribe();
}

static char time_buffer[16];
static void update_time_buffer(void) {
  clock_copy_time_string(time_buffer, sizeof(time_buffer)/sizeof(*time_buffer));
  text_layer_set_text(s_textlayer_time, time_buffer);  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time_buffer();
}

void show_window(void) {
  initialise_ui();
  update_time_buffer();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_window(void) {
  window_stack_remove(s_window, true);
}
