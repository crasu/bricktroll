#include <Arduino.h>
#include "config.h"
#include "controller.h"

void controller_init()
{
    pinMode(HW_CONTROLLER_GPIO_UP_DOWN, INPUT);
    pinMode(HW_CONTROLLER_GPIO_LEFT_RIGHT, INPUT);
    pinMode(HW_CONTROLLER_GPIO_SELECT, INPUT_PULLUP);
    pinMode(HW_CONTROLLER_GPIO_START, INPUT_PULLUP);
    pinMode(HW_CONTROLLER_GPIO_A, INPUT_PULLUP);
    pinMode(HW_CONTROLLER_GPIO_B, INPUT_PULLUP);
    pinMode(HW_CONTROLLER_GPIO_X, INPUT_PULLUP);
    pinMode(HW_CONTROLLER_GPIO_Y, INPUT_PULLUP);

    analogReadResolution(11);
}

int16_t normalize(int16_t in) {
    int16_t out = in - 1024;
    out = -out*100/1024;
    
    if (abs(out) < 15) {
        out = 0;
    }

    return out;
}

Position read_joystick()
{
    Position pos;

    pos.x = normalize(analogRead(HW_CONTROLLER_GPIO_UP_DOWN));
    pos.y = normalize(analogRead(HW_CONTROLLER_GPIO_LEFT_RIGHT));

    return pos;
}


bool Position::operator==(Position& o) {
   return x == o.x && y == o.y;
}

bool Position::operator!=(Position& o) {
   return !(*this == o);
}


Keys read_keys()
{
    Keys keys;

    keys.a = digitalRead(HW_CONTROLLER_GPIO_A);
    keys.b = digitalRead(HW_CONTROLLER_GPIO_B);

    return keys;
}

boolean Keys::pressed()
{
    return this->a && this->b;
}

