#include <M5GFX.h>

extern "C" void app_main() 
{
    static M5GFX display;
    display.begin();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.drawString("M5GFX OK!", 10, 60, &fonts::Font2);
    while (true) vTaskDelay(pdMS_TO_TICKS(1000));
}