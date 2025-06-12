#include "gui_system.hpp"
#include <esp_timer.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

using namespace HAL;

namespace GUI 
{
    /* ---------- stato interno ---------- */
    bool         started  = false;
    TaskHandle_t guiTask  = nullptr;
    static lv_display_t *lvDisplay  = nullptr;
    static lv_indev_t *lvInput = nullptr;
    static lv_group_t *kb_group = nullptr;
    HalCardputer Hal;

    /* ---------- flush callback ---------- */
    static void flush_cb(lv_display_t *display, const lv_area_t *area, unsigned char *data)
    {
        uint32_t w = lv_area_get_width(area);
        uint32_t h = lv_area_get_height(area);
        lv_draw_sw_rgb565_swap(data, w * h);
        Hal.display()->pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
        lv_disp_flush_ready(display);
    }

    /* ---------- tastiera Cardputer → LVGL ---------- */
    static void kb_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
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
                    else if(state.values.front() == KEY_BACKSPACE)
                    {
                        last_key = LV_KEY_DEL;
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
            data->state = LV_INDEV_STATE_PRESSED;
            data->key   = last_key;
            printf("Key pressed");
        } else {
            // Rilascio del tasto (mantieni l'ultimo valore)
            data->state = LV_INDEV_STATE_RELEASED;
            data->key   = last_key;
        }
        data->continue_reading = false;
    }

    /* ---------- LVGL system tick callback -----------*/
    static uint32_t lv_tick(void)
    {
        return esp_timer_get_time() / 1000;
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

    lv_init();
    static constexpr int drawBufSize = LV_HOR_RES_MAX * 70 * 2;
    static void* buf1 = heap_caps_malloc(drawBufSize, MALLOC_CAP_DMA);
    static void* buf2 = heap_caps_malloc(drawBufSize, MALLOC_CAP_DMA);

    lv_tick_set_cb(lv_tick);
    lvDisplay = lv_display_create(LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, flush_cb);
    lv_display_set_buffers(lvDisplay, buf1, buf2, drawBufSize, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_antialiasing(lvDisplay, true);

    /* Driver tastiera → keypad */
    kb_group = lv_group_create();
    lv_group_set_default(kb_group);
    lvInput = lv_indev_create();
    lv_indev_set_type(lvInput, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(lvInput, kb_read);
    lv_indev_set_group(lvInput, kb_group);

    xTaskCreate(gui_service, "gui_service", 8192, nullptr, 7, &guiTask);
    started = true;
    return true;
}

void GUI::shutdown()
{
    if (!started) return;
    lv_deinit();
    vTaskDelete(guiTask);
    started = false;
}

HalCardputer *GUI::hal()
{
    return &Hal;
}