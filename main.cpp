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

// Maximale Laufzeit
const int MAX_RUNTIME_MINUTES = 1;

/*
 *
 * Schrittmotor (Thread 1)
 *
 */
const int GPIO_A1 = 6;
const int GPIO_B1 = 10;
const int GPIO_A2 = 11;
const int GPIO_B2 = 31;

void thread1(Semaphore &sem, bool &isThreadActive) {
    bool active = true;

    sem.p(); // Schutz des kritischen Abschnitts

    StepperMotor sm = StepperMotor(GPIO_A1, GPIO_B1, GPIO_A2, GPIO_B2);
    sm.setForward(true);
    sm.setDelay(1);

    while (active) {
        sm.rotate();

        if (!isThreadActive) {
            active = false;
        }
    }

    sem.v(); // Freigabe des kritischen Abschnitts
}


/*
 *
 * Ultraschallsensor (Thread 2)
 *
 */
const int GPIO_ECHO = 21;
const int GPIO_TRIGGER = 22;

void thread2(Semaphore &sem, bool &isThreadActive, long &distanceCM) {
    bool active = true;

    UltrasonicSensor uS = UltrasonicSensor(GPIO_TRIGGER, GPIO_ECHO);

    while (active) {
        sem.p(); // Schutz des kritischen Abschnitts

        try {
            distanceCM = uS.getDistance();
            cout << distanceCM << "cm" << endl;
        } catch (range_error &e) {}

        sem.v(); // Freigabe des kritischen Abschnitts
        delay(1000);

        if (!isThreadActive) {
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

void thread3(Semaphore &sem, bool &isThreadActive, long &distanceCM) {
    bool active = true;

    LEDMatrix ledMatrix = LEDMatrix(GPIO_DIN, GPIO_LOAD, GPIO_CLK);
    ledMatrix.setBrightness(7);

    while (active) {
        sem.p(); // Schutz des kritischen Abschnitts

        ledMatrix.showSmiley(distanceCM);
        delay(500);

        sem.v(); // Freigabe des kritischen Abschnitts
        delay(1000);

        if (!isThreadActive) {
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

    bool isThreadActive = true;
    auto timeEnd = chrono::system_clock::now() + chrono::minutes(MAX_RUNTIME_MINUTES);
    long distanceCM = 0;

    Semaphore sem = Semaphore(2);

    thread t1 = thread(thread1, ref(sem), ref(isThreadActive));
    thread t2 = thread(thread2, ref(sem), ref(isThreadActive), ref(distanceCM));
    thread t3 = thread(thread3, ref(sem), ref(isThreadActive), ref(distanceCM));

    while (isThreadActive) {
        auto timeNow = chrono::system_clock::now();

        if (timeNow > timeEnd) {
            isThreadActive = false;
        }
    }

    t1.join();
    t2.join();
    t3.join();

    cout << endl << "Automatisch beendet nach " << MAX_RUNTIME_MINUTES << "min." << endl;
    return 0;
}
