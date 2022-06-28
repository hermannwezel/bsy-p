#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <wiringPi.h>
#include "modules/UltrasonicSensor.h"
#include "modules/StepperMotor.h"
#include "modules/LEDMatrix.h"
#include "Semaphore.h"

using namespace std;


/*
 *
 * Zeit abfragen
 *
 */
const int MAX_RUNTIME = 1;
auto timeEnd = chrono::system_clock::now() + chrono::minutes(MAX_RUNTIME);

void checkAlive(bool &isActive) {
    auto timeNow = chrono::system_clock::now();

    if (timeNow > timeEnd) {
        isActive = false;
    }
}


/*
 *
 * StepperMotor (Thread 1)
 *
 */
const int GPIO_A1 = 6;
const int GPIO_B1 = 10;
const int GPIO_A2 = 11;
const int GPIO_B2 = 31;

void thread1(Semaphore &sem, bool &isActive) {
    bool active = true;

    sem.p(); // Schutz des kritischen Abschnitts

    StepperMotor sm = StepperMotor(GPIO_A1, GPIO_B1, GPIO_A2, GPIO_B2);
    sm.setForward(true);
    sm.setDelay(1);

    while (active) {
        sm.rotate();
        checkAlive(isActive); // Schaut ob die Zeit abgelaufen ist

        if (!isActive) {
            active = false;
        }
    }

    sem.v(); // Freigabe des kritischen Abschnitts
}


/*
 *
 * Ultraschall Abstandsensor (Thread 2)
 *
 */
const int GPIO_ECHO = 21;
const int GPIO_TRIGGER = 22;

void thread2(Semaphore &sem, bool &isActive, long &distance_CM) {
    bool active = true;

    UltrasonicSensor uS = UltrasonicSensor(GPIO_TRIGGER, GPIO_ECHO);

    while (active) {
        sem.p(); // Schutz des kritischen Abschnitts

        distance_CM = uS.getDistance();
        if (distance_CM > 0) {
            cout << distance_CM << "cm" << endl;
        }

        sem.v(); // Freigabe des kritischen Abschnitts
        delay(1000);

        if (!isActive) {
            active = false;
        }
    }
}


/*
 *
 * LED Matrix (Thread 3)
 *
 */
const int GPIO_DIN = 27;
const int GPIO_LOAD = 28;
const int GPIO_CLK = 29;

void thread3(Semaphore &sem, bool &isActive, long &distance_CM) {
    bool active = true;

    LEDMatrix ledMatrix = LEDMatrix(GPIO_DIN, GPIO_LOAD, GPIO_CLK);
    ledMatrix.setBrightness(7);

    while (active) {
        sem.p(); // Schutz des kritischen Abschnitts

        if (distance_CM > 0) {
            ledMatrix.show(distance_CM);
            delay(1000);
        }

        sem.v(); // Freigabe des kritischen Abschnitts
        delay(1000);

        if (!isActive) {
            active = false;
        }
    }
}


/*
 *
 * Main
 *
 */
int main() {
    if (wiringPiSetup() == -1) {
        return -1;
    }

    bool isActive = true;
    long distance_CM = -1;
    Semaphore sem = Semaphore(2);

    thread t1 = thread(thread1, ref(sem), ref(isActive));
    thread t2 = thread(thread2, ref(sem), ref(isActive), ref(distance_CM));
    thread t3 = thread(thread3, ref(sem), ref(isActive), ref(distance_CM));

    t1.join();
    t2.join();
    t3.join();

    cout << endl << "Automatisch beendet nach " << MAX_RUNTIME << "min" << endl;
    return 0;
}
