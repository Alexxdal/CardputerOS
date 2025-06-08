#pragma once
#include <lvgl.h>
#include <functional>
#include <string>
#include "gui_widgets.hpp"
#include "button.hpp"

namespace GUI {

struct DialogConf {
    lv_coord_t  width   = 200;
    lv_coord_t  height  = 120;
    uint16_t    radius  = 12;
    lv_color_t  bg      = lv_color_hex(0x303030);
    lv_color_t  border  = lv_color_white();
    uint8_t     border_w= 2;
    const char* title   = "";            // barra titolo lv_win
    const char* msg     = "Message";
    const char* ok_txt  = "OK";
    const char* cancel_txt = "Cancel";
};

class Dialog : public Window {
public:
    lv_obj_t *btn_ok {nullptr};
    lv_obj_t *btn_cancel {nullptr};

    Dialog(const DialogConf& c,
           std::function<void()> onOk     = {},
           std::function<void()> onCancel = {}) : Window(c.title)
    {
        lv_obj_set_size(raw(), c.width, c.height);
        lv_obj_center(raw());

        /* stile */
        lv_obj_set_style_radius      (raw(), c.radius, 0);
        lv_obj_set_style_clip_corner (raw(), true, 0);
        lv_obj_set_style_bg_color    (raw(), c.bg, 0);
        lv_obj_set_style_border_color(raw(), c.border, 0);
        lv_obj_set_style_border_width(raw(), c.border_w, 0);

        /* testo / icona / ecc. */
        lv_obj_t *lbl = lv_label_create(raw());
        lv_label_set_text(lbl, c.msg);
        lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 16);

        /* pulsante OK */
        btn_ok = lv_btn_create(raw());
        lv_obj_t *l_ok = lv_label_create(btn_ok);
        lv_label_set_text(l_ok, c.ok_txt);
        lv_obj_center(l_ok);
        lv_obj_align(btn_ok, LV_ALIGN_BOTTOM_LEFT, 20, -12);
        lv_obj_add_event_cb(btn_ok, [](lv_event_t *e){
            auto f = static_cast<std::function<void()>*>(lv_event_get_user_data(e));
            if (*f) (*f)();
            lv_obj_del(lv_scr_act());
        }, LV_EVENT_CLICKED, new std::function<void()>(onOk));

        /* pulsante Cancel */
        btn_cancel = lv_btn_create(raw());
        lv_obj_t *l_ca = lv_label_create(btn_cancel);
        lv_label_set_text(l_ca, c.cancel_txt);
        lv_obj_center(l_ca);
        lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_RIGHT, -20, -12);
        lv_obj_add_event_cb(btn_cancel, [](lv_event_t *e)
        {
            if(e) {
                auto f = static_cast<std::function<void()>*>(lv_event_get_user_data(e));
                if (*f) (*f)();
            }
            lv_obj_del(lv_scr_act());
        }, LV_EVENT_CLICKED, new std::function<void()>(onCancel));
    }
};

}