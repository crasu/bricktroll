#pragma once

#include "controller.h"
#include "Lpf2Hub.h"
#include "Lpf2HubConst.h"


class RcModel {
    private:
        RcModel();
        static int steering_pos;

    protected:
        RcModel(Lpf2Hub& hub): hub(hub) {};
        Lpf2Hub& hub;
        bool isCalibrated = false;
        bool isInitialized = false;
        void initalizeCallback();

    public:
        virtual void connect();
        virtual void control(Position pos);
        virtual void calibrate();
        static void setSteeringPos(int steering_pos) {
            RcModel::steering_pos = steering_pos;
        }
        int getSteeringPos() {
            return RcModel::steering_pos;
        }
};

class RallyCar: public RcModel {
    private:
        RallyCar();
    public:
        RallyCar(Lpf2Hub& hub): RcModel(hub) {};
        void connect();
        void control(Position pos);
        void calibrate();
};