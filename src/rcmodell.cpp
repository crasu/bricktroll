#include "rcmodel.h"
#include "tft.h"

byte portB = (byte)ControlPlusHubPort::B;
byte portD = (byte)ControlPlusHubPort::D;

static int steering_pos;

void setSteeringPos(int sp) {
    steering_pos = sp;
}

int getSteeringPos() {
    return steering_pos;
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
    setSteeringPos(rotation); 
  }
}

void RcModel::initalizeCallback(byte port) {
    if (!isInitialized) {
        Serial.println("Callback initialized");
        hub.activatePortDevice(port, tachoMotorCallback);
        delay(200);
        isInitialized = true;
    }
}

void RcModel::connect() {
    if (!hub.isConnected() && !hub.isConnecting()) {
        hub.init(); 
    }
    if (hub.isConnecting()) {
        hub.connectHub();
        if (hub.isConnected()) {
            show_full_screen_message("Connected to HUB");
        } else {
            show_full_screen_message("Failed to connected to HUB");
        }
    }
    if (!hub.isConnected()) {
        show_string(".");
        isInitialized = false;
        delay(500);
    }
}

void RcModel::calibrate(byte port) {
    if (hub.isConnected() && !isCalibrated) {
        initalizeCallback(port);

        show_full_screen_message("Calibrating ");  
        
        hub.setAbsoluteMotorEncoderPosition(portB, 0);       
        hub.setTachoMotorSpeedForDegrees(portB, 100, 180);
        show_string(".");
        delay(2000);
        int min_pos = getSteeringPos();

        hub.setTachoMotorSpeedForDegrees(portB, -100, 180);
        show_string(".");
        delay(2000);
        int max_pos = getSteeringPos();

        Serial.print("Min: " + String(min_pos) + " Max: " + String(max_pos));
        hub.setAbsoluteMotorPosition(portB, 100, (min_pos + max_pos)/2);
        show_string(".");
        delay(2000);

        hub.setAbsoluteMotorEncoderPosition(portB, 0);
        show_string(".");
        delay(500);
        show_string(".");
        show_string(" Calibration complete");
        delay(500);
        isCalibrated = true;
    }

}

void RallyCar::calibrate() 
{
    RcModel::calibrate(portB);
}

void RallyCar::control(Position pos) {
    static Position prev_pos;

    if (hub.isConnected()) {
        if (prev_pos != pos) {
            hub.setBasicMotorSpeed(portD, pos.x);
            hub.setAbsoluteMotorPosition(portB, 50, -pos.y);
            show_full_screen_message(String(pos.x) + " " + String(pos.y));
        }

        prev_pos.x = pos.x;
        prev_pos.y = pos.y;
    }
}