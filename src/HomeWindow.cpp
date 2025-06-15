#include "HomeWindow.hpp"
#include "TestWindow.hpp"
#include "Application.hpp"

using namespace std;

namespace Gui {

lv_color_t BG      = lv_color_hex(0x101010);
lv_color_t NORMAL  = lv_color_hex(0x303030);
lv_color_t SELECT  = lv_color_hex(0x5050ff);

static void btn_focus(lv_event_t* e)
{
    auto* btn = static_cast<lv_obj_t*>(lv_event_get_user_data(e));
    lv_obj_set_style_bg_color(btn, SELECT, LV_PART_MAIN);
}

static void btn_defocus(lv_event_t* e)
{
    auto* btn = static_cast<lv_obj_t*>(lv_event_get_user_data(e));
    lv_obj_set_style_bg_color(btn, NORMAL, LV_PART_MAIN);
}

static void btn_click(lv_event_t* e)
{
    auto* btn = static_cast<lv_obj_t*>(lv_event_get_user_data(e));
    lv_obj_t* btn_text = lv_obj_get_child_by_type(btn, 0, &lv_label_class);

    if(lv_strcmp(lv_label_get_text(btn_text), "File Manager") == 0)
    {
        Gui::WindowManager::instance().push(std::make_shared<TestWindow>());
    }
}

void HomeWindow::onBuild(lv_obj_t* root)
{
    lv_obj_set_style_bg_color(root, BG, LV_PART_MAIN);
    lv_obj_center(root);
    // titolo
    auto* title = lv_label_create(root);
    lv_label_set_text(title, "Home");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 4);

    // lista (colonna verticale)
    list_ = lv_obj_create(root);
    lv_obj_set_size(list_, 200, 140);
    lv_obj_set_style_pad_all(list_, 0, 0);
    lv_obj_set_style_border_width(list_, 0, 0);
    lv_obj_set_flex_flow(list_, LV_FLEX_FLOW_COLUMN);
    lv_obj_align(list_, LV_ALIGN_CENTER, 0, 10);

    /* voci */
    const char* labels[] = { "File Manager", "Settings", "About", "Reboot" };
    for (const char* txt : labels) {
        auto* btn = lv_btn_create(list_);
        lv_obj_set_size(btn, 200, 32);
        lv_obj_set_style_bg_color(btn, NORMAL, LV_PART_MAIN);
        auto* l  = lv_label_create(btn);
        lv_label_set_text(l, txt);
        lv_obj_center(l);
        items_.push_back(btn);
        lv_obj_add_event_cb(btn, btn_focus, LV_EVENT_FOCUSED, btn);
        lv_obj_add_event_cb(btn, btn_defocus, LV_EVENT_DEFOCUSED, btn);
        lv_obj_add_event_cb(btn, btn_click, LV_EVENT_CLICKED, btn);
    }
}


void HomeWindow::onTick()
{
    static int dd = 0;
    lv_obj_t* tmp = lv_obj_get_child_by_type(items_.back(), 0, &lv_label_class);
    lv_label_set_text(tmp, std::to_string(dd).c_str());
    dd++;
}

}