#include "Window.hpp"
#include "WindowManager.hpp"
#include <lvgl.h>

namespace Gui {

class TestWindow : public Gui::Window {
public:
    void onBuild(lv_obj_t* root) override;
    bool onKey(uint32_t key) override;
    void onTick() override;

private:
    void highlight(int idx);              ///< aggiorna lo stile selezione
};

}