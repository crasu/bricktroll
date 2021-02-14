#include "tft.h"

TFT_eSPI *tft;
int16_t bg_color;

void tft_init(TTGOClass *watch)
{
    tft = watch->tft;
    bg_color = tft->color565(0, 0, 0);
    tft->setTextSize(2);
    tft->fillScreen(bg_color);
    tft->setCursor(0, 0);
    tft->setRotation(3);
}

void clear_tft()
{
    tft->fillScreen(bg_color);
}

void show_full_screen_message(String msg) 
{
    clear_tft();
    tft->setCursor(0, 0);
    Serial.println(msg);
    tft->println(msg);
}

void show_string(String msg)
{
    Serial.print(msg);
    tft->print(msg);
}
