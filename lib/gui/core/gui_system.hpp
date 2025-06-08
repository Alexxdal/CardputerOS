#pragma once
#include <lvgl.h>
#include <M5GFX.h>
#include "M5Cardputer.h"

namespace GUI {

bool begin();

void shutdown();

/* Utilities                                                          */
lv_disp_t *display();      // display LVGL registrato
M5GFX     &gfx();          // alias rapido a M5Cardputer.Display

}