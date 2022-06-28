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

    if (distance < minDistance_CM || distance > maxDistance_CM) {
        return false;
    }

    return true;
}

long UltrasonicSensor::getDistance() {
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    duration = recordLength();
    duration_CM = ((duration * (343.2 / 1000000)) / 2) * 100;

    if (!isValid(duration_CM)) {
        return -1;
    }

    return duration_CM;
}
