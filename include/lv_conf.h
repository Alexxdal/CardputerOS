/**
 * lv_conf.h - Configurazione minimale per usare LVGL con M5Stack / Cardputer
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_PERF_MONITOR  0

#define LV_COLOR_DEPTH       16
#define LV_COLOR_16_SWAP     0

#define LV_HOR_RES_MAX       240
#define LV_VER_RES_MAX       135

#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "esp_timer.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR ((esp_timer_get_time() / 1000LL))

#define LV_REFR_PERIOD 10 // 10 ms

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

#define LV_INDEV_DRV_UPDATE_RESULT   0

#define LV_TICK_PERIOD_MS 1

/* DEBUG */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL 1 // LV_LOG_LEVEL_TRACE
#define LV_LOG_PRINTF 1
#define LV_USE_ASSERT_MEM          1   // assetti su allocazioni fallite
#define LV_USE_ASSERT_NULL         1   // punteri nulli
#define LV_USE_ASSERT_STYLE        1   // stili non validi

#endif // LV_CONF_H