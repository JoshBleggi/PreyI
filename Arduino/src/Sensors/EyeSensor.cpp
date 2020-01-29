#include "EyeSensor.hpp"

const short MILLIS_BEFORE_CHANGE = 2500;

short lastMeasurementLeft = 0;
short lastMeasurementRight = 0;
long leftStateChangeTime = 0;
long rightStateChangeTime = 0;
// objects for the vl53l0x
Adafruit_VL6180X vl = Adafruit_VL6180X();
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();

LaserReadingStruct Sense();
LaserReadingStruct getState(short leftSensorReading, short rightSensorReading);
bool isRobotStuck(short &leftSensorReading, short leftMaxReading, short &rightSensorReading, short rightMaxReading);
bool isDifInThreshold(short &reading, short &lastMeasurement, long &lastChangeTime, short maxReading);

EyeSensor::EyeSensor() {
  leftStateChangeTime = millis();
  rightStateChangeTime = millis();
}

LaserReadingStruct EyeSensor::Sense() {
  VL53L0X_RangingMeasurementData_t measure1;
  short leftSensorReading, rightSensorReading;
  lox1.getSingleRangingMeasurement(&measure1); // pass in 'true' to get debug data printout!

  leftSensorReading = (short)measure1.RangeMilliMeter;
  rightSensorReading = (short)vl.readRange();
  // print sensor one reading
  Serial.println("Left Sensor: " + String(leftSensorReading));

  // print sensor two reading
  Serial.println("Right Sensor: " + String(rightSensorReading));

  return getState(leftSensorReading, rightSensorReading);
}

LaserReadingStruct EyeSensor::getState(short leftSensorReading, short rightSensorReading) {
  const short LEFT_TRIGGER_MAX = 650, RIGHT_TRIGGER_MAX = 255;
  const short LEFT_MAX_READING = 8190, RIGHT_MAX_READING = 255;

  LaserReadingStruct readings;
  readings.Stuck = isRobotStuck(leftSensorReading, LEFT_MAX_READING, rightSensorReading, RIGHT_MAX_READING);
  readings.LeftTrigger = leftSensorReading > 0 && leftSensorReading < LEFT_TRIGGER_MAX;
  readings.RightTrigger = rightSensorReading > 0 && rightSensorReading < RIGHT_TRIGGER_MAX;

  return readings;
}

bool EyeSensor::isRobotStuck(short &leftSensorReading, short leftMaxReading, short &rightSensorReading, short rightMaxReading) {
  if (isDifInThreshold(leftSensorReading, lastMeasurementLeft, leftStateChangeTime, leftMaxReading) || 
      isDifInThreshold(rightSensorReading, lastMeasurementRight, rightStateChangeTime, rightMaxReading)) {
    return true;
  }
  return false;
}

bool EyeSensor::isDifInThreshold(short &reading, short &lastMeasurement, long &lastChangeTime, short maxReading) {
  const byte threshold = 30;
  short dif = reading - lastMeasurement;
  if (dif < 0) {
    dif = dif * -1;
  }
  //Serial.println("Dif: " + String(dif) + " Last Change Time: " + String(lastChangeTime));
  if (dif < threshold && reading <= maxReading && ((millis() - lastChangeTime) > MILLIS_BEFORE_CHANGE)) {
    //Serial.println("STUCK STUCK STUCK STUCK STUCK");
    return true;
  }
  if (dif > threshold || reading >= maxReading){
    lastMeasurement = reading;
    lastChangeTime = millis();
  }
  //Serial.println("Not stuck");
  return false;
}
