#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static GColor s_background_color;
static GColor s_text_color;
static int s_show_date;

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

  text_layer_set_background_color(s_time_layer, s_background_color);
  text_layer_set_text_color(s_time_layer, s_text_color);
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple *background_color_tuple = dict_find(iterator, MESSAGE_KEY_BACKGROUND_COLOR);
  Tuple *text_color_tuple = dict_find(iterator, MESSAGE_KEY_TEXT_COLOR);

  // If all data is available, use it
  if (background_color_tuple) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Background Color: %d", (int)background_color_tuple->value->int32);
    s_background_color = GColorFromHEX(background_color_tuple->value->int32);
    persist_write_int(MESSAGE_KEY_BACKGROUND_COLOR, background_color_tuple->value->int32);
  }

  if (text_color_tuple) {
    s_text_color = GColorFromHEX(text_color_tuple->value->int32);
    persist_write_int(MESSAGE_KEY_TEXT_COLOR, text_color_tuple->value->int32);
  }

  update_time();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init(void) {

  s_background_color = GColorFromHEX(persist_exists(MESSAGE_KEY_BACKGROUND_COLOR) ?
                                     persist_read_int(MESSAGE_KEY_BACKGROUND_COLOR) :
                                     PBL_IF_COLOR_ELSE(0x000055, 0x000000)
  );

  s_text_color = GColorFromHEX(persist_exists(MESSAGE_KEY_TEXT_COLOR) ?
                               persist_read_int(MESSAGE_KEY_TEXT_COLOR) :
                               0xffffff
  );

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(128, 128);

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
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
