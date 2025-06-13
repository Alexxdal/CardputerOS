#pragma once
#include <lvgl.h>
#include <memory>

namespace Gui {

/// Smart-pointer RAII per oggetti lv_obj_t*
struct LvDeleter {
    void operator()(lv_obj_t* o) const noexcept {
        if (o) lv_obj_delete(o);
    }
};

using LvPtr = std::unique_ptr<lv_obj_t, LvDeleter>;

} // namespace Gui