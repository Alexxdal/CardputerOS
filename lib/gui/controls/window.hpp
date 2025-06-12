#pragma once
#include <lvgl.h>
#include <functional>
#include <string>

using namespace std;

namespace GUI {
    class Window {
    private:
        lv_obj_t *screen;
    
    public:
        lv_obj_t *container;
        lv_obj_t *window;
        lv_obj_t *header;

        /**** Config ****/
        lv_coord_t header_height = 0;
        string title = "";
        int bg_color = 0x0f40f2;
        int bg_color_gradient = 0x439cf0;
        uint32_t animation_time = 300;
        /****************/

        Window();
        ~Window();
        void SetKeyboardCallback(lv_event_cb_t callback)
        {
            if(callback && window)
                lv_obj_add_event_cb(window, callback, LV_EVENT_KEY, this);
        }

        bool AddItem(lv_obj_t *item) {
            if(container) {
                lv_obj_set_parent(item, container);
                return true;
            }
            return false;
        }
        void show() { 
            lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_ON, animation_time, 0, false);
        }
    };

    Window::Window()
    {
        screen = lv_obj_create(nullptr);
        window = lv_win_create(screen);
        lv_obj_set_size(window, LV_HOR_RES, LV_VER_RES);
        lv_obj_center(window);
        container = lv_win_get_content(window);
        header = lv_win_get_header(window);
        lv_obj_set_size(container, lv_pct(100), lv_pct(100));
        lv_obj_set_size(header, 0, 0);
        lv_obj_set_style_pad_all(container, 0, 0);
        lv_obj_set_style_border_width(container, 0, 0);
        lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_bg_color(container, lv_color_hex(bg_color), 0);
        lv_obj_set_style_bg_grad_color(container, lv_color_hex(bg_color_gradient), 0);

        if(lv_group_t *g = lv_group_get_default())
            lv_group_add_obj(g, window);
    }

    Window::~Window() {
        if(screen)
        {
            lv_obj_delete_async(screen);
        }
    }
}