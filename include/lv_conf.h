/**
 * lv_conf.h - Configurazione minimale per usare LVGL con M5Stack / Cardputer
 */
#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_PERF_MONITOR  1
#define LV_USE_LOG           0

#define LV_COLOR_DEPTH       16
#define LV_COLOR_16_SWAP     0

#define LV_HOR_RES_MAX       240
#define LV_VER_RES_MAX       135

#define LV_USE_DRAW_SW       1
#define LV_USE_LABEL         1
#define LV_USE_BTN           1
#define LV_USE_TEXTAREA      1
#define LV_USE_KEYBOARD      1
#define LV_USE_FLEX          1

#define LV_FONT_MONTSERRAT_14    1
#define LV_FONT_MONTSERRAT_16    1
#define LV_USE_DEMO_WIDGETS      0
#define LV_USE_DEMO_BENCHMARK    0

#endif // LV_CONF_H