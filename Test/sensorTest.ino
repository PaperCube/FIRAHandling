#include "Gmotor.h"

Gmotor *mt1 = new Gmotor(2, 3), *mt2 = new Gmotor(5, 4);

int a1, a2, a3, a4, a5;
void setup() { Serial.begin(9600); }

int val = 900;
void loop() {

    updateSensors();
    // Serial.print(a1);
    // Serial.print("     |");
    // Serial.print(a3);
    // Serial.print("|     ");
    // Serial.println(a5);

    while (a1 > val && a5 > val) {
        if (a3 > val)
            mt1->setSpeed(30), mt2->setSpeed(80);
        else if (a3 < val)
            mt1->setSpeed(80), mt2->setSpeed(30);
        updateSensors();
    }
    mt1->stop(0);
    mt2->stop(0);
    delay(1000);
    mt1->setSpeed(70), mt2->setSpeed(70);
    while (a1 > val && a5 > val) {
        updateSensors();
    }
    updateSensors();
    while (!(a1 < val && a5 < val)) {
        if (a1 < val)
            mt1->setSpeed(-170);
        else
            mt1->setSpeed(70);

        if (a5 < val)
            mt2->setSpeed(-170);
        else
            mt2->setSpeed(70);
        delay(15);
        updateSensors();
    }
    mt1->stop(0);
    mt2->stop(0);
    delay(99999999);
}

void updateSensors() {
    a1 = analogRead(A1);
    // a2 = analogRead(A2);
    a3 = analogRead(A3);
    // a4 = analogRead(A4);
    a5 = analogRead(A5);
}