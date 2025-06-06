#include <M5GFX.h>
#include <lvgl.h>

static M5GFX display;

/* ---------- LVGL draw buffer ---------- */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[240 * 20];
static lv_color_t buf2[240 * 20];

/* ---------- Flush callback ---------- */
void my_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;

    display.startWrite();
    //display.pushImage(area->x1, area->y1, w, h,
    //                  (lgfx::rgb565_t *)color_p);
    display.pushImageDMA(area->x1, area->y1, w, h,
                         (lgfx::rgb565_t *)color_p);
    display.endWrite();

    /* aspetta la DMA per essere sicuri (≈ 0.6 ms per 240×20) */
    display.waitDMA();

    lv_disp_flush_ready(disp);
}

/* ---------- Setup LVGL + M5GFX ---------- */
void lvgl_setup()
{
    display.begin();
    display.initDMA();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf,
                          buf1, buf2,
                          sizeof(buf1) / sizeof(lv_color_t));

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res   = display.width();
    disp_drv.ver_res   = display.height();
    disp_drv.flush_cb  = my_flush_cb;
    disp_drv.draw_buf  = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}

/* ---------- App main ---------- */
extern "C" void app_main()
{
    lvgl_setup();

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Ciao, LVGL su Cardputer!");
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (true) 
    {
        lv_timer_handler();            // gestisce task LVGL
        vTaskDelay(pdMS_TO_TICKS(10));  // 5 ms = ~200 fps max
    }
}