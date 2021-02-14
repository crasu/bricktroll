#pragma once
#include "config.h"

extern TFT_eSPI *tft;

void tft_init();
void clear_tft();
void show_full_screen_message(String msg);
void show_string(String msg);
