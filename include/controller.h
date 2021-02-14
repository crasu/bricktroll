#pragma once
#include <Arduino.h>

struct Position {
    int16_t x;
    int16_t y;
    bool operator==(Position& o);
    bool operator!=(Position& o);
};

struct Keys {
    boolean a;
    boolean b;
    bool pressed();
};

void controller_init();
Position read_joystick();
Keys read_keys();