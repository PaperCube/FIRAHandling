#include "Grobot.h"

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup() {
    Serial.begin(9600);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(A14, A7, A13, A6, true, true);
    robot->configMotor(lmt, rmt);
    lmt->setProp(1);
    rmt->setProp(1);
    lmt->setSolid(100);
    rmt->setSolid(100);
    robot->setLineCrossTime(10);
    robot->setNegativeSpeed(100);
    robot->configSensor(A1, A2, A3, A4, A5);
    robot->initialRobot();

    robot->pair();
}

void loop() { robot->enterManualMode(); }
