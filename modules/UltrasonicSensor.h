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
    long durationCM;
    int minDistanceCM = 2;
    int maxDistanceCM = 400;
};


#endif
