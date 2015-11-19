// ++++++ digiTime ++++++
// Autor: DonPIZI
// donpizi@hotmail.de
// http://risingdreams.com

#include <pebble.h>
#define bgColor 0
#define timeColor 1
#define dateColor 2
#define weekdayColor 3
#define sundayColor 4

// ++++++ declatarion of variables ++++++
static Window *my_window;
static TextLayer *hour_layer, *min_layer, *mon_layer , *tue_layer, *wed_layer, *thu_layer, *fri_layer,
                  *sat_layer, *sun_layer, *day_layer, *month_layer, *test_layer;
GColor bg_color, time_color, week_color, sun_color, date_color;
static BitmapLayer *batt_layer, *bt_layer;
static GBitmap *batt, *bt;

//++++++ declaration of fonts ++++++
static GFont sf_bold;
static GFont sf;
static GFont sf_24;
static GFont sf_16;

static void in_recv_handler(DictionaryIterator *iterator, void *context){  
  Tuple *t = dict_read_first(iterator);  
  while (t != NULL){
    switch (t -> key){
      //++++++ background color +++++++
      case bgColor:
       persist_write_int(bgColor, t->value->uint8);
      break;
      //++++++ time color ++++++
      case timeColor:
        persist_write_int(timeColor, t->value->uint8);
      break;
      //++++++ date color ++++++
      case dateColor:
        persist_write_int(dateColor, t->value->uint8);
      break;
      //++++++ weekday color ++++++
      case weekdayColor:
        persist_write_int(weekdayColor, t->value->uint8);
      break;
      //++++++ sunday color ++++++
      case sundayColor:
        persist_write_int(sundayColor, t->value->uint8);
      break;
    } //switch
     t = dict_read_next(iterator);
  } // while
}

// ++++++ get data +++++++
// ++++++ get hour +++++++
static void update_hour(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[3];
  // ++++++ 24h style? ++++++
  if(clock_is_24h_style() == true){
    strftime(buffer, sizeof(buffer), "%H", tick_time);
  } else {
    strftime(buffer, sizeof(buffer), "%I", tick_time);
  }
  text_layer_set_text(hour_layer, buffer);
}

// ++++++ get minute ++++++
static void update_min(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[3];
  strftime(buffer, sizeof(buffer), "%M", tick_time);
  text_layer_set_text(min_layer, buffer);
}

//++++++ get date ++++++
static void update_day(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[3];
  strftime(buffer, sizeof(buffer), "%d", tick_time);
  text_layer_set_text(day_layer, buffer);
}

// ++++++ get month +++++++
static void update_month(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[4];
  strftime(buffer, sizeof(buffer), "%b", tick_time);
  text_layer_set_text(month_layer, buffer);
}

// ++++++ get weekday +++++++
static void update_wd(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[4];
  strftime(buffer, sizeof(buffer), "%a", tick_time);
  // ++++++ compare buffer with string and set text ++++++
  if(strcmp(buffer, "Mon") == 0 || strcmp(buffer, "Mo") == 0){
  text_layer_set_text(mon_layer, buffer);
    text_layer_set_text(sun_layer, "");
  } else if(strcmp(buffer, "Tue") == 0 || strcmp(buffer, "Di") == 0) {
    text_layer_set_text(tue_layer, buffer);
    text_layer_set_text(mon_layer, "");
  } else if(strcmp(buffer, "Wed") == 0 || strcmp(buffer, "Mi") == 0) {
    text_layer_set_text(wed_layer, buffer);
    text_layer_set_text(tue_layer, "");
  } else if(strcmp(buffer, "Thu") == 0 || strcmp(buffer, "Do") == 0) {
    text_layer_set_text(thu_layer, buffer);
    text_layer_set_text(wed_layer, "");
  } else if(strcmp(buffer, "Fri") == 0 || strcmp(buffer, "Fr") == 0) {
    text_layer_set_text(fri_layer, buffer);
    text_layer_set_text(thu_layer, "");
  } else if(strcmp(buffer, "Sat") == 0 || strcmp(buffer, "Sa")  == 0) {
    text_layer_set_text(sat_layer, buffer);
    text_layer_set_text(fri_layer, "");
  } else {
    text_layer_set_text(sun_layer, buffer);
     text_layer_set_text(sat_layer, ""); 
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_hour();
  update_min();
  update_day();
  update_month();
  update_wd();
}

// +++++++ Show current BT connection state +++++++
static void bt_handler(bool connected) {
  if (connected) {
    bitmap_layer_set_bitmap(bt_layer, NULL);
  } else {
    bitmap_layer_set_bitmap(bt_layer, bt);
    vibes_short_pulse();
  }
}

//+++++++ Show low battery icon +++++++
static void battery_handler(BatteryChargeState new_state) {
   static char buffer[4];
  snprintf(buffer, sizeof(buffer), "%d", new_state.charge_percent);
 if(strcmp(buffer , "20") == 0){
   bitmap_layer_set_bitmap(batt_layer, batt);
 } else if(strcmp(buffer , "20") == 0){
   bitmap_layer_set_bitmap(batt_layer, batt);
 } else if(strcmp(buffer , "10") == 0){
   bitmap_layer_set_bitmap(batt_layer, batt);
 } else {
   bitmap_layer_set_bitmap(batt_layer, NULL);
 }  
  //text_layer_set_text(test_layer, buffer); 
}

// ++++++ load content ++++++  
static void window_load(Window *my_window){
  // ++++++ create fonts ++++++
  sf_bold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SF_BOLD_62));
  sf = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SF_18));
  sf_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SF_24));
  sf_16 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SF_16));
  
  // ++++++ text color +++++++
 // #ifdef PBL_COLOR
   GColor time_color;
   GColor week_color;
  GColor sun_color;
  GColor date_color;
 // #endif
  
  // +++++++ time color +++++++  
  int setTimeColor = persist_read_int(timeColor);
  if (setTimeColor == 0){
  time_color = GColorVividCerulean;
  } else {
  time_color.argb = setTimeColor;
  }
  
 // ++++++ date color ++++++++
 int setDateColor = persist_read_int(dateColor);
  if (setDateColor == 0){
    date_color = GColorOrange;
  } else {
  date_color.argb = setDateColor;
  }
  
  // ++++++ weekday color +++++++
 int setWeekdayColor = persist_read_int(weekdayColor);
  if(setWeekdayColor == 0){
    week_color = GColorWhite;
  } else {
 week_color.argb = setWeekdayColor;
  }
    
  // ++++++ sunday color ++++++
 int setSundayColor = persist_read_int(sundayColor);
  if (setSundayColor == 0){
    sun_color = GColorRed;
  } else {
  sun_color.argb = setSundayColor;
  }
      
   // +++++ test layer +++++ 
  test_layer = text_layer_create(GRect(0, 147, 140, 65));
  text_layer_set_background_color(test_layer, GColorClear);
  text_layer_set_text_color(test_layer, GColorBlack);
  text_layer_set_font(test_layer, sf_16);
  text_layer_set_text_alignment(test_layer, GTextAlignmentRight);
    
    // ++++++ create layers ++++++
    /// ++++++ hours +++++++
  //hour_layer = text_layer_create(GRect(14, 44, 130, 65)); // left, top, width, height  date bottom -12  date top 44
   hour_layer = text_layer_create(GRect(14, -2, 130, 65));
  text_layer_set_background_color(hour_layer, GColorClear);
  text_layer_set_text_color(hour_layer, time_color);
  text_layer_set_font(hour_layer, sf_bold);
  text_layer_set_text_alignment(hour_layer, GTextAlignmentRight);
  //text_layer_set_text(hour_layer, "23");
  // ++++++ minutes +++++
  //min_layer = text_layer_create(GRect(14, 96, 130, 65)); // left, top, width, height   40    96
  min_layer = text_layer_create(GRect(14, 86, 130, 65));
  text_layer_set_background_color(min_layer, GColorClear);
  text_layer_set_text_color(min_layer, time_color);
  text_layer_set_font(min_layer, sf_bold);
  text_layer_set_text_alignment(min_layer, GTextAlignmentRight);
  //text_layer_set_text(min_layer, "59");
  
    // +++++++ date +++++++
  //++++++++ day +++++++
  //day_layer = text_layer_create(GRect(72, 0, 50, 30)); // left, top, width, height   107  0
  day_layer = text_layer_create(GRect(92, 66, 50, 30));   // date middle
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_text_color(day_layer, date_color);
  text_layer_set_font(day_layer, sf_24);
  text_layer_set_text_alignment(day_layer, GTextAlignmentRight);
  //text_layer_set_text(day_layer, "28");
  //+++++++ month +++++++
  //month_layer = text_layer_create(GRect(61, 26, 70, 30)); // left, top, width, height   133  26
  month_layer = text_layer_create(GRect(50, 66, 70, 30));
  text_layer_set_background_color(month_layer, GColorClear);
  text_layer_set_text_color(month_layer, date_color);
  text_layer_set_font(month_layer, sf_24);
  text_layer_set_text_alignment(month_layer, GTextAlignmentLeft);
  //text_layer_set_text(month_layer, "Aug");
  
  //++++++ Weekdays +++++++
  //++++++ Monday ++++++
  mon_layer = text_layer_create(GRect(2, 2, 60, 20)); // left, top, width, height
  text_layer_set_background_color(mon_layer, GColorClear);
  text_layer_set_text_color(mon_layer, week_color);
  text_layer_set_font(mon_layer, sf);
  text_layer_set_text_alignment(mon_layer, GTextAlignmentLeft);
  //text_layer_set_text(mon_layer, "Mon");
  //+++++++ Tuesday ++++++
  tue_layer = text_layer_create(GRect(2, 25, 60, 20)); // left, top, width, height
  text_layer_set_background_color(tue_layer, GColorClear);
  text_layer_set_text_color(tue_layer, week_color);
  text_layer_set_font(tue_layer, sf);
  text_layer_set_text_alignment(tue_layer, GTextAlignmentLeft);
  //text_layer_set_text(tue_layer, "Tue");  
  // +++++++ Wednesday +++++++
  wed_layer = text_layer_create(GRect(2, 48, 60, 20)); // left, top, width, height
  text_layer_set_background_color(wed_layer, GColorClear);
  text_layer_set_text_color(wed_layer, week_color);
  text_layer_set_font(wed_layer, sf);
  text_layer_set_text_alignment(wed_layer, GTextAlignmentLeft);
  //text_layer_set_text(wed_layer, "Wed");
  // +++++++ Thuesday +++++++
  thu_layer = text_layer_create(GRect(2, 71, 60, 20)); // left, top, width, height
  text_layer_set_background_color(thu_layer, GColorClear);
  text_layer_set_text_color(thu_layer, week_color);
  text_layer_set_font(thu_layer, sf);
  text_layer_set_text_alignment(thu_layer, GTextAlignmentLeft);
  //text_layer_set_text(thu_layer, "Thu");
  //+++++++ Friday +++++++
  fri_layer = text_layer_create(GRect(2, 94, 60, 20)); // left, top, width, height
  text_layer_set_background_color(fri_layer, GColorClear);
  text_layer_set_text_color(fri_layer, week_color);
  text_layer_set_font(fri_layer, sf);
  text_layer_set_text_alignment(fri_layer, GTextAlignmentLeft);
  //text_layer_set_text(fri_layer, "Fri");
  // +++++++ Saturday ++++++++
  sat_layer = text_layer_create(GRect(2, 117, 60, 20)); // left, top, width, height
  text_layer_set_background_color(sat_layer, GColorClear);
  text_layer_set_text_color(sat_layer, week_color);
  text_layer_set_font(sat_layer, sf);
  text_layer_set_text_alignment(sat_layer, GTextAlignmentLeft);
  //text_layer_set_text(sat_layer, "Sat");
  // +++++++ Sunday ++++++++
  sun_layer = text_layer_create(GRect(2, 140, 60, 20)); // left, top, width, height
  text_layer_set_background_color(sun_layer, GColorClear);
  text_layer_set_text_color(sun_layer, sun_color);
  text_layer_set_font(sun_layer, sf);
  text_layer_set_text_alignment(sun_layer, GTextAlignmentLeft);
  //text_layer_set_text(sun_layer, "Sun");
  
  // ++++++ low bat layer ++++++
  battery_handler(battery_state_service_peek());
  batt = gbitmap_create_with_resource(RESOURCE_ID_LOWBATT);
  batt_layer = bitmap_layer_create(GRect(125, -2, 16, 16)); // left, top, width, height
  //bitmap_layer_set_bitmap(batt_layer, batt);

  // ++++++ no bt ++++++++
  bt_handler(bluetooth_connection_service_peek());
  
  bt = gbitmap_create_with_resource(RESOURCE_ID_BT);
  bt_layer = bitmap_layer_create(GRect(110, 0, 12, 12)); // left, top, width, height
  // bitmap_layer_set_bitmap(bt_layer, bt);
  
 // add layers as children of window
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(hour_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(min_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(mon_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(tue_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(wed_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(thu_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(fri_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(sat_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(sun_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(day_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(month_layer));
 layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(batt_layer));
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(bt_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(test_layer));
}  

static void window_unload(Window *my_window){
  // ++++++ delete used layers ++++++
  text_layer_destroy(hour_layer);
  text_layer_destroy(min_layer);
  text_layer_destroy(mon_layer);
  text_layer_destroy(tue_layer);
  text_layer_destroy(wed_layer);
  text_layer_destroy(thu_layer);
  text_layer_destroy(fri_layer);
  text_layer_destroy(sat_layer);
  text_layer_destroy(sun_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(month_layer);
  text_layer_destroy(test_layer);
  
  // ++++++ delete fonts ++++++
  fonts_unload_custom_font(sf_bold);
  fonts_unload_custom_font(sf);
  fonts_unload_custom_font(sf_24);
  fonts_unload_custom_font(sf_16);
  
  // +++++++ delete bitmap ++++++
  gbitmap_destroy(batt);
  bitmap_layer_destroy(batt_layer);
  gbitmap_destroy(bt);
  bitmap_layer_destroy(bt_layer);
}

static void init(void) {  
  // ++++++ create window +++++++
  my_window = window_create();
  
  // ++++++ set Window Color ++++++
  int setBgColor = persist_read_int(bgColor);  
  if (setBgColor == 0){
    window_set_background_color(my_window, GColorBlack);
  } else {
    bg_color.argb = setBgColor;
  window_set_background_color(my_window, bg_color);
  }
  
  window_set_window_handlers(my_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  
  //++++++ updates every minute ++++++
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  bluetooth_connection_service_subscribe(bt_handler);
  battery_state_service_subscribe(battery_handler);
  window_stack_push(my_window, true);
  
  //++++++ show time and date ++++++
  update_hour();
  update_min();
  update_day();
  update_month();
  update_wd();
  
  //setings
app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void) {
  // ++++++ delete window ++++++
  window_destroy(my_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
