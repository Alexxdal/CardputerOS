#include "TestWindow.hpp"

namespace Gui {

void TestWindow::onBuild(lv_obj_t* root)
{
    lv_obj_set_style_bg_color(root, lv_color_hex(0x101010), LV_PART_MAIN);
    lv_obj_center(root);
    // titolo
    auto* title = lv_label_create(root);
    lv_label_set_text(title, "Test");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 4);
}


void TestWindow::onTick()
{
    static int dd = 0;
    lv_obj_t* tmp = lv_obj_get_child_by_type(root_, 0, &lv_label_class);
    lv_label_set_text(tmp, std::to_string(dd).c_str());
    dd++;
}


bool TestWindow::onKey(uint32_t key)
{
    switch (key) {
    case 96:
        close();                 // chiede al manager di fare pop()
        return true;             // evento gestito

    default:
        return false;
    }
}

}