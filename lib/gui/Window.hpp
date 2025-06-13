#pragma once
#include <lvgl.h>
#include <memory>

namespace Gui {

class WindowManager;

/// Classe base astratta per tutte le schermate
class Window : public std::enable_shared_from_this<Window> {
public:
    lv_obj_t* root() const { return root_; }
    using Ptr = std::shared_ptr<Window>;
    virtual ~Window() = default;

    /// Costruisci il contenuto dentro `root` (screen appena creato)
    virtual void onBuild(lv_obj_t* root) = 0;

    /// Gestione eventi tastiera / encoder (ritorna true se consumato)
    virtual bool onKey(uint32_t lv_key) { (void)lv_key; return false; }

    /// Mostra la finestra (chiamato solo da WindowManager)
    void show();

    /// Chiude la finestra (chiede al manager di fare pop)
    void close();

protected:
    lv_obj_t* root_ = nullptr;        ///< Screen principale di LVGL
};

} // namespace Gui