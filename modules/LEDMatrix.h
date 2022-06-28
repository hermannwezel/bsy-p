#ifndef BSY_LEDMATRIX_H
#define BSY_LEDMATRIX_H

#include <iostream>

class LEDMatrix {
public:
    LEDMatrix(int din, int load, int clk);

    ~LEDMatrix();

    void show(long distance_CM);

    void send(unsigned char opcode, unsigned char segment);

    int getBrightness() const;

    void setBrightness(int brightness);

    int getScanLimit() const;

    void setScanLimit(int limit);

    bool isDecodeMode() const;

    void setDecodeMode(bool decodeMode);

    bool isDisplayTestMode() const;

    void setDisplayTestMode(bool displayTestMode);

    bool isShutdownMode() const;

    void setShutdownMode(bool shutdown);

    void clearDisplay();

private:
    int din, load, clk, brightness = 10, scanLimit = 7;
    bool decodeMode = false, displayTestMode = false, shutdownMode = false;

    int getSmiley(long distance);

    void sendByte(unsigned char data);

    int lastSmiley = -2;

    int limit[2] = {6, 8};
    const int smileys[3][8] = {
            {195, 189, 90, 126, 90,  102, 189, 195},
            {195, 189, 90, 126, 66,  126, 189, 195},
            {195, 189, 90, 126, 102, 90,  189, 195},
    };

};


#endif //BSY_LEDMATRIX_H
