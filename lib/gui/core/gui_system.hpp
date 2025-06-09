#pragma once
#include <lvgl.h>
#include <M5GFX.h>
#include <hal_cardputer.h>

namespace GUI {

bool begin();

void shutdown();

HAL::HalCardputer *hal();

}