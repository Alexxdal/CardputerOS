#include "gui_system.hpp"
#include "gui_widgets.hpp"
#include "navigation.hpp"

using namespace std;
using namespace GUI;
using namespace HAL;

/* Windows */
static Window *test;
static Window *home;

static void test_cb(lv_event_t *e)
{
    if(lv_event_get_code(e) == LV_EVENT_KEY)
	{
        Window *window = (Window *)lv_event_get_user_data(e);
        uint32_t key = lv_indev_get_key(lv_indev_active());
        
        if(key == LV_KEY_ESC)
        {
            if(window->title != "Home")
                home->show();
        }
	}
}

extern "C" void app_main()
{
    GUI::begin();

    home = new Window();
    home->title = "Home";

    test = new Window();
    test->title = "Test";
    test->SetKeyboardCallback(test_cb);
    
    home->show();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}