#ifndef BSY_STEPPERMOTOR_H
#define BSY_STEPPERMOTOR_H

#include <wiringPi.h>

class StepperMotor {
public:
    StepperMotor(int pin1, int pin2, int pin3, int pin4);

    ~StepperMotor();

    int getDelay() const;

    void setDelay(int delayTime);

    void reset();

    bool getForward() const;

    void setForward(bool forward);

    void rotate(int steps = 1);

private:
    int pin1, pin2, pin3, pin4;
    int delayTime = 500;
    int halfStepCounter = 0;
    bool isForward = true;

    const bool steps[8][4] = {
            {HIGH, LOW,  LOW,  LOW},
            {HIGH, HIGH, LOW,  LOW},
            {LOW,  HIGH, LOW,  LOW},
            {LOW,  HIGH, HIGH, LOW},
            {LOW,  LOW,  HIGH, LOW},
            {LOW,  LOW,  HIGH, HIGH},
            {LOW,  LOW,  LOW,  HIGH},
            {HIGH, LOW,  LOW,  HIGH}
    };
};


#endif //BSY_STEPPERMOTOR_H
