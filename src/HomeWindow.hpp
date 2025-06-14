#include "Window.hpp"
#include "WindowManager.hpp"
#include <lvgl.h>

namespace Gui {

class HomeWindow : public Gui::Window {
public:
    void onBuild(lv_obj_t* root) override;
    bool onKey(uint32_t key) override;
    void onTick() override;

private:
    void highlight(int idx);              ///< aggiorna lo stile selezione

    lv_obj_t*                list_{};     ///< contenitore dei pulsanti
    std::vector<lv_obj_t*>   items_;      ///< ogni voce della lista
    int                      sel_{0};     ///< indice selezionato
};

}