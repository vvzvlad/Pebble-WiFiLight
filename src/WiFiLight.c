#include "pebble.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 4
#define DEF_IP "162.211.231.73"
#define DEF_PORT "8080"

static Window *window;
static SimpleMenuLayer *simple_menu_layer;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem first_menu_items[NUM_FIRST_MENU_ITEMS];
static GBitmap *menu_icon_image;


int mapping[7] = {1,4,2,3,5,6,};

static void send_msg(char *channel, char *command, char *ip, char *port) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_cstring(iter, 0, command);
    dict_write_cstring(iter, 1, channel);
    dict_write_cstring(iter, 2, ip);
    dict_write_cstring(iter, 3, port);
    app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "sended!");
}



static void menu_select_callback(int index, void *ctx) {
  first_menu_items[index].subtitle = "Отправка...";
  static char buf[2];
  snprintf(buf, sizeof(buf), "%d", mapping[index]);
  APP_LOG(APP_LOG_LEVEL_DEBUG, buf);
  send_msg(buf,"sw",DEF_IP,DEF_PORT);
  layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer));
}

void menu_initialization(void){
    menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_1);


    int num_a_items = 0;
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Плафон 1",
    .callback = menu_select_callback,
    .icon = menu_icon_image,
  };
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Плафон 2",
    .callback = menu_select_callback,
    .icon = menu_icon_image,
  };
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Розетка",
    .callback = menu_select_callback,
    .icon = menu_icon_image,
  };
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Лампа",
    .callback = menu_select_callback,
    .icon = menu_icon_image,
  };


  menu_sections[0] = (SimpleMenuSection){
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = first_menu_items,
  };

  simple_menu_layer = simple_menu_layer_create(GRect(0, 0, 144, 168), window, menu_sections, NUM_MENU_SECTIONS, NULL);
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(simple_menu_layer));
}


int main(void)
{
  window = window_create();
  window_set_fullscreen(window, true);
  window_stack_push(window, true);
 
  menu_initialization();
  app_message_open(64,64);

  app_event_loop();

  window_destroy(window);
  simple_menu_layer_destroy(simple_menu_layer);
  gbitmap_destroy(menu_icon_image);
}
