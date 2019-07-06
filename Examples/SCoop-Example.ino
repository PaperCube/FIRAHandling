#include "SCoop.h"
defineTask(Task1);
 void Task1::setup()
 {
   pinMode(13, OUTPUT);
 }
 void Task1::loop()
 {
   digitalWrite(13, HIGH);
   sleep(1000);
   digitalWrite(13, LOW);
   sleep(1000);
 }
//快速定义
 defineTaskLoop(Task2)
 {
   digitalWrite(12, HIGH);
   sleep(100);
   digitalWrite(12, LOW);
   sleep(100);
 }
void setup() {
   mySCoop.start();
   pinMode(12, OUTPUT);
 }
void loop()
 {
   yield();
 }