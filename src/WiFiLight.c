#include "pebble.h"

float timer_delay=1; /* Создаем переменную для с временем для таймера */
Window *window; /* Создаем указатель на окно */
TextLayer *text_layer[8];  /* создаем  указатель на текстовый слой */
static const char* messages[] = {
"Плафон 1","1",
"Розетка","2",
"Плафон 2","3",
"Лампа","4",
"5 канал","5",
"6 канал","6",
}; 

int selected=0;
Layer *layer;


#define DEF_IP "162.211.231.73"
#define DEF_PORT "8080"

static void send_msg(char *channel, char *command, char *ip, char *port) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_cstring(iter, 0, command);
    dict_write_cstring(iter, 1, channel);
    dict_write_cstring(iter, 2, ip);
    dict_write_cstring(iter, 3, port);
    app_message_outbox_send();
}

void up_click(ClickRecognizerRef recognizer, void *context)  /* функция, срабатывающая при клике на кнопки */
{
    selected--;
    layer_mark_dirty(layer);
    //send_msg("1","on",DEF_IP,DEF_PORT);
}

void down_click(ClickRecognizerRef recognizer, void *context)  /* функция, срабатывающая при клике на кнопки */
{
    //send_msg("1","off",DEF_IP,DEF_PORT);
    selected++;
    layer_mark_dirty(layer);
}

void WindowsClickConfigProvider(void *context)  /* функция, внутри которой должны находиться подписки на кнопки */
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click); /* при нажатии на верхнюю кнопку запустить click */
    window_single_click_subscribe(BUTTON_ID_SELECT, up_click); 
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click); 
}

void layer_update_callback(Layer *me, GContext* ctx) {
    //graphics_context_set_stroke_color(ctx, GColorBlack);
    //graphics_context_set_fill_color(ctx, GColorBlack);
    //graphics_context_set_text_color(ctx, GColorBlack);

    int i;
    for(i=0;i<6;i++){
    graphics_draw_line(ctx,GPoint(0,i*29),GPoint(144,i*29));
    if (i != selected)
    {
    text_layer_set_text_color(text_layer[i], GColorBlack);  
    text_layer_set_background_color(text_layer[i], GColorClear);

        static char buf[11];
    snprintf(buf, sizeof(buf), "%d", i);
    APP_LOG(APP_LOG_LEVEL_DEBUG, buf);
}

    }


    text_layer_set_text_color(text_layer[selected], GColorWhite);  
    text_layer_set_background_color(text_layer[selected], GColorBlack);
    

}

int main(void)
{
    window = window_create();  /* Инициализируем окно */
    window_set_fullscreen(window, true); /* включаем полноэкранность */
    window_stack_push(window, true);  /* открываем окно */
    app_message_open(64, 64);



    int i;
    for(i=0;i<6;i++){
    text_layer[i] = text_layer_create(GRect(0, i*29, 144, i*29+29)); 
    text_layer_set_text_color(text_layer[i], GColorBlack);  
    text_layer_set_background_color(text_layer[i], GColorClear);
    text_layer_set_font(text_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(text_layer[i], GTextAlignmentLeft);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer[i]));
    text_layer_set_text(text_layer[i], messages[i*2]);
    }
    text_layer_set_text_color(text_layer[selected], GColorWhite);  
    text_layer_set_background_color(text_layer[selected], GColorBlack);

    layer = layer_create(GRect(0, 0, 144, 168));
    layer_set_update_proc(layer, layer_update_callback);
    layer_add_child(window_get_root_layer(window), layer);


    layer_mark_dirty(layer);


    window_set_click_config_provider(window, WindowsClickConfigProvider); /* определяем функцию, в которой будут находиться подписки на кнопки */
    app_event_loop();  /* ждем событий */
    window_destroy(window);  /* уничтожаем объекты, освобождаем ресурсы */
}
