// gui_widgets.hpp – widget wrapper layer over LVGL
#pragma once
#include <lvgl.h>
#include <functional>
#include <string>

namespace GUI {

/* ---------------- Base Widget ---------------- */
class Widget {
protected:
    lv_obj_t *obj_{};
public:
    explicit Widget(lv_obj_t *o = nullptr) : obj_(o) {}

    lv_obj_t *raw() const 
    { 
        return obj_; 
    }

    void setSize(int w, int h)
    { 
        lv_obj_set_size(obj_, w, h);
    }

    void align(lv_align_t a, int x=0, int y=0)
    { 
        lv_obj_align(obj_, a, x, y); 
    }
};

/* ---------------- Window ---------------- */
struct WindowConf 
{
    lv_coord_t header_height = 0;
    const char* title = nullptr;
    int bg_color = 0x0f40f2;
    int bg_color_gradient = 0x439cf0;
};

class Window : public Widget {
public:
    lv_obj_t *screen{};   // Schermo di base (lv_scr_act())
    lv_obj_t *container{};  // Contenitore della finestra
    lv_obj_t *window{}; // Finestra LVGL

    Window(const WindowConf &conf) 
    {
        screen = lv_obj_create(NULL);
        window = lv_win_create(screen, conf.header_height);
        if(conf.title)
            lv_win_add_title(window, conf.title);

        // Imposta dimensioni della finestra (FULLSCREEN)
        lv_obj_set_size(window, LV_HOR_RES, LV_VER_RES);
        lv_obj_center(window);  // Centra se necessario
        container = lv_win_get_content(window);
        lv_obj_set_size(container, lv_pct(100), lv_pct(100));
        lv_obj_set_style_pad_all(container, 0, 0);
        lv_obj_set_style_border_width(container, 0, 0);
        lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_bg_color(container, lv_color_hex(conf.bg_color), 0);
        lv_obj_set_style_bg_grad_color(container, lv_color_hex(conf.bg_color_gradient), 0);
    }

    Window(const char *title) 
    {
        window = lv_win_create(lv_scr_act(), 20);
        if(title)
            lv_win_add_title(window, title);
    }

    bool AddItem(lv_obj_t *item) 
    {
        if(container) {
            lv_obj_set_parent(item, container);
            return true;
        }
        return false;
    }

    void show() 
    { 
        lv_scr_load(screen);
    }
};


} // namespace GUI
