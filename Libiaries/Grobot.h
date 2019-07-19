#include "GMotor.h"
#include <Buzzer.h>
#include <Key.h>
#include <LED.h>
#include <PS2X_lib.h>
#include <Servo.h>
#include <UTFT.h>

typedef unsigned char uc;
typedef unsigned int  ui;

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
byte           vibrate = 0;

class Grobot {
  private:
    Gmotor *mtl, *mtr;
    Servo * armServo, *handServo;
    uc      s1, s2, s3, s4, s5;
    uc      hunterL = s2, hunterM = s3, hunterR = s4;
    UTFT *  GLCD       = new UTFT(QD_TFT180A, 51, 52, 32, 34, 33);
    PS2X *  controller = new PS2X();
    int     threshold[6];
    int     lineCrossTime = 70;
    int     negativeSpeed = 100, negativeRuntime = 50;
    int     handGraspAng, handReleaseAng, armLiftAng, armPutAng;

  public:
    Grobot();
    void configMotor(Gmotor *, Gmotor *);
    void configSensor(uc, uc, uc, uc, uc);
    void configTFT(byte pos);
    void configTFT(byte, int, int, int, int, byte, int);
    byte configController(uc, uc, uc, uc);
    void configController(uc, uc, uc, uc, bool, bool);
    void configServo(int, int);
    void configHandAng(int, int);
    void configArmAng(int, int);

    void testSensors();
    void setThreshold(int, int, int, int, int);
    void setThreshold(int, int);
    int  getThreshold(int);
    void setLineCrossTime(int);
    void setNegativeSpeed(int);
    void setHunterSensor(int, int, int);

    int     getSensorVal(uc);
    Gmotor *getMotor(uc);

    void wait(int);
    void walkTime(int, int);
    void walkBlock(int, int);
    void turnLeft(int, int);
    void turnRight(int, int);
    void huntLine(int, int, int);
    void stop(int);

    void handGrasp();
    void handRelease();
    void armLift();
    void armPut();

    void waitForButtonPress(ui);
    void waitForButtonRelease(ui);

    void standByLine(int, int, int);

    void initialRobot();
    void pair();
    void enterManualMode();
};

Grobot::Grobot() {}

void Grobot::configMotor(Gmotor *leftMotor, Gmotor *rightMotor) {
    this->mtl = leftMotor;
    this->mtr = rightMotor;
}

void Grobot::configSensor(uc s1, uc s2, uc s3, uc s4, uc s5) {
    this->s1 = s1;
    this->s2 = s2, this->s3 = s3, this->s4 = s4;
    this->s5 = s5;
}

void Grobot::configTFT(byte pos) { this->GLCD->InitLCD(pos); }

void Grobot::configTFT(
    byte model, int RS, int WR, int CS, int RST, byte pos, int SER = 0) {
    this->GLCD = new UTFT(model, RS, WR, CS, RST, SER);
    this->GLCD->InitLCD(pos);
}

byte Grobot::configController(uc clk, uc cmd, uc att, uc dat) {
    return this->controller->config_gamepad(clk, cmd, att, dat);
}

void Grobot::configController(
    uc clk, uc cmd, uc att, uc dat, bool pressures, bool rumble) {
    this->controller->config_gamepad(clk, cmd, att, dat, pressures, rumble);
}

void Grobot::configServo(int armPin, int handPin) {
    this->armServo->attach(armPin);
    this->handServo->attach(handPin);
}

void Grobot::configHandAng(int handReleaseAng, int handGraspAng) {
    this->handGraspAng   = handGraspAng;
    this->handReleaseAng = handReleaseAng;
}
void Grobot::configArmAng(int armPutAng, int armLiftAng) {
    this->armLiftAng = armLiftAng;
    this->armPutAng  = armPutAng;
}

void Grobot::waitForButtonPress(ui button) {
    while (true) {
        delay(50);
        this->controller->read_gamepad();
        if (this->controller->ButtonPressed(button))
            break;
    }
}
void Grobot::waitForButtonRelease(ui button) {
    while (true) {
        delay(50);
        this->controller->read_gamepad();
        if (this->controller->ButtonReleased(button))
            break;
    }
}
/*Here are all buttons
PSB_SELECT      0x0001
PSB_L3          0x0002
PSB_R3          0x0004
PSB_START       0x0008
PSB_PAD_UP      0x0010
PSB_PAD_RIGHT   0x0020
PSB_PAD_DOWN    0x0040
PSB_PAD_LEFT    0x0080
PSB_L2          0x0100
PSB_R2          0x0200
PSB_L1          0x0400
PSB_R1          0x0800
PSB_GREEN       0x1000
PSB_RED         0x2000
PSB_BLUE        0x4000
PSB_PINK        0x8000
PSB_TRIANGLE    0x1000
PSB_CIRCLE      0x2000
PSB_CROSS       0x4000
PSB_SQUARE      0x8000
  */

void Grobot::setThreshold(int s1, int s2, int s3, int s4, int s5) {
    this->threshold[1] = s1;
    this->threshold[2] = s2, this->threshold[3] = s3, this->threshold[4] = s4;
    this->threshold[5] = s5;
}

void Grobot::setThreshold(int sensorNum, int val) {
    this->threshold[sensorNum] = val;
}

int Grobot::getThreshold(int sensorNum) { return this->threshold[sensorNum]; }

void Grobot::setLineCrossTime(int lineCrossTime) {
    this->lineCrossTime = lineCrossTime;
}

void Grobot::setNegativeSpeed(int speed) { this->negativeSpeed = speed; }

int Grobot::getSensorVal(uc pin) { return analogRead(pin); }

Gmotor *Grobot::getMotor(uc motor) {
    return motor == 'l' ? this->mtl : this->mtr;
}

void Grobot::wait(int ms) { delay(ms); }

void Grobot::walkTime(int speed, int ms) {
    this->mtl->setSpeed(speed);
    this->mtr->setSpeed(speed);
    delay(ms);
    this->mtl->stop(0);
    this->mtr->stop(0);
}

void Grobot::walkBlock(int speed, int lineCnt) {
    while (analogRead(s3) > this->threshold[3]) {
        this->mtl->setSpeed(speed);
        this->mtr->setSpeed(speed);
    }
    this->walkTime(speed, this->lineCrossTime);
}

void Grobot::turnLeft(int speed, int mode) {
    switch (mode) {
    case 0:
        this->mtl->setSpeed(-speed);
        this->mtr->setSpeed(speed);
        break;
    case 1:
        this->mtr->setSpeed(speed);
        break;
    case 2:
        this->mtl->setSpeed(-speed);
        break;
    default:
        this->mtl->setSpeed(-speed);
        this->mtr->setSpeed(speed);
        break;
    }
    while (analogRead(this->s3) < this->threshold[3])
        ;
    delay(50);
    while (analogRead(this->s3) > this->threshold[3])
        ;
    this->stop(0);
}

void Grobot::turnRight(int speed, int mode) {
    switch (mode) {
    case 0:
        this->mtr->setSpeed(-speed);
        this->mtl->setSpeed(speed);
        break;
    case 1:
        this->mtl->setSpeed(speed);
        break;
    case 2:
        this->mtr->setSpeed(-speed);
        break;
    default:
        this->mtr->setSpeed(-speed);
        this->mtl->setSpeed(speed);
        break;
    }
    while (analogRead(this->s3) > this->threshold[3])
        ;
    delay(50);
    while (analogRead(this->s3) < this->threshold[3])
        ;
    delay(50);
    while (analogRead(this->s3) > this->threshold[3])
        ;
    this->stop(1);
}

void Grobot::setHunterSensor(int hunterNumL, int hunterNumM, int hunterNumR) {
    // this->hunterNumL = hunterNumL;
    // this->hunterNumM = hunterNumM;
    // this->hunterNumR = hunterNumR;
}

void Grobot::huntLine(int baseSpeed, int subSpeed, int lineCnt) {
    for (int i = 1; i <= lineCnt; i++) {
        while (analogRead(A2) > this->threshold[2] &&
               analogRead(A4) > this->threshold[4]) {
            if (analogRead(A3) > this->threshold[3])
                mtl->setSpeed(baseSpeed + subSpeed), mtr->setSpeed(baseSpeed);
            else
                mtl->setSpeed(baseSpeed), mtr->setSpeed(baseSpeed + subSpeed);
        }
        // this->walkTime(baseSpeed + subSpeed, this->lineCrossTime);
        this->stop(0);
    }
}

void Grobot::stop(int mode) {
    if (mode == 1) {
        this->mtl->setSpeed(
            this->mtl->getSpeed() == 0
                ? 0
                : (this->mtl->getSpeed() < 0 ? negativeSpeed : -negativeSpeed));
        this->mtr->setSpeed(
            this->mtl->getSpeed() == 0
                ? 0
                : (this->mtl->getSpeed() < 0 ? negativeSpeed : -negativeSpeed));
        delay(negativeRuntime);
    }
    this->mtl->stop(mode);
    this->mtr->stop(mode);
}

void Grobot::handGrasp() { this->handServo->write(this->handGraspAng); }
void Grobot::handRelease() { this->handServo->write(this->handReleaseAng); }
void Grobot::armLift() { this->armServo->write(this->armLiftAng); }
void Grobot::armPut() { this->armServo->write(this->armPutAng); }

void Grobot::standByLine(int baseSpeed, int subNegative, int sensitivityDelay) {
    while (this->getSensorVal(A1) >= this->threshold[1] &&
           this->getSensorVal(A5) >= this->threshold[5]) {
        if (this->getSensorVal(A1) < this->threshold[1])
            this->mtl->setSpeed(-subNegative);
        if (this->getSensorVal(A5) < this->threshold[5])
            this->mtr->setSpeed(subNegative);
        deelay(50);
        this->mtl->setSpeed(baseSpeed);
        this->mtr->setSpeed(baseSpeed);
    }
}

void Grobot::initialRobot() {
    // error = controller->config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,
    // pressures, rumble);
    byte type = this->controller->readType();
    // ScreenStart

    this->GLCD->clrScr();
    this->GLCD->setBackColor(0, 0, 0);
    this->GLCD->drawRoundRect(7, 5, 128 - 5, 160 - 5);
    this->GLCD->setFont(BigFont);
    this->GLCD->setColor(178, 16, 22);
    this->GLCD->print("FIRAxZB", 80, 25, 90);
    this->GLCD->setColor(255, 255, 255);
    this->GLCD->setFont(SmallFont);
    this->GLCD->print("Loading.....", 22, 40, 90);
    this->GLCD->setBackColor(70, 70, 70);
    this->GLCD->print("VisualDust Design", 60, 14, 90);
    this->GLCD->setBackColor(0, 0, 0);
    this->GLCD->setColor(255, 192, 0);
    for (int i = 5; i <= 155; i++)
        this->GLCD->drawLine(0, i, 3, i), delay(13);
    // ScreenStartEnd
    delay(500);
}

void Grobot::pair() {
    this->GLCD->clrScr();
    this->GLCD->setColor(255, 255, 255);
    this->GLCD->drawRoundRect(5, 5, 128 - 12, 160 - 5);
    this->GLCD->print("Searching", 100, 23, 90);
    this->GLCD->print("Contorller", 86, 19, 90);
    // check if controller is availiable
    for (;;) {
        SetLedRGB(1, 0, 0);
        for (int r = 1; r <= 10; r++)
            this->GLCD->drawCircle(88, 130, r), delay(1);
        this->GLCD->setColor(0, 0, 0);
        SetLedRGB(0, 0, 1);
        for (int r = 1; r <= 10; r++)
            this->GLCD->drawCircle(88, 130, r), delay(1);
        this->GLCD->setColor(255, 255, 255);
        this->controller->read_gamepad();
        if (this->controller->ButtonPressed(PSB_SELECT)) {
            for (int r = 1; r <= 10; r++)
                this->GLCD->drawCircle(88, 130, r), delay(1);
            break;
        }
    }
    this->GLCD->setFont(BigFont);
    this->GLCD->setColor(255, 255, 255);
    this->GLCD->setBackColor(0, 176, 80);
    this->GLCD->print("DONE", 40, 50, 90);
    // check finished
    this->GLCD->setFont(SmallFont);
    this->GLCD->setBackColor(0, 0, 0);
    this->GLCD->setColor(34, 177, 76);
    SetLedRGB(0, 1, 0);
    this->GLCD->print("Press\"Start\"ToRun", 60, 14, 90);
    for (;;) {
        this->controller->read_gamepad();
        if (controller->ButtonPressed(PSB_START) /*  || GetKey(2) z*/)
            break;
        delay(30);
    }
    SetLedRGB(1, 1, 1);
    delay(100);
}

void Grobot::testSensors() {
    this->GLCD->clrScr();
    this->GLCD->clrScr();
    this->GLCD->setColor(255, 255, 255);
    this->GLCD->drawRoundRect(18, 5, 128 - 15, 160 - 5);
    this->GLCD->setFont(SmallFont);
    this->GLCD->print("Now test sensors", 15, 5, 90);
    uc sensorList[6] = {' ', this->s1, this->s2, this->s3, this->s4, this->s5};
    this->GLCD->setBackColor(0, 162, 232);
    this->GLCD->setColor(0, 0, 0);
    this->GLCD->print("Value for S", 31, 7, 90);
    this->GLCD->setBackColor(0, 0, 0);
    this->GLCD->setColor(255, 255, 255);
    for (int i = 1; i <= 5; i++) {
        if (i == 1 || i == 5)
            continue;
        this->GLCD->setBackColor(0, 162, 232);
        this->GLCD->setColor(0, 0, 0);
        this->GLCD->print("                         ", 128, 0, 90);
        this->GLCD->setBackColor(0, 0, 0);
        this->GLCD->setColor(255, 255, 255);
        int tmpW, tmpB;

        this->GLCD->print(String(i) + "W ", 31, 100, 90);
        this->GLCD->setFont(BigFont);
        for (;;) {
            this->GLCD->print(String(analogRead(sensorList[i])) + "  ", 70, 56,
                              90);
            this->controller->read_gamepad();
            if (controller->ButtonPressed(PSB_START) /*  || GetKey(2) z*/)
                break;
        }

        this->GLCD->setFont(SmallFont);
        tmpW = analogRead(sensorList[i]);
        this->GLCD->setBackColor(0, 162, 232);
        this->GLCD->setColor(0, 0, 0);
        this->GLCD->print(" White:" + String(tmpW), 128, 0, 90);
        this->GLCD->setBackColor(0, 0, 0);
        this->GLCD->setColor(255, 255, 255);
        delay(1000);
        this->GLCD->print(String(i) + "B ", 31, 100, 90);
        this->GLCD->setFont(BigFont);
        for (;;) {
            this->GLCD->print(String(analogRead(sensorList[i])) + "  ", 70, 56,
                              90);
            this->controller->read_gamepad();
            if (controller->ButtonPressed(PSB_START) /*  || GetKey(2) z*/)
                break;
        }
        this->GLCD->setFont(SmallFont);
        tmpB = analogRead(sensorList[i]);
        this->GLCD->setBackColor(0, 162, 232);
        this->GLCD->setColor(0, 0, 0);
        this->GLCD->print("Black:" + String(tmpB), 128, 80, 90);
        delay(1000);
        this->GLCD->setBackColor(0, 0, 0);
        this->GLCD->setColor(255, 255, 255);

        int sub            = (tmpW - tmpB) >= 0 ? (tmpW - tmpB) : (tmpB - tmpW);
        this->threshold[i] = (tmpB + tmpW) / 2 + (double)((sub / 2.0) * 0.7);
        Serial.println("Threshold for sensor A" + String(i) + "=" +
                       this->threshold[i]);
    }
}

void Grobot::enterManualMode() {
    for (;;) {
        this->controller->read_gamepad(
            false, vibrate); // read controller and set large motor to spin
                             // at 'vibrate' speed
        if (this->controller->Button(
                PSB_START)) // will be TRUE as long as button is pressed
            Serial.println("Start is being held");
        if (this->controller->Button(PSB_SELECT))
            Serial.println("Select is being held");

        if (this->controller->Button(
                PSB_PAD_UP)) { // will be TRUE as long as button is pressed
            Serial.print("Up held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_UP), DEC);
        }
        if (this->controller->Button(PSB_PAD_RIGHT)) {
            Serial.print("Right held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_RIGHT), DEC);
        }
        if (this->controller->Button(PSB_PAD_LEFT)) {
            Serial.print("LEFT held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_LEFT), DEC);
        }
        if (this->controller->Button(PSB_PAD_DOWN)) {
            Serial.print("DOWN held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_DOWN), DEC);
        }

        vibrate = this->controller->Analog(PSAB_CROSS);
        vibrate = this->controller->Analog(PSAB_CIRCLE);
        if (this->controller
                ->NewButtonState()) { // will be TRUE if any button changes
                                      // state (on to off, or off to on)
            if (this->controller->Button(PSB_L3))
                Serial.println("L3 pressed");
            if (this->controller->Button(PSB_R3))
                Serial.println("R3 pressed");
            if (this->controller->Button(PSB_L2))
                Serial.println("L2 pressed");
            if (this->controller->Button(PSB_R2))
                Serial.println("R2 pressed");
            if (this->controller->Button(PSB_TRIANGLE))
                Serial.println("Triangle pressed");
        }

        if (this->controller->ButtonPressed(PSB_CIRCLE)) {
            // will be TRUE if button was JUST pressed
            mtl->setProp(mtl->getProp() <= 0.95 ? mtl->getProp() + 0.05
                                                : mtl->getProp());
            mtr->setProp(mtr->getProp() <= 0.95 ? mtr->getProp() + 0.05
                                                : mtr->getProp());
            delay(50);
            Serial.println("Circle just pressed");
        }
        if (this->controller->NewButtonState(PSB_CROSS)) {
            // will be TRUE if button was JUST pressed OR
            // released
            mtl->setProp(mtl->getProp() >= 0.05 ? mtl->getProp() - 0.05
                                                : mtl->getProp());
            mtr->setProp(mtr->getProp() >= 0.05 ? mtr->getProp() - 0.05
                                                : mtr->getProp());
            delay(50);
            Serial.println("X just changed");
        }
        if (this->controller->ButtonReleased(
                PSB_SQUARE)) // will be TRUE if button was JUST released
            Serial.println("Square just released");

        Serial.print("Stick Values:");
        Serial.print(this->controller->Analog(PSS_LY),
                     DEC); // Left stick, Y axis. Other options: LX, RY, RX
        Serial.print(",");
        Serial.print(this->controller->Analog(PSS_LX), DEC);
        Serial.print(",");
        Serial.print(this->controller->Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(this->controller->Analog(PSS_RX), DEC);
        mtl->setSpeed(127 - this->controller->Analog(PSS_LY));
        mtr->setSpeed(127 - this->controller->Analog(PSS_RY));
        delay(50);
    }
}