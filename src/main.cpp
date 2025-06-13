#include "Window.hpp"
#include "WindowManager.hpp"
#include "Application.hpp"
#include <freertos/FreeRTOS.h>
#include <lvgl.h>
#include <string>



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


lv_color_t BG      = lv_color_hex(0x101010);
lv_color_t NORMAL  = lv_color_hex(0x303030);
lv_color_t SELECT  = lv_color_hex(0x5050ff);

/* -------------------- layout ------------------------------------------ */
void HomeWindow::onBuild(lv_obj_t* root)
{
    lv_obj_set_style_bg_color(root, BG, LV_PART_MAIN);

    // titolo
    auto* title = lv_label_create(root);
    lv_label_set_text(title, "Home");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 4);

    // lista (colonna verticale)
    list_ = lv_obj_create(root);
    lv_obj_set_size(list_, 200, 140);
    lv_obj_set_style_pad_all(list_, 0, 0);
    lv_obj_set_style_border_width(list_, 0, 0);
    lv_obj_set_flex_flow(list_, LV_FLEX_FLOW_COLUMN);
    lv_obj_align(list_, LV_ALIGN_CENTER, 0, 10);

    /* voci */
    const char* labels[] = { "File Manager", "Settings", "About", "Reboot" };
    for (const char* txt : labels) {
        auto* btn = lv_btn_create(list_);
        lv_obj_set_size(btn, 200, 32);
        lv_obj_set_style_bg_color(btn, NORMAL, LV_PART_MAIN);
        auto* l  = lv_label_create(btn);
        lv_label_set_text(l, txt);
        lv_obj_center(l);

        items_.push_back(btn);
    }

    highlight(sel_);                       // evidenzia la prima voce
}

static int dd = 0;
void HomeWindow::onTick()
{
    lv_obj_t* tmp = lv_obj_get_child_by_type(items_.back(), 0, &lv_label_class);
    lv_label_set_text(tmp, std::to_string(dd).c_str());
    dd++;
}

/* -------------------- tastiera ---------------------------------------- */
bool HomeWindow::onKey(uint32_t key)
{
    switch (key) {
    case LV_KEY_DOWN:
        if (sel_ + 1 < static_cast<int>(items_.size())) {
            ++sel_;
            highlight(sel_);
        }
        return true;

    case LV_KEY_UP:
        if (sel_ > 0) {
            --sel_;
            highlight(sel_);
        }
        return true;

    case LV_KEY_ENTER:
        // azioni demo sulle singole voci
        if (sel_ == 0) {
            /* apri File Manager */
            // Gui::WindowManager::instance().push(std::make_shared<FileWindow>());
        } else if (sel_ == 1) {
            /* Settings */
        } else if (sel_ == 2) {
            /* About */
        } else if (sel_ == 3) {
            /* Reboot */
        }
        return true;
    
    case 96:
        close();                 // chiede al manager di fare pop()
        return true;             // evento gestito

    default:
        return false;
    }
}

/* -------------------- utilità ---------------------------------------- */
void HomeWindow::highlight(int idx)
{
    for (size_t i = 0; i < items_.size(); ++i) {
        lv_obj_set_style_bg_color(items_[i],
                                  i == static_cast<size_t>(idx) ? SELECT : NORMAL,
                                  LV_PART_MAIN);
    }

    /* assicura che la riga visibile resti sempre nel display */
    lv_obj_scroll_to_view_recursive(items_[idx], LV_ANIM_OFF);
}

static void gui_task(void*)
{
    Gui::Application::instance().run(std::make_shared<HomeWindow>());
    vTaskDelete(nullptr);
}

extern "C" void app_main()
{
    /* 16 kB stack, priorità 5, core 1 (facoltativo) */
    xTaskCreatePinnedToCore(gui_task, "gui", 16384, nullptr, 5, nullptr, 1);
}