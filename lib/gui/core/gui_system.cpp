#include "gui_system.hpp"
#include <esp_timer.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace 
{
    /* ---------- stato interno ---------- */
    bool         started  = false;
    TaskHandle_t guiTask  = nullptr;
    lv_disp_t   *lv_disp  = nullptr;

    /* ---------- flush callback ---------- */
    static void flush_cb(lv_disp_drv_t *disp, const lv_area_t *a, lv_color_t *p)
    {
        uint16_t w = a->x2 - a->x1 + 1;
        uint16_t h = a->y2 - a->y1 + 1;
        M5Cardputer.Display.pushImageDMA(a->x1, a->y1, w, h, (lgfx::rgb565_t *)p);
        lv_disp_flush_ready(disp);
    }

    /* ---------- tastiera Cardputer → LVGL ---------- */
    static void kb_read(lv_indev_drv_t *, lv_indev_data_t *d)
    {
        static uint32_t last = 0;
        auto &state = M5Cardputer.Keyboard.keysState();

        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) 
        {
            if (!state.word.empty()) 
            {
                /* 1) c’è un carattere stampabile → prendi il 1° */
                last = (uint8_t)state.word.front();
            }
            else if (!state.hid_keys.empty()) 
            {
                /* 2) altrimenti prendi il 1° HID (BACKSPACE, ENTER, ecc.) */
                uint8_t hid = state.hid_keys.front();
                switch (hid) {
                    case KEY_ENTER:     last = LV_KEY_ENTER; break;
                    case KEY_BACKSPACE: last = LV_KEY_BACKSPACE; break;
                    case KEY_TAB:       last = LV_KEY_NEXT; break;       /* o LV_KEY_PREV */
                    /* ... altri mapping se ti servono ... */
                    default:            last = 0; break;
                }
            }
            else 
            {
                last = 0;
            }
            d->state = LV_INDEV_STATE_PRESSED;
            d->key   = last;
        }
        else {
            d->state = LV_INDEV_STATE_RELEASED;
            d->key   = last;
        }
    }

    /* ---------- task di servizio GUI ---------- */
    static void gui_service(void *)
    {
        const TickType_t xDelay = pdMS_TO_TICKS(LV_REFR_PERIOD);
        while (true) 
        {
            lv_timer_handler();
            M5Cardputer.update();
            vTaskDelay(xDelay);
        }
    }
}

/* ===== API PUBLICA ================================================= */
bool GUI::begin()
{
    if (started) return true;

    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.init();
    M5Cardputer.Display.initDMA();
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.fillScreen(TFT_BLACK);

    
    static lv_disp_draw_buf_t draw_buf; // buffer di disegno LVGL
    static lv_disp_drv_t disp_drv; // driver display → LVGL
    static lv_indev_drv_t kb_drv; // tastiera Cardputer → LVGL

    lv_init();
    static lv_color_t *buf1 = (lv_color_t*)heap_caps_malloc(LV_HOR_RES_MAX * 30 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    static lv_color_t *buf2 = (lv_color_t*)heap_caps_malloc(LV_HOR_RES_MAX * 30 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_HOR_RES_MAX * 20);
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

    xTaskCreate(gui_service, "gui_service", 4096, nullptr, 5, &guiTask);

    started = true;
    return true;
}

void GUI::shutdown()
{
    if (!started) return;
    vTaskDelete(guiTask);
    started = false;
}

lv_disp_t *GUI::display() { return lv_disp; }
M5GFX     &GUI::gfx()     { return M5Cardputer.Display; }
