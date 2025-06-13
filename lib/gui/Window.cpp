#include "Window.hpp"
#include "WindowManager.hpp"
#include <lvgl.h>

namespace Gui {

static void lv_key_cb(lv_event_t* e) {
    auto* win = static_cast<Window*>(lv_event_get_user_data(e));
    if (!win) return;

    if (lv_event_get_code(e) == LV_EVENT_KEY) {
        uint32_t key = lv_indev_get_key(lv_indev_active());
        if (win->onKey(key)) {
            lv_event_stop_bubbling(e);
        }
    }
}

Window::~Window()
{
    if (timer_) {
        lv_timer_del(timer_);
        timer_ = nullptr;
    }
}

void Window::show() {
    root_ = lv_obj_create(nullptr);           // nuova screen “nuda”
    lv_obj_add_event_cb(root_, lv_key_cb, LV_EVENT_KEY, this);
    onBuild(root_);                           // chiama override

    if (uint32_t p = tickPeriod()) {
        timer_ = lv_timer_create(
            [](lv_timer_t* t) {
                auto* win = static_cast<Window*>(lv_timer_get_user_data(t));
                win->onTick();
            },
            p, this);
    }

    lv_scr_load(root_);                       // mostra
}

void Window::close() {
    WindowManager::instance().pop();          // farà delete via shared_ptr
}

} // namespace Gui