#include <UTFT.h>
#include "GMotor.h"
#include <Servo.h>

int a1, a2, a3, a4, a5;
int thr1 = 500,
    thr2 = 500,
    thr3 = 500,
    thr4 = 500,
    thr5 = 500;

int lPin = 3, rPin = 4;

int speed_h = 100,
    speed_l = 30;

void setup()
{
    Serial.begin(9600);
    setMotor(3, 233);
    setMotor(4, 233);
}

void loop()
{
    // Serial.print(analogRead(A1));
    // Serial.print("   |");
    // Serial.print(analogRead(A2));
    // Serial.print("|");
    // Serial.print(analogRead(A3));
    // Serial.print("|");
    // Serial.print(analogRead(A4));
    // Serial.print("|   ");
    // Serial.print(analogRead(A5));
    // Serial.println();
    updateSensorStatus();
    // while (a1 >= thr1 && a5 >= thr5)
    // {
    //     updateSensorStatus();
    //     if (a3 >= thr3)
    //         setMotor(lPin, speed_h), setMotor(rPin, speed_l);
    //     else if (a3 < thr3)
    //         setMotor(rPin, speed_h), setMotor(lPin, speed_l);
    //     delay(50);
    // }
}

void updateSensorStatus()
{
    a1 = analogRead(A1);
    // a2=analogRead(A2);
    a3 = analogRead(A3);
    // a4=analogRead(A4);
    a5 = analogRead(A5);
}