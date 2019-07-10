#define abs(a) a > 0 ? a : -a

class Gmotor {
  private:
    int cnt1, cnt2;
    int speed;
    int solid;
    int prop  = 1;
    int direc = 0;

  public:
    Gmotor(int, int); // initial your motor with two pwm output pin num. check
                      // the direction of your motor first
    void stop(int mode);
    void setSolid(int); // when you try to use stop() method, this param decides
                        // the way how your motor stop
    void setSpeed(int); // no more than 255 please
    int  getSpeed();
    void setProp(int);
    void exchangePin(); // exchange the two controlPort
    void gradientTo(int, int);
    void walk(int, int);
};

Gmotor::Gmotor(int cnt1, int cnt2) {
    this->cnt1  = cnt1;
    this->cnt2  = cnt2;
    this->speed = 0;
}

void Gmotor::stop(int mode) {
    if (mode != 0) {
        int conc = this->speed / -60;
        for (int i = 0; i <= 60; i++) {
            this->speed += conc;
            this->setSpeed(speed * prop);
            delay(5);
        }
    }
    analogWrite(cnt1, solid);
    analogWrite(cnt2, solid);
}

void Gmotor::setSolid(int solid) { this->solid = solid; }

void Gmotor::setSpeed(int speed) {
    if ((speed < 0 && this->direc == 0) || (speed > 0 && this->direc == 1))
        this->exchangePin();
    speed       = speed < 0 ? -speed : speed;
    this->speed = speed;
    analogWrite(cnt1, 0);
    analogWrite(cnt2, this->speed * prop);
}

int Gmotor::getSpeed() { return this->speed; }

void Gmotor::setProp(int prop) { this->prop = prop > 1 ? 1 : prop; }

void Gmotor::exchangePin() {
    int t       = this->cnt1;
    this->cnt1  = this->cnt2;
    this->cnt2  = t;
    this->direc = this->direc == 0 ? 1 : 0;
}

void Gmotor::gradientTo(int speed, int ms) {
    double sub          = (double)(speed - this->speed) / (ms / 5);
    double currentSpeed = this->speed;
    for (int i = 1; i <= ms / 5; i++) {
        currentSpeed += sub;
        this->setSpeed((int)currentSpeed);
        // Serial.print("gradientTo: changing speed to ");
        // Serial.println(this->speed);
        delay(5);
    }
    this->setSpeed(speed);
}

void Gmotor::walk(int speed, int ms) {
    this->setSpeed(speed * this->prop);
    delay(ms);
}
