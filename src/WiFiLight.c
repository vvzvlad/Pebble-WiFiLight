#include "pebble.h"

float timer_delay=1; /* Создаем переменную для с временем для таймера */
Window *window; /* Создаем указатель на окно */
TextLayer *text_layer;  /* создаем  указатель на текстовый слой */
enum {
  QUOTE_KEY_COMMAND = 0x0,
  QUOTE_KEY_CHANNEL = 0x1,
};

static void send_msg(int channel, int command) {
  Tuplet command_tuple = TupletInteger(QUOTE_KEY_COMMAND, command);
  Tuplet channel_tuple = TupletInteger(QUOTE_KEY_CHANNEL, channel);
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    return;
  }

  dict_write_tuplet(iter, &command_tuple);
  dict_write_tuplet(iter, &channel_tuple);
  dict_write_end(iter);

  app_message_outbox_send();
}

void config_text_layer(int16_t x,int16_t y,int16_t h,int16_t w, const char *font_key)  /* для исключения дублирования кода, создали функцию, которая занимается инициализаций и настройкой текстового массива*/
{
    text_layer = text_layer_create(GRect(x, y, h, w)); /* создаем массив, указываем размер и координаты */
    text_layer_set_text_color(text_layer, GColorWhite);  /* устанавливаем цвет текста */
    text_layer_set_background_color(text_layer, GColorClear);  /* устанавливаем цвет фона */
    text_layer_set_font(text_layer, fonts_get_system_font(font_key)); /* устанавливаем шрифт */
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter); /* устанавливаем выравнивание по центру */
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));  /* подключаем текстовый слой к основному в качестве дочернего */
}
static void in_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Received!");
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}


void up_click(ClickRecognizerRef recognizer, void *context)  /* функция, срабатывающая при клике на кнопки */
{
    send_msg(1,1);
}

void down_click(ClickRecognizerRef recognizer, void *context)  /* функция, срабатывающая при клике на кнопки */
{
    send_msg(1,0);
}

void WindowsClickConfigProvider(void *context)  /* функция, внутри которой должны находиться подписки на кнопки */
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click); /* при нажатии на верхнюю кнопку запустить click */
    window_single_click_subscribe(BUTTON_ID_SELECT, up_click); 
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click); 
}

int main(void)
{
    window = window_create();  /* Инициализируем окно */
    window_set_background_color(window, GColorBlack); /* устанавливаем фоновый цвет */
    window_set_fullscreen(window, true); /* включаем полноэкранность */
    window_stack_push(window, true);  /* открываем окно */

    app_message_register_inbox_received(in_received_handler);
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_failed(out_failed_handler);
    app_message_open(64, 64);

    config_text_layer(0, 3, 144, 168, FONT_KEY_GOTHIC_24);
    window_set_click_config_provider(window, WindowsClickConfigProvider); /* определяем функцию, в которой будут находиться подписки на кнопки */
    text_layer_set_text(text_layer, "WiFiLight");  /* показываем сообщение при запуске программы */
    app_event_loop();  /* ждем событий */
    text_layer_destroy(text_layer); /* уничтожаем объекты, освобождаем ресурсы */
    window_destroy(window);  /* уничтожаем объекты, освобождаем ресурсы */
}
