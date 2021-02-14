#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include "Lpf2Hub.h"
#include "Lpf2HubConst.h"
#include "config.h"
#include "controller.h"
#include "rcmodel.h"
#include "tft.h"

TTGOClass *watch;

RcModel* model;

Lpf2Hub legoHub;

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

    tft_init();
    controller_init();

    show_full_screen_message("Connecting ");

    model = new RallyCar(legoHub);
}

void loop()
{
    model->connect();
    model->calibrate();
    Position pos =read_joystick();
    model->control(pos);
    delay(20);
    //delay(1000);
    //watch->shutdown();
}

