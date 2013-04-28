#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

// 144 × 168

#define MY_UUID { 0x0F, 0x33, 0x6C, 0x35, 0x05, 0x18, 0x49, 0x20, 0x9D, 0x00, 0xA9, 0xE5, 0x92, 0xF3, 0x6A, 0x09 }
PBL_APP_INFO(MY_UUID,
             "Test WatchFace", "mikeL",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
BmpContainer bg_container;
TextLayer timeLayer;

void updateTime() {
  PblTm currentTime;

  get_time(&currentTime);

  static char timeText[] = "00:00:00";

  string_format_time(timeText, sizeof(timeText), "%T", &currentTime);

  text_layer_set_text(&timeLayer, timeText);
}

void updateTimeLayer(TextLayer *text_layer, GContext *ctx) {
  updateTime();
}

void SetBackground() {
  bmp_init_container(RESOURCE_ID_IMG_DOOM, &bg_container);
  layer_add_child(&window.layer, &bg_container.layer.layer);
}

void AddTimeLayer() {
  text_layer_init(&timeLayer, GRect(0, 138, 144 /* width */, 30 /* height */));

  text_layer_set_text_color(&timeLayer, GColorWhite);
  text_layer_set_background_color(&timeLayer, GColorClear);
  text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  updateTime();
  layer_add_child(&window.layer, &timeLayer.layer);
}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  layer_mark_dirty(&(timeLayer.layer));
}

void window_load(Window *window) {
  SetBackground();
  
  AddTimeLayer();
  
  timeLayer.layer.update_proc = (LayerUpdateProc)updateTimeLayer;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  resource_init_current_app(&TEST_APP_RESOURCES);

  window_init(&window, "Watch Face");
  window_set_background_color(&window, GColorBlack);
  window_stack_push(&window, true /* Animated */);
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;
  bmp_deinit_container(&bg_container);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
