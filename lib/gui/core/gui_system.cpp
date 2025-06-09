#include "gui_system.hpp"
#include <esp_timer.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <M5GFX.h>

using namespace HAL;

namespace GUI 
{
    /* ---------- stato interno ---------- */
    bool         started  = false;
    TaskHandle_t guiTask  = nullptr;
    lv_disp_t   *lv_disp  = nullptr;
    HalCardputer Hal;

    /* ---------- flush callback ---------- */
    static void flush_cb(lv_disp_drv_t *disp, const lv_area_t *a, lv_color_t *p)
    {
        uint16_t w = a->x2 - a->x1 + 1;
        uint16_t h = a->y2 - a->y1 + 1;
        Hal.display()->pushImageDMA(a->x1, a->y1, w, h, (lgfx::rgb565_t *)p);
        lv_disp_flush_ready(disp);
    }

    /* ---------- tastiera Cardputer → LVGL ---------- */
    static void kb_read(lv_indev_drv_t *, lv_indev_data_t *d)
    {
        static uint32_t last_key = 0;
        auto *kb = Hal.keyboard();
        auto &state = kb->keysState();

        if (kb->isChanged() && kb->isPressed()) 
        {
            if (!state.values.empty()) 
            {
                if(state.fn)
                {
                    if(state.values.front() == ','){
                        last_key = LV_KEY_LEFT;
                    }
                    else if(state.values.front() == '.'){
                        last_key = LV_KEY_DOWN;
                    }
                    else if(state.values.front() == '/'){
                        last_key = LV_KEY_RIGHT;
                    }
                    else if(state.values.front() == ';'){
                        last_key = LV_KEY_UP;
                    } 
                } 
                else {
                    // 3) Carattere stampabile → prendi il primo
                    last_key = static_cast<uint8_t>(state.values.front());
                }
            }
            else if (!state.hidKey.empty()) {
                // 2) Tasti speciali HID → prendi il primo e mappa
                uint8_t hid = static_cast<uint8_t>(state.hidKey.front());
                switch (hid) {
                    case KEY_ENTER:     last_key = LV_KEY_ENTER;     break;
                    case KEY_BACKSPACE: last_key = LV_KEY_BACKSPACE; break;
                    case KEY_TAB:       last_key = LV_KEY_NEXT;      break;
                    case KEY_LEFT:      last_key = LV_KEY_LEFT;      break;
                    case KEY_RIGHT:     last_key = LV_KEY_RIGHT;     break;
                    case KEY_UP:        last_key = LV_KEY_UP;        break;
                    case KEY_DOWN:      last_key = LV_KEY_DOWN;      break;
                    case KEY_HOME:      last_key = LV_KEY_HOME;      break;
                    // Aggiungi altri mapping HID se necessari
                    default:            last_key = 0;                break;
                }
            }
            else {
                if(state.tab) {
                    last_key = LV_KEY_NEXT;
                }
                else {
                    last_key = 0;
                }
            }
            d->state = LV_INDEV_STATE_PRESSED;
            d->key   = last_key;
        } else {
            // Rilascio del tasto (mantieni l'ultimo valore)
            d->state = LV_INDEV_STATE_RELEASED;
            d->key   = last_key;
        }
    }

    /* ---------- task di servizio GUI ---------- */
    static void gui_service(void *)
    {
        static uint32_t delay = 0;
        while (true) 
        {
            delay = lv_timer_handler();
            Hal.keyboard()->updateKeyList();
            Hal.keyboard()->updateKeysState();
            if(delay == LV_NO_TIMER_READY)
                delay = pdMS_TO_TICKS(LV_REFR_PERIOD);
            else
                delay = pdMS_TO_TICKS(delay);
            vTaskDelay(delay);
        }
    }
}

/* ===== API PUBLICA ================================================= */
bool GUI::begin()
{
    if (started) return true;
    Hal.init();

    static lv_disp_draw_buf_t draw_buf; // buffer di disegno LVGL
    static lv_disp_drv_t disp_drv; // driver display → LVGL
    static lv_indev_drv_t kb_drv; // tastiera Cardputer → LVGL

    lv_init();
    static lv_color_t *buf1 = (lv_color_t*)heap_caps_malloc(LV_HOR_RES_MAX * 135 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    static lv_color_t *buf2 = (lv_color_t*)heap_caps_malloc(LV_HOR_RES_MAX * 135 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_HOR_RES_MAX * 135);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = LV_HOR_RES_MAX;
    disp_drv.ver_res  = LV_VER_RES_MAX;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp = lv_disp_drv_register(&disp_drv);

    /* Driver tastiera → keypad */
    lv_indev_drv_init(&kb_drv);
    kb_drv.type    = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb = kb_read;
    lv_indev_t *kb = lv_indev_drv_register(&kb_drv);
    lv_group_t *g  = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(kb, g);

    xTaskCreate(gui_service, "gui_service", 4096, nullptr, 7, &guiTask);

    started = true;
    return true;
}

void GUI::shutdown()
{
    if (!started) return;
    vTaskDelete(guiTask);
    started = false;
}

HalCardputer *GUI::hal()
{
    return &Hal;
}