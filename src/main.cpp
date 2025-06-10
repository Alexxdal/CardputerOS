#include "gui_system.hpp"
#include "gui_widgets.hpp"

using namespace GUI;
using namespace HAL;

static void lv_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        uint8_t bat = hal()->getBatLevel();
    	printf("Batt: %d\n", bat);
    }
}

extern "C" void app_main()
{
    
    GUI::begin();
    WindowConf conf = {
        .header_height = 20, // Altezza dell'intestazione della finestra
        .title = "Demo Window",
        .bg_color = 0x0f40f2, // Colore di sfondo
        .bg_color_gradient = 0x439cf0, // Colore di sfondo gradiente
    };
    Window *win = new Window(conf);
    Carousel car(win);
    car.addItem("Clock", &lv_event_cb);
    car.addItem("Settings", lv_event_cb);
    car.addItem("Apps", lv_event_cb);
    car.addItem("Info", lv_event_cb);
    win->show();
    car.focus(0);

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}