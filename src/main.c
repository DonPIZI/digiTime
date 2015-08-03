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


//static char *setBgColor;


// settings 

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  
  Tuple *t = dict_read_first(iterator);
  
  while (t != NULL){
    switch (t -> key){
      //++++++ background color +++++++
      case bgColor:
        persist_write_string(bgColor, t->value->cstring);
      break;
      //++++++ time color ++++++
      case timeColor:
        persist_write_string(timeColor, t->value->cstring);
      break;
      //++++++ date color ++++++
      case dateColor:
        persist_write_string(timeColor, t->value->cstring);
      break;
      //++++++ weekday color ++++++
      case weekdayColor:
        persist_write_string(timeColor, t->value->cstring);
      break;
      //++++++ sunday color ++++++
      case sundayColor:
        persist_write_string(timeColor, t->value->cstring);
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
  if(strcmp(buffer, "Mon") == 0){
  text_layer_set_text(mon_layer, buffer);
    text_layer_set_text(sun_layer, "");
  } else if(strcmp(buffer, "Tue") == 0) {
    text_layer_set_text(tue_layer, buffer);
    text_layer_set_text(mon_layer, "");
  } else if(strcmp(buffer, "Wed") == 0) {
    text_layer_set_text(wed_layer, buffer);
    text_layer_set_text(tue_layer, "");
  } else if(strcmp(buffer, "Thu") == 0) {
    text_layer_set_text(thu_layer, buffer);
    text_layer_set_text(wed_layer, "");
  } else if(strcmp(buffer, "Fri") == 0) {
    text_layer_set_text(fri_layer, buffer);
    text_layer_set_text(thu_layer, "");
  } else if(strcmp(buffer, "Sat") == 0) {
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
  char setTimeColor[25]; 
  persist_read_string(timeColor, setTimeColor, sizeof(setTimeColor));
 if(strcmp(setTimeColor, "inchworm") == 0)
      {
        time_color =  GColorInchworm;
      }else if(strcmp(setTimeColor,  "springBud") == 0)
      {
        time_color =  GColorSpringBud;
      } else if(strcmp(setTimeColor,  "brightGreen") == 0)
      {
        time_color =  GColorBrightGreen;
      } else if(strcmp(setTimeColor,  "mintGreen") == 0)
      {
        time_color =  GColorMintGreen;
      } else if(strcmp(setTimeColor,  "screaminGreen") == 0)
      {
        time_color =  GColorScreaminGreen;
      } else if(strcmp(setTimeColor,  "green") == 0)
      {
        time_color =  GColorGreen;
      } else if(strcmp(setTimeColor,  "malachite") == 0)
      {
        time_color =  GColorMalachite;
      } else if(strcmp(setTimeColor,  "islamicGreen") == 0)
      {
        time_color =  GColorIslamicGreen;
      } else if(strcmp(setTimeColor,  "kellyGreen") == 0)
      {
        time_color =  GColorKellyGreen;
      } else if(strcmp(setTimeColor,  "brass") == 0)
      {
        time_color =  GColorBrass;
      } else if(strcmp(setTimeColor,  "limerick") == 0)
      {
        time_color =  GColorLimerick;
      } else if(strcmp(setTimeColor,  "mediumAquamarine") == 0)
      {
        time_color =  GColorMediumAquamarine;
      } else if(strcmp(setTimeColor,  "jaegerGreen") == 0)
      {
        time_color =  GColorJaegerGreen;
      } else if(strcmp(setTimeColor,  "mayGreen") == 0)
      {
        time_color =  GColorMayGreen;
      } else if(strcmp(setTimeColor,  "darkGreen") == 0)
      {
        time_color =  GColorDarkGreen;
      } else if(strcmp(setTimeColor,  "armyGreen") == 0)
      {
        time_color =  GColorArmyGreen;
      } else if(strcmp(setTimeColor,  "mediumSpringGreen") == 0)
      {
        time_color =  GColorMediumSpringGreen;
      } else if(strcmp(setTimeColor,  "tiffanyBlue") == 0)
      {
        time_color =  GColorTiffanyBlue;
      } else if(strcmp(setTimeColor,  "cadetBlue") == 0)
      {
        time_color =  GColorCadetBlue;
      } else if(strcmp(setTimeColor,  "midnightGreen") == 0)
      {
        time_color =  GColorMidnightGreen;
      } else if(strcmp(setTimeColor,  "electricBlue") == 0)
      {
        time_color =  GColorElectricBlue;
      } else if(strcmp(setTimeColor,  "cyan") == 0)
      {
        time_color =  GColorCyan;
      } else if(strcmp(setTimeColor,  "celeste") == 0)
      {
        time_color =  GColorCeleste;
      } else if(strcmp(setTimeColor,  "cobaltBlue") == 0)
      {
        time_color =  GColorCobaltBlue;
      } else if(strcmp(setTimeColor,  "vividCerulean") == 0)
      {
        time_color =  GColorVividCerulean;
      } else if(strcmp(setTimeColor,  "dukeBlue") == 0)
      {
        time_color =  GColorDukeBlue;
      } else if(strcmp(setTimeColor,  "oxfordBlue") == 0)
      {
        time_color =  GColorOxfordBlue;
      } else if(strcmp(setTimeColor,  "pictonBlue") == 0)
      {
        time_color =  GColorPictonBlue;
      } else if(strcmp(setTimeColor,  "blue") == 0)
      {
        time_color =  GColorBlue;
      } else if(strcmp(setTimeColor,  "electricUltramarine") == 0)
      {
        time_color =  GColorElectricUltramarine;
      } else if(strcmp(setTimeColor,  "blueMoon") == 0)
      {
        time_color =  GColorBlueMoon;
      } else if(strcmp(setTimeColor,  "veryLightBlue") == 0)
      {
        time_color =  GColorVeryLightBlue;
      } else if(strcmp(setTimeColor,  "liberty") == 0)
      {
        time_color =  GColorLiberty;
      } else if(strcmp(setTimeColor,  "babyBlueEyes") == 0)
      {
        time_color =  GColorBabyBlueEyes;
      } else if(strcmp(setTimeColor,  "lavenderIndigo") == 0)
      {
        time_color =  GColorLavenderIndigo;
      } else if(strcmp(setTimeColor,  "vividViolet") == 0)
      {
        time_color =  GColorVividViolet;
      } else if(strcmp(setTimeColor,  "purpureus") == 0)
      {
        time_color =  GColorPurpureus;
      } else if(strcmp(setTimeColor,  "shockingPink") == 0)
      {
        time_color =  GColorShockingPink;
      } else if(strcmp(setTimeColor,  "richBrilliantLavender") == 0)
      {
        time_color =  GColorRichBrilliantLavender;
      } else if(strcmp(setTimeColor,  "brilliantRose") == 0)
      {
        time_color =  GColorBrilliantRose;
      } else if(strcmp(setTimeColor,  "magenta") == 0)
      {
        time_color =  GColorMagenta;
      } else if(strcmp(setTimeColor,  "purple") == 0)
      {
        time_color =  GColorPurple;
      } else if(strcmp(setTimeColor,  "indigo") == 0)
      {
        time_color =  GColorIndigo;
      } else if(strcmp(setTimeColor,  "imperialPurple") == 0)
      {
        time_color =  GColorImperialPurple;
      } else if(strcmp(setTimeColor,  "jazzberryJam") == 0)
      {
        time_color =  GColorJazzberryJam;
      } else if(strcmp(setTimeColor,  "fashionMagenta") == 0)
      {
        time_color =  GColorFashionMagenta;
      } else if(strcmp(setTimeColor,  "darkCandyAppleRed") == 0)
      {
        time_color =  GColorDarkCandyAppleRed;
      } else if(strcmp(setTimeColor,  "bulgarianRose") == 0)
      {
        time_color =  GColorBulgarianRose;
      } else if(strcmp(setTimeColor,  "roseVale") == 0)
      {
        time_color =  GColorRoseVale;
      } else if(strcmp(setTimeColor,  "folly") == 0)
      {
        time_color =  GColorFolly;
      } else if(strcmp(setTimeColor,  "sunsetOrange") == 0)
      {
        time_color =  GColorSunsetOrange;
      } else if(strcmp(setTimeColor,  "red") == 0)
      {
        time_color =  GColorRed;
      } else if(strcmp(setTimeColor,  "windsorTan") == 0)
      {
        time_color =  GColorWindsorTan;
      } else if(strcmp(setTimeColor,  "chromeYellow") == 0)
      {
        time_color =  GColorChromeYellow;
      } else if(strcmp(setTimeColor,  "orange") == 0)
      {
        time_color =  GColorOrange;
      } else if(strcmp(setTimeColor,  "melon") == 0)
      {
        time_color =  GColorMelon;
      } else if(strcmp(setTimeColor,  "rajah") == 0)
      {
        time_color =  GColorRajah;
      } else if(strcmp(setTimeColor,  "yellow") == 0)
      {
        time_color =  GColorYellow;
      } else if(strcmp(setTimeColor,  "icterine") == 0)
      {
        time_color =  GColorIcterine;
      } else if(strcmp(setTimeColor,  "pastelYellow") == 0)
      {
        time_color =  GColorPastelYellow;
      } else if(strcmp(setTimeColor,  "white") == 0)
      {
        time_color =  GColorWhite;
      } else if(strcmp(setTimeColor,  "lightGray") == 0)
      {
        time_color =  GColorLightGray;
      } else if(strcmp(setTimeColor,  "darkGray") == 0)
      {
        time_color =  GColorDarkGray;
      } else if(strcmp(setTimeColor,  "black") == 0)
      {
        time_color =  GColorBlack;
      } else { 
         time_color =  GColorVividCerulean;
      }
  
 // ++++++ date color ++++++++
    char setDateColor[25]; 
  persist_read_string(dateColor, setDateColor, sizeof(setDateColor));
 if(strcmp(setDateColor, "inchworm") == 0)
      {
        date_color =  GColorInchworm;
      }else if(strcmp(setDateColor,  "springBud") == 0)
      {
        date_color =  GColorSpringBud;
      } else if(strcmp(setDateColor,  "brightGreen") == 0)
      {
        date_color =  GColorBrightGreen;
      } else if(strcmp(setDateColor,  "mintGreen") == 0)
      {
        date_color =  GColorMintGreen;
      } else if(strcmp(setDateColor,  "screaminGreen") == 0)
      {
        date_color =  GColorScreaminGreen;
      } else if(strcmp(setDateColor,  "green") == 0)
      {
        date_color =  GColorGreen;
      } else if(strcmp(setDateColor,  "malachite") == 0)
      {
        date_color =  GColorMalachite;
      } else if(strcmp(setDateColor,  "islamicGreen") == 0)
      {
        date_color =  GColorIslamicGreen;
      } else if(strcmp(setDateColor,  "kellyGreen") == 0)
      {
        date_color =  GColorKellyGreen;
      } else if(strcmp(setDateColor,  "brass") == 0)
      {
        date_color =  GColorBrass;
      } else if(strcmp(setDateColor,  "limerick") == 0)
      {
        date_color =  GColorLimerick;
      } else if(strcmp(setDateColor,  "mediumAquamarine") == 0)
      {
        date_color =  GColorMediumAquamarine;
      } else if(strcmp(setDateColor,  "jaegerGreen") == 0)
      {
        date_color =  GColorJaegerGreen;
      } else if(strcmp(setDateColor,  "mayGreen") == 0)
      {
        date_color =  GColorMayGreen;
      } else if(strcmp(setDateColor,  "darkGreen") == 0)
      {
        date_color =  GColorDarkGreen;
      } else if(strcmp(setDateColor,  "armyGreen") == 0)
      {
        date_color =  GColorArmyGreen;
      } else if(strcmp(setDateColor,  "mediumSpringGreen") == 0)
      {
        date_color =  GColorMediumSpringGreen;
      } else if(strcmp(setDateColor,  "tiffanyBlue") == 0)
      {
        date_color =  GColorTiffanyBlue;
      } else if(strcmp(setDateColor,  "cadetBlue") == 0)
      {
        date_color =  GColorCadetBlue;
      } else if(strcmp(setDateColor,  "midnightGreen") == 0)
      {
        date_color =  GColorMidnightGreen;
      } else if(strcmp(setDateColor,  "electricBlue") == 0)
      {
        date_color =  GColorElectricBlue;
      } else if(strcmp(setDateColor,  "cyan") == 0)
      {
        date_color =  GColorCyan;
      } else if(strcmp(setDateColor,  "celeste") == 0)
      {
        date_color =  GColorCeleste;
      } else if(strcmp(setDateColor,  "cobaltBlue") == 0)
      {
        date_color =  GColorCobaltBlue;
      } else if(strcmp(setDateColor,  "vividCerulean") == 0)
      {
        date_color =  GColorVividCerulean;
      } else if(strcmp(setDateColor,  "dukeBlue") == 0)
      {
        date_color =  GColorDukeBlue;
      } else if(strcmp(setDateColor,  "oxfordBlue") == 0)
      {
        date_color =  GColorOxfordBlue;
      } else if(strcmp(setDateColor,  "pictonBlue") == 0)
      {
        date_color =  GColorPictonBlue;
      } else if(strcmp(setDateColor,  "blue") == 0)
      {
        date_color =  GColorBlue;
      } else if(strcmp(setDateColor,  "electricUltramarine") == 0)
      {
        date_color =  GColorElectricUltramarine;
      } else if(strcmp(setDateColor,  "blueMoon") == 0)
      {
        date_color =  GColorBlueMoon;
      } else if(strcmp(setDateColor,  "veryLightBlue") == 0)
      {
        date_color =  GColorVeryLightBlue;
      } else if(strcmp(setDateColor,  "liberty") == 0)
      {
        date_color =  GColorLiberty;
      } else if(strcmp(setDateColor,  "babyBlueEyes") == 0)
      {
        date_color =  GColorBabyBlueEyes;
      } else if(strcmp(setDateColor,  "lavenderIndigo") == 0)
      {
        date_color =  GColorLavenderIndigo;
      } else if(strcmp(setDateColor,  "vividViolet") == 0)
      {
        date_color =  GColorVividViolet;
      } else if(strcmp(setDateColor,  "purpureus") == 0)
      {
        date_color =  GColorPurpureus;
      } else if(strcmp(setDateColor,  "shockingPink") == 0)
      {
        date_color =  GColorShockingPink;
      } else if(strcmp(setDateColor,  "richBrilliantLavender") == 0)
      {
        date_color =  GColorRichBrilliantLavender;
      } else if(strcmp(setDateColor,  "brilliantRose") == 0)
      {
        date_color =  GColorBrilliantRose;
      } else if(strcmp(setDateColor,  "magenta") == 0)
      {
        date_color =  GColorMagenta;
      } else if(strcmp(setDateColor,  "purple") == 0)
      {
        date_color =  GColorPurple;
      } else if(strcmp(setDateColor,  "indigo") == 0)
      {
        date_color =  GColorIndigo;
      } else if(strcmp(setDateColor,  "imperialPurple") == 0)
      {
        date_color =  GColorImperialPurple;
      } else if(strcmp(setDateColor,  "jazzberryJam") == 0)
      {
        date_color =  GColorJazzberryJam;
      } else if(strcmp(setDateColor,  "fashionMagenta") == 0)
      {
        date_color =  GColorFashionMagenta;
      } else if(strcmp(setDateColor,  "darkCandyAppleRed") == 0)
      {
        date_color =  GColorDarkCandyAppleRed;
      } else if(strcmp(setDateColor,  "bulgarianRose") == 0)
      {
        date_color =  GColorBulgarianRose;
      } else if(strcmp(setDateColor,  "roseVale") == 0)
      {
        date_color =  GColorRoseVale;
      } else if(strcmp(setDateColor,  "folly") == 0)
      {
        date_color =  GColorFolly;
      } else if(strcmp(setDateColor,  "sunsetOrange") == 0)
      {
        date_color =  GColorSunsetOrange;
      } else if(strcmp(setDateColor,  "red") == 0)
      {
        date_color =  GColorRed;
      } else if(strcmp(setDateColor,  "windsorTan") == 0)
      {
        date_color =  GColorWindsorTan;
      } else if(strcmp(setDateColor,  "chromeYellow") == 0)
      {
        date_color =  GColorChromeYellow;
      } else if(strcmp(setDateColor,  "orange") == 0)
      {
        date_color =  GColorOrange;
      } else if(strcmp(setDateColor,  "melon") == 0)
      {
        date_color =  GColorMelon;
      } else if(strcmp(setDateColor,  "rajah") == 0)
      {
        date_color =  GColorRajah;
      } else if(strcmp(setDateColor,  "yellow") == 0)
      {
        date_color =  GColorYellow;
      } else if(strcmp(setDateColor,  "icterine") == 0)
      {
        date_color =  GColorIcterine;
      } else if(strcmp(setDateColor,  "pastelYellow") == 0)
      {
        date_color =  GColorPastelYellow;
      } else if(strcmp(setDateColor,  "white") == 0)
      {
        date_color =  GColorWhite;
      } else if(strcmp(setDateColor,  "lightGray") == 0)
      {
        date_color =  GColorLightGray;
      } else if(strcmp(setDateColor,  "darkGray") == 0)
      {
        date_color =  GColorDarkGray;
      } else if(strcmp(setDateColor,  "black") == 0)
      {
        date_color =  GColorBlack;
      } else { 
         date_color =  GColorOrange;
      }
  
  // ++++++ weekday color +++++++
    char setWeekColor[25]; 
  persist_read_string(weekdayColor, setWeekColor, sizeof(setWeekColor));
 if(strcmp(setWeekColor, "inchworm") == 0)
      {
        week_color =  GColorInchworm;
      }else if(strcmp(setWeekColor,  "springBud") == 0)
      {
        week_color =  GColorSpringBud;
      } else if(strcmp(setWeekColor,  "brightGreen") == 0)
      {
        week_color =  GColorBrightGreen;
      } else if(strcmp(setWeekColor,  "mintGreen") == 0)
      {
        week_color =  GColorMintGreen;
      } else if(strcmp(setWeekColor,  "screaminGreen") == 0)
      {
        week_color =  GColorScreaminGreen;
      } else if(strcmp(setWeekColor,  "green") == 0)
      {
        week_color =  GColorGreen;
      } else if(strcmp(setWeekColor,  "malachite") == 0)
      {
        week_color =  GColorMalachite;
      } else if(strcmp(setWeekColor,  "islamicGreen") == 0)
      {
        week_color =  GColorIslamicGreen;
      } else if(strcmp(setWeekColor,  "kellyGreen") == 0)
      {
        week_color =  GColorKellyGreen;
      } else if(strcmp(setWeekColor,  "brass") == 0)
      {
        week_color =  GColorBrass;
      } else if(strcmp(setWeekColor,  "limerick") == 0)
      {
        week_color =  GColorLimerick;
      } else if(strcmp(setWeekColor,  "mediumAquamarine") == 0)
      {
        week_color =  GColorMediumAquamarine;
      } else if(strcmp(setWeekColor,  "jaegerGreen") == 0)
      {
        week_color =  GColorJaegerGreen;
      } else if(strcmp(setWeekColor,  "mayGreen") == 0)
      {
        week_color =  GColorMayGreen;
      } else if(strcmp(setWeekColor,  "darkGreen") == 0)
      {
        week_color =  GColorDarkGreen;
      } else if(strcmp(setWeekColor,  "armyGreen") == 0)
      {
        week_color =  GColorArmyGreen;
      } else if(strcmp(setWeekColor,  "mediumSpringGreen") == 0)
      {
        week_color =  GColorMediumSpringGreen;
      } else if(strcmp(setWeekColor,  "tiffanyBlue") == 0)
      {
        week_color =  GColorTiffanyBlue;
      } else if(strcmp(setWeekColor,  "cadetBlue") == 0)
      {
        week_color =  GColorCadetBlue;
      } else if(strcmp(setWeekColor,  "midnightGreen") == 0)
      {
        week_color =  GColorMidnightGreen;
      } else if(strcmp(setWeekColor,  "electricBlue") == 0)
      {
        week_color =  GColorElectricBlue;
      } else if(strcmp(setWeekColor,  "cyan") == 0)
      {
        week_color =  GColorCyan;
      } else if(strcmp(setWeekColor,  "celeste") == 0)
      {
        week_color =  GColorCeleste;
      } else if(strcmp(setWeekColor,  "cobaltBlue") == 0)
      {
        week_color =  GColorCobaltBlue;
      } else if(strcmp(setWeekColor,  "vividCerulean") == 0)
      {
        week_color =  GColorVividCerulean;
      } else if(strcmp(setWeekColor,  "dukeBlue") == 0)
      {
        week_color =  GColorDukeBlue;
      } else if(strcmp(setWeekColor,  "oxfordBlue") == 0)
      {
        week_color =  GColorOxfordBlue;
      } else if(strcmp(setWeekColor,  "pictonBlue") == 0)
      {
        week_color =  GColorPictonBlue;
      } else if(strcmp(setWeekColor,  "blue") == 0)
      {
        week_color =  GColorBlue;
      } else if(strcmp(setWeekColor,  "electricUltramarine") == 0)
      {
        week_color =  GColorElectricUltramarine;
      } else if(strcmp(setWeekColor,  "blueMoon") == 0)
      {
        week_color =  GColorBlueMoon;
      } else if(strcmp(setWeekColor,  "veryLightBlue") == 0)
      {
        week_color =  GColorVeryLightBlue;
      } else if(strcmp(setWeekColor,  "liberty") == 0)
      {
        week_color =  GColorLiberty;
      } else if(strcmp(setWeekColor,  "babyBlueEyes") == 0)
      {
        week_color =  GColorBabyBlueEyes;
      } else if(strcmp(setWeekColor,  "lavenderIndigo") == 0)
      {
        week_color =  GColorLavenderIndigo;
      } else if(strcmp(setWeekColor,  "vividViolet") == 0)
      {
        week_color =  GColorVividViolet;
      } else if(strcmp(setWeekColor,  "purpureus") == 0)
      {
        week_color =  GColorPurpureus;
      } else if(strcmp(setWeekColor,  "shockingPink") == 0)
      {
        week_color =  GColorShockingPink;
      } else if(strcmp(setWeekColor,  "richBrilliantLavender") == 0)
      {
        week_color =  GColorRichBrilliantLavender;
      } else if(strcmp(setWeekColor,  "brilliantRose") == 0)
      {
        week_color =  GColorBrilliantRose;
      } else if(strcmp(setWeekColor,  "magenta") == 0)
      {
        week_color =  GColorMagenta;
      } else if(strcmp(setWeekColor,  "purple") == 0)
      {
        week_color =  GColorPurple;
      } else if(strcmp(setWeekColor,  "indigo") == 0)
      {
        week_color =  GColorIndigo;
      } else if(strcmp(setWeekColor,  "imperialPurple") == 0)
      {
        week_color =  GColorImperialPurple;
      } else if(strcmp(setWeekColor,  "jazzberryJam") == 0)
      {
        week_color =  GColorJazzberryJam;
      } else if(strcmp(setWeekColor,  "fashionMagenta") == 0)
      {
        week_color =  GColorFashionMagenta;
      } else if(strcmp(setWeekColor,  "darkCandyAppleRed") == 0)
      {
        week_color =  GColorDarkCandyAppleRed;
      } else if(strcmp(setWeekColor,  "bulgarianRose") == 0)
      {
        week_color =  GColorBulgarianRose;
      } else if(strcmp(setWeekColor,  "roseVale") == 0)
      {
        week_color =  GColorRoseVale;
      } else if(strcmp(setWeekColor,  "folly") == 0)
      {
        week_color =  GColorFolly;
      } else if(strcmp(setWeekColor,  "sunsetOrange") == 0)
      {
        week_color =  GColorSunsetOrange;
      } else if(strcmp(setWeekColor,  "red") == 0)
      {
        week_color =  GColorRed;
      } else if(strcmp(setWeekColor,  "windsorTan") == 0)
      {
        week_color =  GColorWindsorTan;
      } else if(strcmp(setWeekColor,  "chromeYellow") == 0)
      {
        week_color =  GColorChromeYellow;
      } else if(strcmp(setWeekColor,  "orange") == 0)
      {
        week_color =  GColorOrange;
      } else if(strcmp(setWeekColor,  "melon") == 0)
      {
        week_color =  GColorMelon;
      } else if(strcmp(setWeekColor,  "rajah") == 0)
      {
        week_color =  GColorRajah;
      } else if(strcmp(setWeekColor,  "yellow") == 0)
      {
        week_color =  GColorYellow;
      } else if(strcmp(setWeekColor,  "icterine") == 0)
      {
        week_color =  GColorIcterine;
      } else if(strcmp(setWeekColor,  "pastelYellow") == 0)
      {
        week_color =  GColorPastelYellow;
      } else if(strcmp(setWeekColor,  "white") == 0)
      {
        week_color =  GColorWhite;
      } else if(strcmp(setWeekColor,  "lightGray") == 0)
      {
        week_color =  GColorLightGray;
      } else if(strcmp(setWeekColor,  "darkGray") == 0)
      {
        week_color =  GColorDarkGray;
      } else if(strcmp(setWeekColor,  "black") == 0)
      {
        week_color =  GColorBlack;
      } else { 
         week_color =  GColorWhite;
      }
  
  
  // ++++++ sunday color ++++++
    char setSundayColor[25]; 
  persist_read_string(sundayColor, setSundayColor, sizeof(setSundayColor));
 if(strcmp(setSundayColor, "inchworm") == 0)
      {
        sun_color =  GColorInchworm;
      }else if(strcmp(setSundayColor,  "springBud") == 0)
      {
        sun_color =  GColorSpringBud;
      } else if(strcmp(setSundayColor,  "brightGreen") == 0)
      {
        sun_color =  GColorBrightGreen;
      } else if(strcmp(setSundayColor,  "mintGreen") == 0)
      {
        sun_color =  GColorMintGreen;
      } else if(strcmp(setSundayColor,  "screaminGreen") == 0)
      {
        sun_color =  GColorScreaminGreen;
      } else if(strcmp(setSundayColor,  "green") == 0)
      {
        sun_color =  GColorGreen;
      } else if(strcmp(setSundayColor,  "malachite") == 0)
      {
        sun_color =  GColorMalachite;
      } else if(strcmp(setSundayColor,  "islamicGreen") == 0)
      {
        sun_color =  GColorIslamicGreen;
      } else if(strcmp(setSundayColor,  "kellyGreen") == 0)
      {
        sun_color =  GColorKellyGreen;
      } else if(strcmp(setSundayColor,  "brass") == 0)
      {
        sun_color =  GColorBrass;
      } else if(strcmp(setSundayColor,  "limerick") == 0)
      {
        sun_color =  GColorLimerick;
      } else if(strcmp(setSundayColor,  "mediumAquamarine") == 0)
      {
        sun_color =  GColorMediumAquamarine;
      } else if(strcmp(setSundayColor,  "jaegerGreen") == 0)
      {
        sun_color =  GColorJaegerGreen;
      } else if(strcmp(setSundayColor,  "mayGreen") == 0)
      {
        sun_color =  GColorMayGreen;
      } else if(strcmp(setSundayColor,  "darkGreen") == 0)
      {
        sun_color =  GColorDarkGreen;
      } else if(strcmp(setSundayColor,  "armyGreen") == 0)
      {
        sun_color =  GColorArmyGreen;
      } else if(strcmp(setSundayColor,  "mediumSpringGreen") == 0)
      {
        sun_color =  GColorMediumSpringGreen;
      } else if(strcmp(setSundayColor,  "tiffanyBlue") == 0)
      {
        sun_color =  GColorTiffanyBlue;
      } else if(strcmp(setSundayColor,  "cadetBlue") == 0)
      {
        sun_color =  GColorCadetBlue;
      } else if(strcmp(setSundayColor,  "midnightGreen") == 0)
      {
        sun_color =  GColorMidnightGreen;
      } else if(strcmp(setSundayColor,  "electricBlue") == 0)
      {
        sun_color =  GColorElectricBlue;
      } else if(strcmp(setSundayColor,  "cyan") == 0)
      {
        sun_color =  GColorCyan;
      } else if(strcmp(setSundayColor,  "celeste") == 0)
      {
        sun_color =  GColorCeleste;
      } else if(strcmp(setSundayColor,  "cobaltBlue") == 0)
      {
        sun_color =  GColorCobaltBlue;
      } else if(strcmp(setSundayColor,  "vividCerulean") == 0)
      {
        sun_color =  GColorVividCerulean;
      } else if(strcmp(setSundayColor,  "dukeBlue") == 0)
      {
        sun_color =  GColorDukeBlue;
      } else if(strcmp(setSundayColor,  "oxfordBlue") == 0)
      {
        sun_color =  GColorOxfordBlue;
      } else if(strcmp(setSundayColor,  "pictonBlue") == 0)
      {
        sun_color =  GColorPictonBlue;
      } else if(strcmp(setSundayColor,  "blue") == 0)
      {
        sun_color =  GColorBlue;
      } else if(strcmp(setSundayColor,  "electricUltramarine") == 0)
      {
        sun_color =  GColorElectricUltramarine;
      } else if(strcmp(setSundayColor,  "blueMoon") == 0)
      {
        sun_color =  GColorBlueMoon;
      } else if(strcmp(setSundayColor,  "veryLightBlue") == 0)
      {
        sun_color =  GColorVeryLightBlue;
      } else if(strcmp(setSundayColor,  "liberty") == 0)
      {
        sun_color =  GColorLiberty;
      } else if(strcmp(setSundayColor,  "babyBlueEyes") == 0)
      {
        sun_color =  GColorBabyBlueEyes;
      } else if(strcmp(setSundayColor,  "lavenderIndigo") == 0)
      {
        sun_color =  GColorLavenderIndigo;
      } else if(strcmp(setSundayColor,  "vividViolet") == 0)
      {
        sun_color =  GColorVividViolet;
      } else if(strcmp(setSundayColor,  "purpureus") == 0)
      {
        sun_color =  GColorPurpureus;
      } else if(strcmp(setSundayColor,  "shockingPink") == 0)
      {
        sun_color =  GColorShockingPink;
      } else if(strcmp(setSundayColor,  "richBrilliantLavender") == 0)
      {
        sun_color =  GColorRichBrilliantLavender;
      } else if(strcmp(setSundayColor,  "brilliantRose") == 0)
      {
        sun_color =  GColorBrilliantRose;
      } else if(strcmp(setSundayColor,  "magenta") == 0)
      {
        sun_color =  GColorMagenta;
      } else if(strcmp(setSundayColor,  "purple") == 0)
      {
        sun_color =  GColorPurple;
      } else if(strcmp(setSundayColor,  "indigo") == 0)
      {
        sun_color =  GColorIndigo;
      } else if(strcmp(setSundayColor,  "imperialPurple") == 0)
      {
        sun_color =  GColorImperialPurple;
      } else if(strcmp(setSundayColor,  "jazzberryJam") == 0)
      {
        sun_color =  GColorJazzberryJam;
      } else if(strcmp(setSundayColor,  "fashionMagenta") == 0)
      {
        sun_color =  GColorFashionMagenta;
      } else if(strcmp(setSundayColor,  "darkCandyAppleRed") == 0)
      {
        sun_color =  GColorDarkCandyAppleRed;
      } else if(strcmp(setSundayColor,  "bulgarianRose") == 0)
      {
        sun_color =  GColorBulgarianRose;
      } else if(strcmp(setSundayColor,  "roseVale") == 0)
      {
        sun_color =  GColorRoseVale;
      } else if(strcmp(setSundayColor,  "folly") == 0)
      {
        sun_color =  GColorFolly;
      } else if(strcmp(setSundayColor,  "sunsetOrange") == 0)
      {
        sun_color =  GColorSunsetOrange;
      } else if(strcmp(setSundayColor,  "red") == 0)
      {
        sun_color =  GColorRed;
      } else if(strcmp(setSundayColor,  "windsorTan") == 0)
      {
        sun_color =  GColorWindsorTan;
      } else if(strcmp(setSundayColor,  "chromeYellow") == 0)
      {
        sun_color =  GColorChromeYellow;
      } else if(strcmp(setSundayColor,  "orange") == 0)
      {
        sun_color =  GColorOrange;
      } else if(strcmp(setSundayColor,  "melon") == 0)
      {
        sun_color =  GColorMelon;
      } else if(strcmp(setSundayColor,  "rajah") == 0)
      {
        sun_color =  GColorRajah;
      } else if(strcmp(setSundayColor,  "yellow") == 0)
      {
        sun_color =  GColorYellow;
      } else if(strcmp(setSundayColor,  "icterine") == 0)
      {
        sun_color =  GColorIcterine;
      } else if(strcmp(setSundayColor,  "pastelYellow") == 0)
      {
        sun_color =  GColorPastelYellow;
      } else if(strcmp(setSundayColor,  "white") == 0)
      {
        sun_color =  GColorWhite;
      } else if(strcmp(setSundayColor,  "lightGray") == 0)
      {
        sun_color =  GColorLightGray;
      } else if(strcmp(setSundayColor,  "darkGray") == 0)
      {
        sun_color =  GColorDarkGray;
      } else if(strcmp(setSundayColor,  "black") == 0)
      {
        sun_color =  GColorBlack;
      } else { 
         sun_color =  GColorRed;
      }
  
  
  
  
  
    
   // +++++ test layer +++++ 
  test_layer = text_layer_create(GRect(0, 147, 140, 65));
  text_layer_set_background_color(test_layer, GColorClear);
  text_layer_set_text_color(test_layer, time_color);
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
  char setBgColor[25]; 
  persist_read_string(bgColor, setBgColor, sizeof(setBgColor));
  
  if(strcmp(setBgColor, "inchworm") == 0)
      {
        window_set_background_color(my_window, GColorInchworm);
      }else if(strcmp(setBgColor,  "springBud") == 0)
      {
        window_set_background_color(my_window, GColorSpringBud);
      } else if(strcmp(setBgColor,  "brightGreen") == 0)
      {
        window_set_background_color(my_window, GColorBrightGreen);
      } else if(strcmp(setBgColor,  "mintGreen") == 0)
      {
        window_set_background_color(my_window, GColorMintGreen);
      } else if(strcmp(setBgColor,  "screaminGreen") == 0)
      {
        window_set_background_color(my_window, GColorScreaminGreen);
      } else if(strcmp(setBgColor,  "green") == 0)
      {
        window_set_background_color(my_window, GColorGreen);
      } else if(strcmp(setBgColor,  "malachite") == 0)
      {
        window_set_background_color(my_window, GColorMalachite);
      } else if(strcmp(setBgColor,  "islamicGreen") == 0)
      {
        window_set_background_color(my_window, GColorIslamicGreen);
      } else if(strcmp(setBgColor,  "kellyGreen") == 0)
      {
        window_set_background_color(my_window, GColorKellyGreen);
      } else if(strcmp(setBgColor,  "brass") == 0)
      {
        window_set_background_color(my_window, GColorBrass);
      } else if(strcmp(setBgColor,  "limerick") == 0)
      {
        window_set_background_color(my_window, GColorLimerick);
      } else if(strcmp(setBgColor,  "mediumAquamarine") == 0)
      {
        window_set_background_color(my_window, GColorMediumAquamarine);
      } else if(strcmp(setBgColor,  "jaegerGreen") == 0)
      {
        window_set_background_color(my_window, GColorJaegerGreen);
      } else if(strcmp(setBgColor,  "mayGreen") == 0)
      {
        window_set_background_color(my_window, GColorMayGreen);
      } else if(strcmp(setBgColor,  "darkGreen") == 0)
      {
        window_set_background_color(my_window, GColorDarkGreen);
      } else if(strcmp(setBgColor,  "armyGreen") == 0)
      {
        window_set_background_color(my_window, GColorArmyGreen);
      } else if(strcmp(setBgColor,  "mediumSpringGreen") == 0)
      {
        window_set_background_color(my_window, GColorMediumSpringGreen);
      } else if(strcmp(setBgColor,  "tiffanyBlue") == 0)
      {
        window_set_background_color(my_window, GColorTiffanyBlue);
      } else if(strcmp(setBgColor,  "cadetBlue") == 0)
      {
        window_set_background_color(my_window, GColorCadetBlue);
      } else if(strcmp(setBgColor,  "midnightGreen") == 0)
      {
        window_set_background_color(my_window, GColorMidnightGreen);
      } else if(strcmp(setBgColor,  "electricBlue") == 0)
      {
        window_set_background_color(my_window, GColorElectricBlue);
      } else if(strcmp(setBgColor,  "cyan") == 0)
      {
        window_set_background_color(my_window, GColorCyan);
      } else if(strcmp(setBgColor,  "celeste") == 0)
      {
        window_set_background_color(my_window, GColorCeleste);
      } else if(strcmp(setBgColor,  "cobaltBlue") == 0)
      {
        window_set_background_color(my_window, GColorCobaltBlue);
      } else if(strcmp(setBgColor,  "vividCerulean") == 0)
      {
        window_set_background_color(my_window, GColorVividCerulean);
      } else if(strcmp(setBgColor,  "dukeBlue") == 0)
      {
        window_set_background_color(my_window, GColorDukeBlue);
      } else if(strcmp(setBgColor,  "oxfordBlue") == 0)
      {
        window_set_background_color(my_window, GColorOxfordBlue);
      } else if(strcmp(setBgColor,  "pictonBlue") == 0)
      {
        window_set_background_color(my_window, GColorPictonBlue);
      } else if(strcmp(setBgColor,  "blue") == 0)
      {
        window_set_background_color(my_window, GColorBlue);
      } else if(strcmp(setBgColor,  "electricUltramarine") == 0)
      {
        window_set_background_color(my_window, GColorElectricUltramarine);
      } else if(strcmp(setBgColor,  "blueMoon") == 0)
      {
        window_set_background_color(my_window, GColorBlueMoon);
      } else if(strcmp(setBgColor,  "veryLightBlue") == 0)
      {
        window_set_background_color(my_window, GColorVeryLightBlue);
      } else if(strcmp(setBgColor,  "liberty") == 0)
      {
        window_set_background_color(my_window, GColorLiberty);
      } else if(strcmp(setBgColor,  "babyBlueEyes") == 0)
      {
        window_set_background_color(my_window, GColorBabyBlueEyes);
      } else if(strcmp(setBgColor,  "lavenderIndigo") == 0)
      {
        window_set_background_color(my_window, GColorLavenderIndigo);
      } else if(strcmp(setBgColor,  "vividViolet") == 0)
      {
        window_set_background_color(my_window, GColorVividViolet);
      } else if(strcmp(setBgColor,  "purpureus") == 0)
      {
        window_set_background_color(my_window, GColorPurpureus);
      } else if(strcmp(setBgColor,  "shockingPink") == 0)
      {
        window_set_background_color(my_window, GColorShockingPink);
      } else if(strcmp(setBgColor,  "richBrilliantLavender") == 0)
      {
        window_set_background_color(my_window, GColorRichBrilliantLavender);
      } else if(strcmp(setBgColor,  "brilliantRose") == 0)
      {
        window_set_background_color(my_window, GColorBrilliantRose);
      } else if(strcmp(setBgColor,  "magenta") == 0)
      {
        window_set_background_color(my_window, GColorMagenta);
      } else if(strcmp(setBgColor,  "purple") == 0)
      {
        window_set_background_color(my_window, GColorPurple);
      } else if(strcmp(setBgColor,  "indigo") == 0)
      {
        window_set_background_color(my_window, GColorIndigo);
      } else if(strcmp(setBgColor,  "imperialPurple") == 0)
      {
        window_set_background_color(my_window, GColorImperialPurple);
      } else if(strcmp(setBgColor,  "jazzberryJam") == 0)
      {
        window_set_background_color(my_window, GColorJazzberryJam);
      } else if(strcmp(setBgColor,  "fashionMagenta") == 0)
      {
        window_set_background_color(my_window, GColorFashionMagenta);
      } else if(strcmp(setBgColor,  "darkCandyAppleRed") == 0)
      {
        window_set_background_color(my_window, GColorDarkCandyAppleRed);
      } else if(strcmp(setBgColor,  "bulgarianRose") == 0)
      {
        window_set_background_color(my_window, GColorBulgarianRose);
      } else if(strcmp(setBgColor,  "roseVale") == 0)
      {
        window_set_background_color(my_window, GColorRoseVale);
      } else if(strcmp(setBgColor,  "folly") == 0)
      {
        window_set_background_color(my_window, GColorFolly);
      } else if(strcmp(setBgColor,  "sunsetOrange") == 0)
      {
        window_set_background_color(my_window, GColorSunsetOrange);
      } else if(strcmp(setBgColor,  "red") == 0)
      {
        window_set_background_color(my_window, GColorRed);
      } else if(strcmp(setBgColor,  "windsorTan") == 0)
      {
        window_set_background_color(my_window, GColorWindsorTan);
      } else if(strcmp(setBgColor,  "chromeYellow") == 0)
      {
        window_set_background_color(my_window, GColorChromeYellow);
      } else if(strcmp(setBgColor,  "orange") == 0)
      {
        window_set_background_color(my_window, GColorOrange);
      } else if(strcmp(setBgColor,  "melon") == 0)
      {
        window_set_background_color(my_window, GColorMelon);
      } else if(strcmp(setBgColor,  "rajah") == 0)
      {
        window_set_background_color(my_window, GColorRajah);
      } else if(strcmp(setBgColor,  "yellow") == 0)
      {
        window_set_background_color(my_window, GColorYellow);
      } else if(strcmp(setBgColor,  "icterine") == 0)
      {
        window_set_background_color(my_window, GColorIcterine);
      } else if(strcmp(setBgColor,  "pastelYellow") == 0)
      {
        window_set_background_color(my_window, GColorPastelYellow);
      } else if(strcmp(setBgColor,  "white") == 0)
      {
        window_set_background_color(my_window, GColorWhite);
      } else if(strcmp(setBgColor,  "lightGray") == 0)
      {
        window_set_background_color(my_window, GColorLightGray);
      } else if(strcmp(setBgColor,  "darkGray") == 0)
      {
        window_set_background_color(my_window, GColorDarkGray);
      } else {
        window_set_background_color(my_window, GColorBlack);
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
