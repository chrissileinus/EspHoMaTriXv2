#ifndef EHMTX_H
#define EHMTX_H
#include "esphome.h"
#define USE_Fireplugin
#include "esphome/components/time/real_time_clock.h"

const uint8_t MAXQUEUE = 24;
const uint8_t C_RED = 240; // default
const uint8_t C_BLUE = 240;
const uint8_t C_GREEN = 240;
const uint8_t CD_RED = 100; // dim
const uint8_t CD_BLUE = 100;
const uint8_t CD_GREEN = 100;
const uint8_t CA_RED = 200; // alarm
const uint8_t CA_BLUE = 50;
const uint8_t CA_GREEN = 50;
const uint8_t CG_GREY = 50;
const uint8_t C_BLACK = 0;

const uint8_t D_LIFETIME = 5;
const uint8_t D_SCREEN_TIME = 10;

const uint8_t MAXICONS = 90;
const uint8_t BLANKICON = MAXICONS + 1;
const uint8_t TEXTSCROLLSTART = 8;
const uint8_t TEXTSTARTOFFSET = (32 - 8);

const uint16_t POLLINGINTERVAL = 250;
static const char *const EHMTX_VERSION = "2023.9.1";
static const char *const TAG = "EHMTXv2";

enum show_mode : uint8_t
{
  MODE_EMPTY = 0,
  MODE_BLANK = 1,
  MODE_CLOCK = 2,
  MODE_DATE = 3,
  MODE_FULL_SCREEN = 4,
  MODE_ICON_SCREEN = 5,
  MODE_TEXT_SCREEN = 6,
  MODE_RAINBOW_ICON = 7,
  MODE_RAINBOW_TEXT = 8,
  MODE_RAINBOW_CLOCK = 9,
  MODE_RAINBOW_DATE = 10,
  MODE_BITMAP_SCREEN = 11,
  MODE_BITMAP_SMALL = 12,
  MODE_COLOR = 13,
  MODE_FIRE = 14,
  MODE_ICON_CLOCK = 15,
  MODE_ALERT_SCREEN = 16,
  MODE_GRAPH_SCREEN = 17,
  MODE_ICON_DATE = 18,
  MODE_ICON_PROGRESS = 19,
  MODE_RAINBOW_BITMAP_SMALL = 20,
  MODE_ICON_TEXT_SCREEN = 21,
  MODE_RAINBOW_ICON_TEXT_SCREEN = 22,
  MODE_BITMAP_STACK_SCREEN = 23
};

namespace esphome
{
  class EHMTX_queue;
  class EHMTX_Icon;
  class EHMTXNextScreenTrigger;
  class EHMTXEmptyQueueTrigger;
  class EHMTXAddScreenTrigger;
  class EHMTXIconErrorTrigger;
  class EHMTXExpiredScreenTrigger;
  class EHMTXNextClockTrigger;
  class EHMTXStartRunningTrigger;
  class EHMTXShowDisplayTrigger;
  class EHMTXNightModeTrigger;

  class EHMTX : public PollingComponent, public api::CustomAPIDevice
  {
  protected:
    float get_setup_priority() const override { return esphome::setup_priority::LATE; }
    uint8_t brightness_;
    uint8_t target_brightness_;
    uint32_t boot_anim = 0;
    uint8_t screen_pointer;
    bool show_day_of_week;

    std::vector<EHMTXNextScreenTrigger *> on_next_screen_triggers_;
    std::vector<EHMTXEmptyQueueTrigger *> on_empty_queue_triggers_;
    std::vector<EHMTXIconErrorTrigger *> on_icon_error_triggers_;
    std::vector<EHMTXExpiredScreenTrigger *> on_expired_screen_triggers_;
    std::vector<EHMTXNextClockTrigger *> on_next_clock_triggers_;
    std::vector<EHMTXStartRunningTrigger *> on_start_running_triggers_;
    std::vector<EHMTXAddScreenTrigger *> on_add_screen_triggers_;
    std::vector<EHMTXShowDisplayTrigger *> on_show_display_triggers_;
    std::vector<EHMTXNightModeTrigger *> on_night_mode_triggers_;
    EHMTX_queue *find_icon_queue_element(uint8_t icon);
    EHMTX_queue *find_mode_queue_element(uint8_t mode);
    EHMTX_queue *find_mode_icon_queue_element(uint8_t mode, std::string name);
    EHMTX_queue *find_free_queue_element();

  public:
    void setup() override;
    EHMTX();

#ifdef USE_Fireplugin
    void fire_screen( int lifetime, int screen_time);
#endif    
    uint16_t hue_ = 0;
    void dump_config();
    bool info_font = true;
    int8_t info_y_offset = 0;
#ifdef USE_ESP32
    PROGMEM Color text_color, alarm_color, rindicator_color, lindicator_color, today_color, weekday_color, rainbow_color, clock_color, info_lcolor, info_rcolor;
    PROGMEM Color bitmap[256];
    PROGMEM Color cgauge[8];
    PROGMEM EHMTX_Icon *icons[MAXICONS];
#endif

#ifdef USE_ESP8266
    Color text_color, alarm_color, gauge_color, gauge_bgcolor,rindicator_color,lindicator_color, today_color, weekday_color, rainbow_color, clock_color, info_lcolor, info_rcolor;
    EHMTX_Icon *icons[MAXICONS];
    uint8_t gauge_value;
#endif
    display::BaseFont *default_font;
    display::BaseFont *special_font;
    int display_rindicator;
    int display_lindicator;
    int display_alarm;
    uint8_t ticks_per_second=62;
    bool display_gauge;
    bool is_running = false;
        
    uint16_t clock_time;
    uint16_t scroll_step;

    EHMTX_queue *queue[MAXQUEUE];
    addressable_light::AddressableLightDisplay *display;
    esphome::time::RealTimeClock *clock;
    #ifdef USE_GRAPH
      void graph_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);
      void icon_graph_screen(std::string icon, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);
      graph::Graph *graph;
    #endif

    bool show_seconds;

    uint8_t icon_count; // max iconnumber -1
    unsigned long last_scroll_time;
    unsigned long last_rainbow_time;
    unsigned long last_anim_time;
    float next_action_time = 0.0; // when is the next screen change
    uint32_t tick_next_action = 0; // when is the next screen change
    uint32_t ticks_ = 0; // when is the next screen change

    void remove_expired_queue_element();
    uint8_t find_oldest_queue_element();
    uint8_t queue_count();
    uint8_t find_icon_in_queue(std::string);
    void force_screen(std::string name, int mode = MODE_ICON_SCREEN);
    void add_icon(EHMTX_Icon *icon);
    bool show_display = false;
    bool night_mode = false;
    uint8_t find_icon(std::string name);
    uint8_t find_last_clock();
    bool string_has_ending(std::string const &fullString, std::string const &ending);
    void draw_day_of_week(bool small=false);
    void show_all_icons();
    float get_tick();
    void tick();
    void draw();
    void get_status();
    void queue_status();
    void skip_screen();
    void hold_screen(int t = 30);
    void set_display(addressable_light::AddressableLightDisplay *disp);
    void set_clock_time(uint16_t t = 10);
    void set_show_day_of_week(bool b=true);
    void set_show_seconds(bool b=false);
    void set_brightness(int b);
    void set_display_on();
    void set_display_off();
    void set_night_mode_off();
    void set_night_mode_on();
    void set_clock(esphome::time::RealTimeClock *clock);
    #ifdef USE_GRAPH
      void set_graph(esphome::graph::Graph *graph);
    #endif
    void set_default_font(display::BaseFont *font);
    void set_special_font(display::BaseFont *font);

    void show_rindicator(int r = C_RED, int g = C_GREEN, int b = C_BLUE, int s = 3);
    void show_lindicator(int r = C_RED, int g = C_GREEN, int b = C_BLUE, int s = 3);
    void set_text_color(int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void set_today_color(int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void set_weekday_color(int r = CD_RED, int g = CD_GREEN, int b = CD_BLUE);
    void set_clock_color(int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void set_infotext_color(int lr = CG_GREY, int lg = CG_GREY, int lb = CG_GREY, int rr = CG_GREY, int rg = CG_GREY, int rb = CG_GREY, bool info_font = true, int y_offset = 0);

    void show_alarm(int r = CA_RED, int g = CA_GREEN, int b = CA_BLUE, int s = 2);
    void show_gauge(int v, int r = C_RED, int g = C_GREEN, int b = C_BLUE,int bgr = CG_GREY, int bgg = CG_GREY, int bgb = CG_GREY); // int because of register_service
    void hide_gauge();
    void hide_rindicator();
    void hide_lindicator();
    void hide_alarm();
    void full_screen(std::string icon, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);
    void icon_screen(std::string icon, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void alert_screen(std::string icon, std::string text, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = CA_RED, int g = CA_GREEN, int b = CA_BLUE);
    void icon_clock(std::string icon, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void icon_date(std::string icon, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void text_screen(std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void clock_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void date_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void blank_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);
    void color_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void icon_screen_progress(std::string icon, std::string text, int progress, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void set_progressbar_color(std::string icon, int r = C_BLACK, int g = C_BLACK, int b = C_BLACK, int bg_r = C_BLACK, int bg_g = C_BLACK, int bg_b = C_BLACK);

    void icon_text_screen(std::string icon, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void rainbow_icon_text_screen(std::string icon, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);

    void bitmap_stack(std::string icons, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);

    void bitmap_screen(std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME);
    void color_gauge(std::string text);
    void bitmap_small(std::string icon, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true, int r = C_RED, int g = C_GREEN, int b = C_BLUE);
    void rainbow_bitmap_small(std::string icon, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);
    void rainbow_icon_screen(std::string icon_name, std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);
    void rainbow_text_screen(std::string text, int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);
    void rainbow_clock_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);
    void rainbow_date_screen(int lifetime = D_LIFETIME, int screen_time = D_SCREEN_TIME, bool default_font = true);
    void del_screen(std::string icon, int mode = MODE_ICON_SCREEN);

    void draw_gauge();
    void draw_alarm();
    void draw_rindicator();
    void draw_lindicator();

    void set_replace_time_date_active(bool b=false);
    void set_weekday_char_count(uint8_t i);
    bool replace_time_date_active;
    std::string replace_time_date(std::string time_date);
    uint8_t weekday_char_count;
    std::string GetWeekdayChar(int position);

    int GetTextBounds(esphome::display::BaseFont *font, const char *buffer);
    int GetTextWidth(esphome::display::BaseFont *font, const char* formatting, const char raw_char);
    int GetTextWidth(esphome::display::BaseFont *font, const char* formatting, const char *raw_text);
    int GetTextWidth(esphome::display::BaseFont *font, const char* formatting, const int raw_int);
    int GetTextWidth(esphome::display::BaseFont *font, const char* formatting, const float raw_float);
    int GetTextWidth(esphome::display::BaseFont *font, const char* formatting, esphome::ESPTime time);

    void add_on_next_screen_trigger(EHMTXNextScreenTrigger *t) { this->on_next_screen_triggers_.push_back(t); }
    void add_on_empty_queue_trigger(EHMTXEmptyQueueTrigger *t) { this->on_empty_queue_triggers_.push_back(t); }
    void add_on_add_screen_trigger(EHMTXAddScreenTrigger *t) { this->on_add_screen_triggers_.push_back(t); }
    void add_on_icon_error_trigger(EHMTXIconErrorTrigger *t) { this->on_icon_error_triggers_.push_back(t); }
    void add_on_expired_screen_trigger(EHMTXExpiredScreenTrigger *t) { this->on_expired_screen_triggers_.push_back(t); }
    void add_on_next_clock_trigger(EHMTXNextClockTrigger *t) { this->on_next_clock_triggers_.push_back(t); }
    void add_on_start_running_trigger(EHMTXStartRunningTrigger *t) { this->on_start_running_triggers_.push_back(t); }
    void add_on_show_display_trigger(EHMTXShowDisplayTrigger *t) { this->on_show_display_triggers_.push_back(t); }
    void add_on_night_mode_trigger(EHMTXNightModeTrigger *t) { this->on_night_mode_triggers_.push_back(t); }
    void update();

    uint8_t get_brightness();
  };

  class EHMTX_queue
  {
  protected:
    EHMTX *config_;

  public:
    uint16_t pixels_;
    float screen_time_;
    bool default_font;
    float endtime;
    float last_time;
    uint8_t icon;
    uint16_t scroll_reset;
    show_mode mode;
    int8_t progress;
    Color* sbitmap;

#ifdef USE_ESP32
    PROGMEM Color text_color, progressbar_color, progressbar_back_color;
    PROGMEM std::string text;
    PROGMEM std::string icon_name;
#endif
#ifdef USE_ESP8266
    Color text_color, progressbar_color, progressbar_back_color;
    std::string text;
    std::string icon_name;
#endif

    EHMTX_queue(EHMTX *config);
    Color heatColor(uint8_t temperature);
    void status();
    void draw();
    bool isfree();
    bool update_slot(uint8_t _icon);
    void update_screen();
    void hold_slot(uint8_t _sec);
    void calc_scroll_time(std::string, uint16_t);
    int xpos();
    int xpos(uint8_t item);
  };

  class EHMTXNextScreenTrigger : public Trigger<std::string, std::string>
  {
  public:
    explicit EHMTXNextScreenTrigger(EHMTX *parent) { parent->add_on_next_screen_trigger(this); }
    void process(std::string, std::string);
  };

  class EHMTXEmptyQueueTrigger : public Trigger<>
  {
  public:
    explicit EHMTXEmptyQueueTrigger(EHMTX *parent) { parent->add_on_empty_queue_trigger(this); }
    void process();
  };

  class EHMTXAddScreenTrigger : public Trigger<std::string, uint8_t>
  {
  public:
    explicit EHMTXAddScreenTrigger(EHMTX *parent) { parent->add_on_add_screen_trigger(this); }
    void process(std::string, uint8_t);
  };

  class EHMTXIconErrorTrigger : public Trigger<std::string>
  {
  public:
    explicit EHMTXIconErrorTrigger(EHMTX *parent) { parent->add_on_icon_error_trigger(this); }
    void process(std::string);
  };

 class EHMTXStartRunningTrigger : public Trigger<>
  {
  public:
    explicit EHMTXStartRunningTrigger(EHMTX *parent) { parent->add_on_start_running_trigger(this); }
    void process();
  };

  class EHMTXExpiredScreenTrigger : public Trigger<std::string, std::string>
  {
  public:
    explicit EHMTXExpiredScreenTrigger(EHMTX *parent) { parent->add_on_expired_screen_trigger(this); }
    void process(std::string, std::string);
  };

  class EHMTXNextClockTrigger : public Trigger<>
  {
  public:
    explicit EHMTXNextClockTrigger(EHMTX *parent) { parent->add_on_next_clock_trigger(this); }
    void process();
  };

  class EHMTXShowDisplayTrigger : public Trigger<bool>
  {
  public:
    explicit EHMTXShowDisplayTrigger(EHMTX *parent) { parent->add_on_show_display_trigger(this); }
    void process(bool);
  };

  class EHMTXNightModeTrigger : public Trigger<bool>
  {
  public:
    explicit EHMTXNightModeTrigger(EHMTX *parent) { parent->add_on_night_mode_trigger(this); }
    void process(bool);
  };

  class EHMTX_Icon : public animation::Animation
  {
  protected:
    bool counting_up;

  public:
    EHMTX_Icon(const uint8_t *data_start, int width, int height, uint32_t animation_frame_count, esphome::image::ImageType type, std::string icon_name, bool revers, uint16_t frame_duration);
#ifdef USE_ESP32
    PROGMEM std::string name;
#endif
#ifdef USE_ESP8266
    std::string name;
#endif
    uint16_t frame_duration;
    void next_frame();
    bool reverse;
  };
}

#endif
