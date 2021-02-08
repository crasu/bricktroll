#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include "Lpf2Hub.h"
#include "Lpf2HubConst.h"
#include "config.h"
#include "controller.h"

TTGOClass *watch;
TFT_eSPI *tft;
int16_t bg_color;
int steering_pos = 0;
bool isCalibrated = false;
bool isInitialized = false;

Lpf2Hub legoHub;
byte portB = (byte)ControlPlusHubPort::B;
byte portD = (byte)ControlPlusHubPort::D;

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

void tachoMotorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData)
{
  Lpf2Hub *myHub = (Lpf2Hub *)hub;

  Serial.print("cb port: ");
  Serial.println(portNumber, DEC);
  if (portNumber == 1)
  {
    int rotation = myHub->parseTachoMotor(pData);
    Serial.print("Rotation: ");
    Serial.print(rotation, DEC);
    steering_pos = rotation; 
  }
}

void initalizeCallback() {
    if (!isInitialized) {
        Serial.println("Callback initialized");
        legoHub.activatePortDevice(portB, tachoMotorCallback);
        delay(200);
        isInitialized = true;
    }
}

void connect_hub() {
    if (!legoHub.isConnected() && !legoHub.isConnecting()) {
        legoHub.init(); 
    }
    if (legoHub.isConnecting()) {
        legoHub.connectHub();
        if (legoHub.isConnected()) {
            show_full_screen_message("Connected to HUB");
            initalizeCallback();
        } else {
            show_full_screen_message("Failed to connected to HUB");
        }
    }
    if (!legoHub.isConnected()) {
        show_string(".");
        isInitialized = false;
        delay(500);
    }

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
}

void calibrate() {
    if (legoHub.isConnected() && !isCalibrated) {
        show_full_screen_message("Calibrating ");  
        
        legoHub.setAbsoluteMotorEncoderPosition(portB, 0);       
        legoHub.setTachoMotorSpeedForDegrees(portB, 100, 180);
        show_string(".");
        delay(2000);
        int min_pos = steering_pos;

        legoHub.setTachoMotorSpeedForDegrees(portB, -100, 180);
        show_string(".");
        delay(2000);
        int max_pos = steering_pos;

        Serial.print("Min: " + String(min_pos) + " Max: " + String(max_pos));
        legoHub.setAbsoluteMotorPosition(portB, 100, (min_pos + max_pos)/2);
        show_string(".");
        delay(2000);

        legoHub.setAbsoluteMotorEncoderPosition(portB, 0);
        show_string(".");
        delay(500);
        show_string(".");
        show_string(" Calibration complete");
        delay(500);
        isCalibrated = true;
    }
}

void control()
{
    Position prev_pos;

    if (legoHub.isConnected()) {
        Position pos = read_joystick();

        if (prev_pos != pos) {
            legoHub.setBasicMotorSpeed(portD, pos.x);
            legoHub.setAbsoluteMotorPosition(portB, 50, -pos.y);
            show_full_screen_message(String(pos.x) + " " + String(pos.y));
        }

        prev_pos.x = pos.x;
        prev_pos.y = pos.y;
    }
}

void loop()
{
    connect_hub();
    calibrate();
    control();
    
    delay(20);
}

