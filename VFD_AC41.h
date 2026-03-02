#ifndef VFD_AC41_H
#define VFD_AC41_H

#include <Arduino.h>
#include <SPI.h>
#include "VFD_AC41_fonts.h"

#define NUM_GRID 11
#define NUM_BYTES_PER_GRID 3

class VFD_AC41 {
public:
    VFD_AC41(uint8_t stbPin);
    void begin();
    void clear();	

    // Новые команды
    void setIcon(uint8_t grid, uint16_t mask);
    void clearIcon(uint8_t grid, uint16_t mask);

    // Управление цифрами/буквами grid 3-7
    void printDigit(uint8_t gridNum, char c);
    void printString(const char* str, uint8_t startGrid=3);

    // Анимации / иконки grid 0-2, 8-10
    void iconOn(VFD_ICON icon);
    void iconOff(VFD_ICON icon);

    // Тире и двоеточия
    void setDash(bool state);
    void setColon1(bool state);
    void setColon2(bool state);

    // LED
    void ledWrite(uint8_t ledNum, bool state);
    void setBrightness(uint8_t level); // 0-7

    // Клавиатура
    void readKeypad();
    bool isPressed(uint8_t key);
    uint8_t readSwitch();
    bool switchRead(uint8_t sw);

    // Обновление буфера дисплея
    void update();

private:
    uint8_t STB;
    uint8_t displayBuffer[NUM_GRID][NUM_BYTES_PER_GRID];
    uint8_t ledState_;
    uint8_t keyStates_[6];
    bool dashState;
    bool colon1State;
    bool colon2State;

    void sendCommand(uint8_t cmd);
    void writeGrid(uint8_t gridNum, uint16_t data);
};

#endif