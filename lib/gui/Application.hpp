#pragma once
#include <lvgl.h>
#include <memory>

namespace Gui {

class Window;          // forward

/// Boot-strap di tutta la GUI + loop LVGL
class Application {
public:
    static Application& instance();

    /// Entra nel loop principale mostrando la prima finestra
    void run(std::shared_ptr<Window> first);

    /// Fa uscire il loop (es. da un pulsante “Quit”)
    void quit() { quit_requested_ = true; }

    /// Accesso al gruppo d’input (encoder / tastiera) se serve
    lv_group_t* inputGroup() const { return kb_group; }

private:
    Application() = default;
    void init_hal();
    void init_lvgl();          // init display, input, theme
    void start_task();         // crea il task FreeRTOS (separato dal main)

    bool            quit_requested_{false};
    lv_display_t*   lvDisplay{nullptr};
    lv_indev_t*     lvInput{nullptr};
    lv_group_t*     kb_group{nullptr};
};

} // namespace Gui