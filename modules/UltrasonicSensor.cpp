#include <iostream>
#include <wiringPi.h>
#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trigger, int echo) {
    this->trigger = trigger;
    this->echo = echo;

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
    pullUpDnControl(echo, PUD_UP);
}

long UltrasonicSensor::recordLength() {
    while (digitalRead(echo) == LOW);
    startTime = micros();

    while (digitalRead(echo) == HIGH);
    endTime = micros();

    return endTime - startTime;
}

bool UltrasonicSensor::isValid(long distance) {

    if (distance < minDistanceCM || distance > maxDistanceCM) {
        return false;
    }

    return true;
}

long UltrasonicSensor::getDistance() {
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    duration = recordLength();
    durationCM = ((duration * (343.2 / 1000000)) / 2) * 100;

    if (!isValid(durationCM)) {
        throw std::range_error("out of range");
    }

    return durationCM;
}
