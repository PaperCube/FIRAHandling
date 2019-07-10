#include "Grobot.h"

#define PS2_DAT A6  // 14
#define PS2_CMD A7  // 15
#define PS2_SEL A13 // 16
#define PS2_CLK A14 // 17

#define pressures true
#define rumble true

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(3, 2), *rmt = new Gmotor(5, 4);

void setup() {
    Serial.begin(9600);
    robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
    robot->configController(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures,
                            rumble);
    lmt->setProp(1);
    rmt->setProp(1);
    lmt->setSolid(100);
    rmt->setSolid(100);

    robot->configMotor(lmt, rmt);
    robot->configSensor(6, 7);
    robot->configSensor(A1, A2, A3, A4, A5);

    // robot->initialRobot();
    // robot->pair();
    robot->setLineCrossTime(10);
    robot->setNegativeSpeed(100);
    robot->setThreshold(1, 888);
    robot->setThreshold(2, 888);
    robot->setThreshold(3, 888);
    robot->setThreshold(4, 888);
    robot->setThreshold(5, 888);
    // robot->testSensors();
    // robot->robot->setHunterSensor(2, 3, 4);
    robot->waitForButtonPress(PSB_START);
    // robot->huntLine(50, 50, 9999);
    for (;;) {
        robot->huntLine(30, 60, 1);
        robot->turnLeft(80, 0);
        robot->huntLine(30, 60, 1);
        robot->turnLeft(80, 0);
        robot->huntLine(30, 60, 1);
        robot->turnLeft(80, 0);
        robot->huntLine(30, 60, 1);
        robot->turnLeft(80, 0);
    }
    robot->stop(0);
    robot->waitForButtonPress(PSB_START);
    robot->enterManualMode();
}

void loop() {}
