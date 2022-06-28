#ifndef BSY_ULTRASONICSENSOR_H
#define BSY_ULTRASONICSENSOR_H


class UltrasonicSensor {
public:
    UltrasonicSensor(int trigger, int echo);

    long getDistance();

private:
    long recordLength();

    bool isValid(long distance);

    int trigger;
    int echo;
    long startTime;
    long endTime;
    long duration;
    long duration_CM;
    int minDistance_CM = 2;
    int maxDistance_CM = 400;
};


#endif
