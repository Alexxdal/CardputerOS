#pragma once
#include <lvgl.h>
#include <functional>
#include <string>
#include "gui_widgets.hpp"

namespace GUI {

class Button : public Widget {
public:
    Button(lv_obj_t *parent, const char *text) 
    {
        obj_ = lv_btn_create(parent);
        lv_obj_t *l = lv_label_create(obj_);
        lv_label_set_text(l, text);
        lv_obj_center(l);
    }

    template <typename F>
    void onClick(F &&cb) 
    {
        auto *fn = new std::function<void()>(std::forward<F>(cb));
        lv_obj_add_event_cb(obj_, [](lv_event_t *e)
        {
            auto *fn = static_cast<std::function<void()>*>(lv_event_get_user_data(e));
            (*fn)();
        }, LV_EVENT_CLICKED, fn);
    }
};

}