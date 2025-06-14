/**
 * @file hal_cardputer.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "hal_cardputer.h"
#include "common_define.h"
#include "bat/adc_read.h"

using namespace HAL;

void HalCardputer::_display_init()
{
    _display = new M5GFX;
    _display->init();

    // Canvas
    /*_canvas = new LGFX_Sprite(_display);
    _canvas->createSprite(204, 109);

    _canvas_keyboard_bar = new LGFX_Sprite(_display);
    _canvas_keyboard_bar->createSprite(_display->width() - _canvas->width(), display()->height());

    _canvas_system_bar = new LGFX_Sprite(_display);
    _canvas_system_bar->createSprite(_canvas->width(), _display->height() - _canvas->height());*/
}

void HalCardputer::_keyboard_init()
{
    _keyboard = new KEYBOARD::Keyboard;
    _keyboard->init();
}

void HalCardputer::_mic_init()
{
    _mic = new m5::Mic_Class;

    // Configs
    auto cfg = _mic->config();
    cfg.pin_data_in = 46;
    cfg.pin_ws = 43;
    cfg.magnification = 4;

    cfg.task_priority = 15;

    // cfg.dma_buf_count = 8;
    // cfg.dma_buf_len = 512;
    // cfg.stereo = true;
    // cfg.sample_rate = 16000;

    cfg.i2s_port = i2s_port_t::I2S_NUM_0;
    _mic->config(cfg);
}

void HalCardputer::_speaker_init()
{
    _speaker = new m5::Speaker_Class;

    auto cfg = _speaker->config();
    cfg.pin_data_out = 42;
    cfg.pin_bck = 41;
    cfg.pin_ws = 43;
    cfg.i2s_port = i2s_port_t::I2S_NUM_1;
    // cfg.magnification = 1;
    _speaker->config(cfg);
}

void HalCardputer::_button_init()
{
    _homeButton = new Button(0);
    _homeButton->begin();
}

void HalCardputer::_bat_init() { adc_read_init(); }

void HalCardputer::_sdcard_init() { _sdcard = new SDCard; }

void HalCardputer::init()
{
    _display_init();
    _keyboard_init();
    //_speaker_init();
    //_mic_init();
    _button_init();
    _bat_init();
    _sdcard_init();
}

uint8_t HalCardputer::getBatLevel()
{
    double bat_v = static_cast<double>(adc_read_get_value()) * 2 / 1000;
    uint8_t result = 0;
    if (bat_v >= 4.12)
        result = 100;
    else if (bat_v >= 3.88)
        result = 75;
    else if (bat_v >= 3.61)
        result = 50;
    else if (bat_v >= 3.40)
        result = 25;
    else
        result = 0;
    return result;
}
