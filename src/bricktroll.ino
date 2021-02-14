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

    esp_wifi_deinit();

    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    tft_init(watch);
    controller_init();

    show_full_screen_message("Connecting ");

    model = new RallyCar(legoHub);
}

void auto_shutdown()
{
    static int counter = 0;
    if (legoHub.isConnected()) {
        counter = 0;
    } else {
        if (counter > 100) {
            show_full_screen_message("Shutting down");
            watch->shutdown();
            delay(500);
        }
        counter++;
    }
}

void loop()
{
    model->connect();
    model->calibrate();
    Position pos = read_joystick();
    model->control(pos);
    auto_shutdown();
    delay(20);
}

