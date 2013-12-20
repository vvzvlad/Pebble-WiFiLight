#include "pebble.h"


static Window *window;
static char* UUID="1687136871";
static AppSync sync;
static uint8_t sync_buffer[64];
TextLayer *text_layer;  /* создаем  указатель на текстовый слой */

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case 0:
      text_layer_set_text(text_layer, new_tuple->value->cstring);
      break;

  }
}

void send() 
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_cstring(iter, 1, UUID);
  app_message_outbox_send();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "send UUID!");
  app_timer_register(5000, send, NULL); 
}


int main(void)
{
  window = window_create();
  window_set_fullscreen(window, true);
  window_stack_push(window, true);
  srand(time(NULL));

  app_message_open(64,64);
  snprintf(UUID, sizeof(UUID), "%d", rand());
  app_timer_register(1000, send, NULL); 


   text_layer = text_layer_create(GRect(0, 0, 144, 168)); /* создаем текстовый массив, указываем размер и координаты */
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18)); /* устанавливаем шрифт */
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter); /* устанавливаем выравнивание по центру */
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));  

  Tuplet initial_values[] = {
    TupletCString(0, ""),
  };

  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);


  app_event_loop();
  window_destroy(window);
}
