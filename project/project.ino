#include "Grobot.h"

#define PS2_DAT A6  //14
#define PS2_CMD A7  //15
#define PS2_SEL A13 //16
#define PS2_CLK A14 //17

#define pressures true
#define rumble true

Grobot *robot = new Grobot();
Gmotor *lmt = new Gmotor(2, 3),
       *rmt = new Gmotor(5, 4);

void setup()
{
  Serial.begin(9600);
  robot->configTFT(QD_TFT180A, 51, 52, 32, 34, 0, 33);
  robot->configController(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  lmt->setProp(0.8);
  rmt->setProp(0.8);
  robot->configMotor(lmt, rmt);

  robot->initialRobot();
  robot->enterManualMode();
}

void loop()
{
}
