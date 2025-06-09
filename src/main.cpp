#include "gui_system.hpp"
#include "gui_widgets.hpp"
#include "dialog.hpp"
#include "carousel.hpp"

extern "C" void app_main()
{
    using namespace GUI;
    using namespace HAL;
    GUI::begin();

    WindowConf conf = {
        .header_height = 20, // Altezza dell'intestazione della finestra
        .title = "Demo Window",
        .bg_color = 0x0f40f2, // Colore di sfondo
        .bg_color_gradient = 0x439cf0, // Colore di sfondo gradiente
    };
    Window win(conf);
    Carousel car(win);

    car.addItem("Clock", [](lv_event_t *){ printf("Clock\n"); });
    car.addItem("Settings",[](lv_event_t *){ printf("Settings\n"); });
    car.addItem("Apps", [](lv_event_t *){ printf("Apps\n"); });
    car.addItem("Info", [](lv_event_t *){ printf("Info\n"); });

    win.show();
    car.focus(0); // Focalizza il primo elemento del carosello

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}