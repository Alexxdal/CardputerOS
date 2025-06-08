#pragma once
#include <lvgl.h>
#include <vector>
#include <string>
#include "gui_widgets.hpp"

namespace GUI {

/**
 * Carousel – contenitore orizzontale scrollabile che centra
 * automaticamente l’elemento che riceve il focus.
 * Nessun callback esplicito: usiamo il flag LV_OBJ_FLAG_SCROLL_ON_FOCUS.
 */
class Carousel {
    lv_obj_t *container{};                     // contenitore scrollabile
    std::vector<lv_obj_t*> items_;         // bottoni/icone

public:
    /**
     * @param win      finestra LVGL che ospita il carosello
     * @param item_sz  lato (px) dei bottoni quadrati
     * @param gap      spazio (px) fra i bottoni
     */
    Carousel(GUI::Window &win,
             uint16_t item_sz = 60,
             uint16_t gap     = 20)
    {
        container = win.container;

        /*-- Layout flex orizzontale --*/
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER,
                                   LV_FLEX_ALIGN_CENTER,
                                   LV_FLEX_ALIGN_CENTER);

        /*-- Spaziatura & stile --*/
        lv_obj_set_style_pad_row(container, 0,   0);
        lv_obj_set_style_pad_column(container, gap, 0);
        lv_obj_set_style_pad_all(container, 0,   0);
        /*-- Scroll --*/
        lv_obj_set_scroll_dir(container, LV_DIR_HOR);
        lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER);
        lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_ACTIVE);
        lv_obj_add_flag(container, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    }

    /**
     * Aggiunge un elemento (bottone) al carosello.
     * @param txt   testo da mostrare se non viene passata un’icona
     * @param cb    callback LVGL quando il bottone viene cliccato
     * @param img   icona opzionale (lv_img_dsc_t*)
     * @param sz    lato del bottone (px). Di default item_sz passato al ctor.
     * @return puntatore all’oggetto bottone LVGL creato
     */
    lv_obj_t* addItem(const char           *txt,
                      lv_event_cb_t         cb  = nullptr,
                      const lv_img_dsc_t   *img = nullptr,
                      uint16_t              sz  = 60)
    {
        lv_obj_t *btn = lv_btn_create(container);
        lv_obj_set_size(btn, sz, sz);

        /* Bordo evidenziato quando il bottone riceve il focus */
        lv_obj_set_style_outline_width(btn, 2, LV_STATE_FOCUSED);
        lv_obj_set_style_outline_color(btn, lv_color_hex(0x00FFFF), LV_STATE_FOCUSED);

        /* Contenuto: icona o label */
        if(img) {
            lv_obj_t *ic = lv_img_create(btn);
            lv_img_set_src(ic, img);
            lv_obj_center(ic);
        } else {
            lv_obj_t *lab = lv_label_create(btn);
            lv_label_set_text(lab, txt);
            lv_obj_center(lab);
        }

        /* Navigazione tastiera/encoder */
        if(lv_group_t *g = lv_group_get_default())
            lv_group_add_obj(g, btn);

        /* Callback click */
        if(cb)
            lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, nullptr);

        items_.push_back(btn);   
        return btn;
    }

    /**
     * Porta il focus (e quindi centra) l’elemento con indice idx.
     */
    void focus(uint32_t idx)
    {
        if(idx < items_.size())
            lv_obj_scroll_to_view_recursive(items_[idx], LV_ANIM_ON);
    }

    /** Accesso diretto al contenitore LVGL. */
    lv_obj_t *raw() const { return container; }
};

} // namespace GUI
