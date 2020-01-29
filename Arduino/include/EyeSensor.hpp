#ifndef EYESENSE_H
#define EYESENSE_H

#include <Adafruit_VL6180X.h>
#include <Adafruit_VL53L0X.h>
#include "LaserReadingStruct.hpp"

extern const short MILLIS_BEFORE_CHANGE;

extern short lastMeasurementLeft;
extern short lastMeasurementRight;
extern long leftStateChangeTime;
extern long rightStateChangeTime;
// objects for the vl53l0x
extern Adafruit_VL6180X vl;
extern Adafruit_VL53L0X lox1;

// this holds the measurement
extern VL53L0X_RangingMeasurementData_t measure1;

class EyeSensor {
    public:
        EyeSensor();
        LaserReadingStruct Sense();
    private:
        LaserReadingStruct getState(short leftSensorReading, short rightSensorReading);
        bool isRobotStuck(short &leftSensorReading, short leftMaxReading, short &rightSensorReading, short rightMaxReading);
        bool isDifInThreshold(short &reading, short &lastMeasurement, long &lastChangeTime, short maxReading);
};

#endif