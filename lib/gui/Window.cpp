#include "Window.hpp"
#include "WindowManager.hpp"
#include "Application.hpp"
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
    if(root_ && lv_obj_is_valid(root_)) {
        lv_obj_delete(root_);
    }
}

void Window::show() {
    root_ = lv_obj_create(nullptr);           // nuova screen “nuda”
    lv_obj_add_event_cb(root_, lv_key_cb, LV_EVENT_KEY, this);
    onBuild(root_);                           // chiama override

    auto* grp = Gui::Application::instance().inputGroup();
    if (grp) {
        lv_group_add_obj(grp, root_);         // metti la nuova screen nel gruppo
        lv_group_focus_obj(root_);            // e falla diventare la focalizzata
    }

    if (uint32_t p = tickPeriod()) {
        timer_ = lv_timer_create(
            [](lv_timer_t* t) {
                auto* win = static_cast<Window*>(lv_timer_get_user_data(t));
                win->onTick();
            },
            p, this);
    }

    lv_scr_load(root_);
}

void Window::close() {
    lv_obj_remove_event_cb(root_, lv_key_cb);
    lv_timer_delete(timer_);
    Gui::WindowManager::instance().pop();
}

} // namespace Gui