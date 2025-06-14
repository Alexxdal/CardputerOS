#include "Window.hpp"
#include "WindowManager.hpp"
#include "Application.hpp"
#include <freertos/FreeRTOS.h>
#include <lvgl.h>
#include <string>

#include "HomeWindow.hpp"

using namespace Gui;

static void gui_task(void*)
{
    Gui::Application::instance().run(std::make_shared<HomeWindow>());
    vTaskDelete(nullptr);
}

extern "C" void app_main()
{
    /* 16 kB stack, priorità 5, core 1 (facoltativo) */
    xTaskCreatePinnedToCore(gui_task, "gui", 16384, nullptr, 5, nullptr, 1);
}