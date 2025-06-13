#include "WindowManager.hpp"
#include <lvgl.h>

namespace Gui {

WindowManager& WindowManager::instance() {
    static WindowManager mgr;
    return mgr;
}

void WindowManager::push(Window::Ptr win) {
    if (!win) return;

    if (!stack_.empty()) {
        lv_obj_add_flag(stack_.back()->root(), LV_OBJ_FLAG_HIDDEN);
    }

    stack_.push_back(std::move(win));
    stack_.back()->show();
}

void WindowManager::pop() {
    if (stack_.size() <= 1) return;          // non rimuovere l’ultima

    // distrugge la screen corrente tramite Window::~Window (smart-ptr)
    stack_.pop_back();

    // ri-mostra quella sotto
    lv_obj_clear_flag(stack_.back()->root(), LV_OBJ_FLAG_HIDDEN);
    lv_scr_load(stack_.back()->root());
}

Window::Ptr WindowManager::current() const {
    return stack_.empty() ? nullptr : stack_.back();
}

} // namespace Gui