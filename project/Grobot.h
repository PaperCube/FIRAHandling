#include <PS2X_lib.h>
#include <LED.h>
#include <Key.h>
#include <Buzzer.h>
#include <UTFT.h>
#include <Servo.h>
#include "GMotor.h"
typedef unsigned char uc;

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
byte vibrate = 0;

class Grobot
{
private:
    Gmotor *mtl, *mtr;
    uc s1, s2, s3, s4, s5;
    UTFT *GLCD = new UTFT(QD_TFT180A, 51, 52, 32, 34, 33);
    PS2X *controller = new PS2X();

public:
    Grobot();
    void configMotor(Gmotor *, Gmotor *);
    void configSensor(uc, uc, uc, uc, uc);
    void configTFT(byte pos);
    void configTFT(byte, int, int, int, int, byte, int);
    byte configController(uc, uc, uc, uc);
    void configController(uc, uc, uc, uc, bool, bool);

    void walk(int, int);
    void turnLeft();
    void turnRight();
    void stop(int);

    void initialRobot();
    void enterManualMode();
};

Grobot::Grobot()
{
}

void Grobot::configMotor(Gmotor *leftMotor, Gmotor *rightMotor)
{
    this->mtl = leftMotor;
    this->mtr = rightMotor;
}

void Grobot::configSensor(uc s1, uc s2, uc s3, uc s4, uc s5)
{
    this->s1 = s1,
    this->s2 = s2, this->s3 = s3, this->s4 = s4,
    this->s5 = s5;
}

void Grobot::configTFT(byte pos) { this->GLCD->InitLCD(pos); }

void Grobot::configTFT(byte model, int RS, int WR, int CS, int RST, byte pos, int SER = 0)
{
    this->GLCD = new UTFT(model, RS, WR, CS, RST, SER);
    this->GLCD->InitLCD(pos);
}

byte Grobot::configController(uc clk, uc cmd, uc att, uc dat)
{
    return this->controller->config_gamepad(clk, cmd, att, dat);
}

void Grobot::configController(uc clk, uc cmd, uc att, uc dat, bool pressures, bool rumble)
{
    this->controller->config_gamepad(clk, cmd, att, dat, pressures, rumble);
}

void Grobot::initialRobot()
{
    // error = controller->config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    byte type = this->controller->readType();
    //ScreenStart

    GLCD->clrScr();
    GLCD->setBackColor(0, 0, 0);
    GLCD->drawRoundRect(7, 5, 128 - 5, 160 - 5);
    GLCD->setFont(BigFont);
    GLCD->setColor(178, 16, 22);
    GLCD->print("FIRAxZB", 80, 25, 90);
    GLCD->setColor(255, 255, 255);
    GLCD->setFont(SmallFont);
    GLCD->print("Loading.....", 22, 40, 90);
    GLCD->setBackColor(70, 70, 70);
    GLCD->print("VisualDust Design", 60, 14, 90);
    GLCD->setBackColor(0, 0, 0);
    GLCD->setColor(255, 192, 0);
    for (int i = 5; i <= 155; i++)
        GLCD->drawLine(0, i, 3, i),
            delay(13);
    //ScreenStartEnd
    delay(500);
    GLCD->clrScr();
    GLCD->setColor(255, 255, 255);
    GLCD->drawRoundRect(5, 5, 128 - 12, 160 - 5);
    GLCD->print("Searching", 100, 23, 90);
    GLCD->print("Contorller", 86, 19, 90);
    //check if controller is availiable
    for (;;)
    {
        SetLedRGB(1, 0, 0);
        for (int r = 1; r <= 10; r++)
            GLCD->drawCircle(88, 130, r), delay(1);
        GLCD->setColor(0, 0, 0);
        SetLedRGB(0, 0, 1);
        for (int r = 1; r <= 10; r++)
            GLCD->drawCircle(88, 130, r), delay(1);
        GLCD->setColor(255, 255, 255);
        this->controller->read_gamepad();
        if (this->controller->ButtonPressed(PSB_SELECT))
        {
            for (int r = 1; r <= 10; r++)
                GLCD->drawCircle(88, 130, r), delay(1);
            break;
        }
    }
    GLCD->setFont(BigFont);
    GLCD->setColor(255, 255, 255);
    GLCD->setBackColor(0, 176, 80);
    GLCD->print("DONE", 40, 50, 90);
    //check finished
    GLCD->setFont(SmallFont);
    GLCD->setBackColor(0, 0, 0);
    GLCD->setColor(34, 177, 76);
    SetLedRGB(0, 1, 0);
    GLCD->print("Press\"Start\"ToRun", 60, 14, 90);
    for (;;)
    {
        this->controller->read_gamepad();
        if (controller->ButtonPressed(PSB_START) /*  || GetKey(2) z*/)
            break;
        delay(30);
    }
    SetLedRGB(1, 1, 1);
    delay(100);
}

void Grobot::enterManualMode()
{
    for (;;)
    {
        this->controller->read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
        if (this->controller->Button(PSB_START))        //will be TRUE as long as button is pressed
            Serial.println("Start is being held");
        if (this->controller->Button(PSB_SELECT))
            Serial.println("Select is being held");

        if (this->controller->Button(PSB_PAD_UP))
        { //will be TRUE as long as button is pressed
            Serial.print("Up held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_UP), DEC);
        }
        if (this->controller->Button(PSB_PAD_RIGHT))
        {
            Serial.print("Right held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_RIGHT), DEC);
        }
        if (this->controller->Button(PSB_PAD_LEFT))
        {
            Serial.print("LEFT held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_LEFT), DEC);
        }
        if (this->controller->Button(PSB_PAD_DOWN))
        {
            Serial.print("DOWN held this hard: ");
            Serial.println(this->controller->Analog(PSAB_PAD_DOWN), DEC);
        }

        vibrate = this->controller->Analog(PSAB_CROSS); //this will set the large motor vibrate speed based on how hard you press the blue (X) button
        if (this->controller->NewButtonState())
        { //will be TRUE if any button changes state (on to off, or off to on)
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

        if (this->controller->ButtonPressed(PSB_CIRCLE)) //will be TRUE if button was JUST pressed
            Serial.println("Circle just pressed");
        if (this->controller->NewButtonState(PSB_CROSS)) //will be TRUE if button was JUST pressed OR released
            Serial.println("X just changed");
        if (this->controller->ButtonReleased(PSB_SQUARE)) //will be TRUE if button was JUST released
            Serial.println("Square just released");

        if (this->controller->Button(PSB_L1) || this->controller->Button(PSB_R1))
        { //print stick values if either is TRUE
            Serial.print("Stick Values:");
            Serial.print(this->controller->Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
            Serial.print(",");
            Serial.print(this->controller->Analog(PSS_LX), DEC);
            Serial.print(",");
            Serial.print(this->controller->Analog(PSS_RY), DEC);
            Serial.print(",");
            Serial.println(this->controller->Analog(PSS_RX), DEC);
            mtl->setSpeed(127 - this->controller->Analog(PSS_LY));
            mtr->setSpeed(127 - this->controller->Analog(PSS_RY));
        }
        delay(50);
    }
}