#include "window.h"
#include <pebble.h>

static Window *s_window;
static GBitmap *s_bitmap_panda;
static GFont s_font_bree;
static BitmapLayer *s_bitmaplayer_panda;
static TextLayer *s_textlayer_time;

static void initialize_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, PBL_IF_COLOR_ELSE(GColorLightGray, GColorBlack));
#if PBL_SDK_2
  window_set_fullscreen(s_window, true);
#endif

  Layer *root_layer = window_get_root_layer(s_window);

  s_bitmap_panda = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PANDA);
#if PBL_PLATFORM_CHALK
  s_font_bree = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BREE_EXTRABOLD_24));
#else
  s_font_bree = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BREE_EXTRABOLD_28));
#endif

  const GRect panda_rect = PBL_IF_ROUND_ELSE((GRect(0, 0, 180, 180)), (GRect(0, 0, 144, 168)));
  s_bitmaplayer_panda = bitmap_layer_create(panda_rect);
  bitmap_layer_set_alignment(s_bitmaplayer_panda, GAlignBottom);
  bitmap_layer_set_bitmap(s_bitmaplayer_panda, s_bitmap_panda);
  layer_add_child(root_layer, (Layer *)s_bitmaplayer_panda);

  const GRect time_rect = PBL_IF_ROUND_ELSE((GRect(0, 17, 180, 28)), (GRect(0, 2, 144, 28)));
  s_textlayer_time = text_layer_create(time_rect);
  text_layer_set_background_color(s_textlayer_time, GColorClear);
  text_layer_set_text(s_textlayer_time, "9:41 AM");
  text_layer_set_text_alignment(s_textlayer_time, GTextAlignmentCenter);

  text_layer_set_font(s_textlayer_time, s_font_bree);
  layer_add_child(root_layer, (Layer *)s_textlayer_time);
}

static void handle_window_unload(Window *window) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_panda);
  text_layer_destroy(s_textlayer_time);
  gbitmap_destroy(s_bitmap_panda);
  fonts_unload_custom_font(s_font_bree);
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
  initialize_ui();
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
