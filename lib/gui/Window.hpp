#pragma once
#include <lvgl.h>
#include <memory>

namespace Gui {

class WindowManager;

/// Classe base astratta per tutte le schermate
class Window : public std::enable_shared_from_this<Window> {
public:
    /**
     * @brief std::shared_ptr<Gui::Window> is a smart pointer template specialization that provides reference-counted
     * ownership semantics for managing Gui::Window objects. It ensures proper resource management by automatically 
     * deleting the managed object when the last shared_ptr owning it is destroyed or reset.
     */
    using Ptr = std::shared_ptr<Window>;

    virtual ~Window();

    /**
     * @brief The Gui::Window::onBuild function is a virtual method that is intended to be overridden in derived classes.
     * It takes a pointer to an lv_obj_t object named root and is likely used to define or customize the construction of 
     * a graphical user interface window within the LittlevGL framework.
     */
    virtual void onBuild(lv_obj_t* root) = 0;

    /**
     * @brief The Gui::Window::onKey method is an inline virtual function that handles keyboard or encoder events,
     * taking a uint32_t key as input. It returns a boolean value indicating whether the event was consumed,
     * but the default implementation ignores the input and always returns false.
     */
    virtual bool onKey(uint32_t lv_key) { (void)lv_key; return false; }

    /**
     * @brief The Gui::Window::show function is a member of the Gui::Window class and is responsible
     * for displaying the window. It is exclusively called by the WindowManager.
     */
    void show();

    virtual uint32_t tickPeriod() const { return 500; }   // 0 = nessun tick
    virtual void onTick() {}  

    /**
     * @brief The Gui::Window::close function is a member of the 
     * Gui::Window class that requests the manager to remove or "pop" the window, effectively closing it.
     */
    void close();

    lv_obj_t* root() const { return root_; }
    void detachRoot() { root_ = nullptr; }

protected:
    lv_obj_t*   root_{nullptr};
    lv_timer_t* timer_{nullptr};
};

} // namespace Gui