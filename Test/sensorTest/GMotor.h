int param = 1;
void setMotor(int pinNum, int speed)
{
    speed *= param;
    if (pinNum == 1)
        digitalWrite(48, HIGH),
            digitalWrite(49, LOW),
            analogWrite(13, speed);
    else if (pinNum == 2)
        digitalWrite(46, HIGH),
            digitalWrite(47, LOW),
            analogWrite(12, speed);
    else if (pinNum == 3)
        digitalWrite(45, LOW),
            digitalWrite(44, HIGH),
            analogWrite(11, speed);
    else if (pinNum == 4)
        digitalWrite(43, LOW),
            digitalWrite(42, HIGH),
            analogWrite(10, speed);
    else if (pinNum == 5)
        digitalWrite(40, LOW),
            digitalWrite(41, HIGH),
            analogWrite(9, speed);
    else if (pinNum == 6)
        digitalWrite(38, LOW),
            digitalWrite(39, LOW),
            analogWrite(8, speed);
}