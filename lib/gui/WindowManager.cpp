#include "WindowManager.hpp"
#include "Application.hpp"
#include <lvgl.h>

namespace Gui {

WindowManager& WindowManager::instance() {
    static WindowManager mgr;
    return mgr;
}

void WindowManager::push(Window::Ptr win) {
    if (!win) return;

    stack_.push_back(std::move(win));
    stack_.back()->show();
}

void WindowManager::pop() {
    if (stack_.size() <= 1) return;          // non rimuovere l’ultima

    auto closing = stack_.back();
    auto previous = stack_[stack_.size() - 2];

    lv_scr_load_anim(previous->root(), LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 0, true);

    if (Gui::Application::instance().inputGroup()) {
        lv_group_focus_obj(previous->root());
    }

    closing->detachRoot(); 
    stack_.pop_back();
}

bool WindowManager::dispatchKey(uint32_t key)
{
    if(stack_.empty()) return false;
    return stack_.back()->onKey(key);          // solo la finestra in cima allo stack
}

Window::Ptr WindowManager::current() const {
    return stack_.empty() ? nullptr : stack_.back();
}

} // namespace Gui