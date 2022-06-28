#include "StepperMotor.h"
#include <wiringPi.h>

StepperMotor::StepperMotor(int a1, int b1, int a2, int b2) {
    this->pin1 = a1;
    this->pin2 = b1;
    this->pin3 = a2;
    this->pin4 = b2;

    pinMode(a1, OUTPUT);
    pinMode(b1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(b2, OUTPUT);

    reset();
}

StepperMotor::~StepperMotor() {
    reset();
}

int StepperMotor::getDelay() const {
    return this->delayTime;
}

void StepperMotor::setDelay(int delay) {
    this->delayTime = delay;
}

bool StepperMotor::getForward() const {
    return this->isForward;
}

void StepperMotor::setForward(bool forward) {
    this->isForward = forward;
}


void StepperMotor::rotate(int stepCount) {
    int pins[4] = {pin1, pin2, pin3, pin4};

    halfStepCounter = this->isForward ? (halfStepCounter + 7) % 8 : (halfStepCounter + 1) % 8;

    for (int s = 0; s < stepCount; s++) {
        for (int i = 0; i < 4; i++) {
            digitalWrite(pins[i], steps[halfStepCounter][i]);
            delay(delayTime);
        }
    }
}

void StepperMotor::reset() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
}
