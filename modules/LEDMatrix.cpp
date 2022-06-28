#include "LEDMatrix.h"
#include <wiringPi.h>
#include <iostream>


using namespace std;

#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15


LEDMatrix::LEDMatrix(int din, int load, int clk) {
    this->din = din;
    this->load = load;
    this->clk = clk;

    pinMode(din, OUTPUT);
    pinMode(load, OUTPUT);
    pinMode(clk, OUTPUT);

    setScanLimit(7);
    setDisplayTestMode(false);
    setShutdownMode(false);
    setDecodeMode(false);
    clearDisplay();
}

LEDMatrix::~LEDMatrix() {
    clearDisplay();
}

int LEDMatrix::getBrightness() const {
    return this->brightness;
}

void LEDMatrix::setBrightness(int intensity) {
    this->brightness = intensity;
    send(OP_INTENSITY, intensity);
}

int LEDMatrix::getScanLimit() const {
    return this->scanLimit;
}

void LEDMatrix::setScanLimit(int sLimit) {
    this->scanLimit = sLimit;
    send(OP_SCANLIMIT, sLimit);
}

bool LEDMatrix::isDecodeMode() const {
    return this->decodeMode;
}

void LEDMatrix::setDecodeMode(bool decode) {
    this->decodeMode = decode;
    send(OP_DECODEMODE, decode);
}

bool LEDMatrix::isDisplayTestMode() const {
    return this->displayTestMode;
}

void LEDMatrix::setDisplayTestMode(bool displayTest) {
    this->displayTestMode = displayTest;
    send(OP_DISPLAYTEST, displayTest);
}

bool LEDMatrix::isShutdownMode() const {
    return this->shutdownMode;
}

void LEDMatrix::setShutdownMode(bool shutdown) {
    this->shutdownMode = shutdown;
    send(OP_SHUTDOWN, !shutdown);
}

void LEDMatrix::clearDisplay() {
    for (int i = 1; i <= 8; i++) {
        send(i, 0x00);
        delay(10);
    }
    delay(1000);
}


void LEDMatrix::sendByte(char data) {
    for (int i = 8; i > 0; i--) {
        char bitMask = 1 << (i - 1);
        digitalWrite(clk, LOW);

        if (data & bitMask) {
            digitalWrite(din, HIGH);
        } else {
            digitalWrite(din, LOW);
        }

        digitalWrite(clk, HIGH);
    }
}

void LEDMatrix::send(char opcode, char segment) {
    digitalWrite(load, LOW);

    sendByte(opcode);
    sendByte(segment);

    digitalWrite(load, HIGH);
}

int LEDMatrix::getSmiley(long distance) {

    // Größer als die beiden Schwellen
    if (distance > limit[1] && this->lastSmiley != 0) {
        lastSmiley = 0;
        return 0;

        // Kleiner als die zweite Schwelle und größer als die erste Schwelle
    } else if ((distance >= limit[0] && distance <= limit[1]) && this->lastSmiley != 1) {
        return 1;

        // Kleiner als die erste Schwelle
    } else if (distance < limit[0] && this->lastSmiley != 2) {
        return 2;
    }

    // Keine Änderung
    return -1;
}

void LEDMatrix::show(long distance_CM) {
    int smiley = getSmiley(distance_CM);

    if (smiley != -1) {
        lastSmiley = smiley;;
        for (int i = 1; i <= 8; i++) {
            send(i, smileys[lastSmiley][i - 1]);
            delay(50);
        }
    }
}
