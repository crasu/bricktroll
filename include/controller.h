#pragma once
#include <Arduino.h>

struct position {
    int16_t x;
    int16_t y;
};

void controller_init();
position read_joystick();