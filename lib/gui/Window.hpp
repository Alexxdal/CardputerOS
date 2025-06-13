#pragma once
#include <lvgl.h>
#include <memory>

namespace Gui {

class WindowManager;

/// Classe base astratta per tutte le schermate
class Window : public std::enable_shared_from_this<Window> {
public:
    using Ptr = std::shared_ptr<Window>;
    virtual ~Window();

    /// Costruisci il contenuto dentro `root` (screen appena creato)
    virtual void onBuild(lv_obj_t* root) = 0;

    /// Gestione eventi tastiera / encoder (ritorna true se consumato)
    virtual bool onKey(uint32_t lv_key) { (void)lv_key; return false; }

    /// Mostra la finestra (chiamato solo da WindowManager)
    void show();

    virtual uint32_t tickPeriod() const { return 500; }   // 0 = nessun tick
    virtual void     onTick() {}  

    /// Chiude la finestra (chiede al manager di fare pop)
    void close();

    lv_obj_t* root() const { return root_; }

protected:
    lv_obj_t*   root_{nullptr};
    lv_timer_t* timer_{nullptr};
};

} // namespace Gui