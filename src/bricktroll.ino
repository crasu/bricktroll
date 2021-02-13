#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include "Lpf2Hub.h"
#include "Lpf2HubConst.h"
#include "config.h"
#include "controller.h"
#include "rcmodel.h"

TTGOClass *watch;
TFT_eSPI *tft;
int16_t bg_color;
RcModel* model;

Lpf2Hub legoHub;

void clear_tft()
{
    tft->fillScreen(bg_color);
}

void show_full_screen_message(String msg) {
    clear_tft();
    tft->setCursor(0, 0);
    Serial.println(msg);
    tft->println(msg);
}

void show_string(String msg) {
    Serial.print(msg);
    tft->print(msg);
}

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();
    watch->openBL();

    tft = watch->tft;

    esp_wifi_deinit();

    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    bg_color = tft->color565(0, 0, 0);
    tft->setTextSize(2);
    tft->fillScreen(bg_color);
    tft->setCursor(0, 0);
    tft->setRotation(3);

    controller_init();

    show_full_screen_message("Connecting ");

    model = new RallyCar(legoHub);
}

void loop()
{
    /*model->connect();
    model->calibrate();
    Position pos =read_joystick();
    model->control(pos);
    */
    delay(20);
}

