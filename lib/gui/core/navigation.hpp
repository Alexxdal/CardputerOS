#pragma once
#include <lvgl.h>
#include <functional>
#include <string>
#include <vector>
#include "window.hpp"

using namespace std;
using namespace GUI;

namespace Navigation {

    static vector<Window*> stack;

    Window *getCurrentWindow(void)
    {
        return stack.back();
    }

    void push(Window *win)
    {
        stack.push_back(win);
        win->show();
    }

    void pop(void)
    {
        /* Get current window */
        Window *cur_win = stack.back();
        /* Remove deleted win from stack*/
        stack.pop_back();
        /* Show previous win */
        Window *prev_win = stack.back();
        prev_win->show();
        /* Delete last screen */
        delete(cur_win);
    }
}