#include <pebble.h>
#include "enamel.h"
#include <pebble-events/pebble-events.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window) {
// Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50)
  );

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                       "%H:%M" : "%I:%M", tick_time);

  text_layer_set_background_color(s_time_layer, enamel_get_BACKGROUND_COLOR());
  text_layer_set_text_color(s_time_layer, enamel_get_TEXT_COLOR());
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void enamel_register_settings_received_cb() {
  update_time();
}

static void init(void) {

  // Initialize Enamel to register App Message handlers and restores settings
  enamel_init();

  // Register our callback
  enamel_register_settings_received(enamel_register_settings_received_cb);

  // call pebble-events app_message_open function
  events_app_message_open();

  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload,
  });

  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
}

static void deinit(void) {
  window_destroy(s_main_window);

  // Deinit Enamel to unregister App Message handlers and save settings
  enamel_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
