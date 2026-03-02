#include "VFD_AC41.h"

VFD_AC41::VFD_AC41(uint8_t stbPin) {
    STB = stbPin;
    dashState = colon1State = colon2State = false;
    ledState_ = 0;
}

void VFD_AC41::sendCommand(uint8_t cmd) {
    digitalWrite(STB, LOW);
    SPI.transfer(cmd);
    digitalWrite(STB, HIGH);
}

void VFD_AC41::begin() {
    pinMode(STB, OUTPUT);
    digitalWrite(STB, HIGH);

    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
    SPI.setDataMode(SPI_MODE3);
    SPI.setClockDivider(SPI_CLOCK_DIV16);

    sendCommand(0x40);     // write mode
    sendCommand(0x0A);     // 11 digit / 17 segment
    setBrightness(7);      // max
    clear();
}

void VFD_AC41::clear() {
    for(int g=0; g<NUM_GRID; g++)
        for(int b=0; b<NUM_BYTES_PER_GRID; b++)
            displayBuffer[g][b] = 0;
    update();
}

// ------------------- Работа с гридом -------------------
void VFD_AC41::writeGrid(uint8_t gridNum, uint16_t data) {
    if(gridNum >= NUM_GRID) return;
    displayBuffer[gridNum][0] = data & 0xFF;
    displayBuffer[gridNum][1] = (data >> 8) & 0xFF;
}

void VFD_AC41::printDigit(uint8_t gridNum, char c) {
    uint16_t seg = 0;
    if(c >= '0' && c <= '9') seg = VFD_DIGITS[c-'0'];
    else if(c >= 'A' && c <= 'Z') seg = VFD_LETTERS[c-'A'];

    uint16_t current = displayBuffer[gridNum][0] | (displayBuffer[gridNum][1] << 8);
    uint16_t symbolBit = current & 0x0001; // сохраняем тире/двоеточие
    writeGrid(gridNum, seg | symbolBit);
}

void VFD_AC41::printString(const char* str, uint8_t startGrid) {
    for(uint8_t i=0; i<5; i++){
        if(str[i]==0) break;
        printDigit(startGrid+i, str[i]);
    }
}

// ------------------- Двоеточия и тире -------------------
void VFD_AC41::setDash(bool state) {
    if(state) displayBuffer[3][0] |= 0x01;
    else displayBuffer[3][0] &= ~0x01;
}

void VFD_AC41::setColon1(bool state) {
    colon1State = state;
    if(colon1State) displayBuffer[4][0] |= 0x01;
    else displayBuffer[4][0] &= ~0x01;
}

void VFD_AC41::setColon2(bool state) {
    colon2State = state;
    if(colon2State) displayBuffer[6][0] |= 0x01;
    else displayBuffer[6][0] &= ~0x01;
}

// ------------------- Иконки -------------------
void VFD_AC41::iconOn(VFD_ICON icon) {
    setIcon(icon.grid, icon.mask);
}

void VFD_AC41::iconOff(VFD_ICON icon) {
    clearIcon(icon.grid, icon.mask);
}

void VFD_AC41::setIcon(uint8_t grid, uint16_t mask) {
    if(grid >= NUM_GRID) return;
    displayBuffer[grid][0] |= mask & 0xFF;
    displayBuffer[grid][1] |= (mask >> 8) & 0xFF;
}

void VFD_AC41::clearIcon(uint8_t grid, uint16_t mask) {
    if(grid >= NUM_GRID) return;
    displayBuffer[grid][0] &= ~(mask & 0xFF);
    displayBuffer[grid][1] &= ~((mask >> 8) & 0xFF);
}

// ------------------- LED и яркость -------------------
void VFD_AC41::ledWrite(uint8_t ledNum, bool state) {
    if(ledNum > 5) return;
    uint8_t mask = 1 << ledNum;
    ledState_ = (ledState_ & ~mask) | (state ? mask : 0);
    sendCommand(0x44);
    digitalWrite(STB, LOW);
    SPI.transfer(ledState_);
    digitalWrite(STB, HIGH);
}

void VFD_AC41::setBrightness(uint8_t level) {
    if(level > 7) level = 7;
    sendCommand(0x88 | 0x08 | level);
}

// ------------------- Клавиатура -------------------
void VFD_AC41::readKeypad() {
    sendCommand(0x42);
    digitalWrite(STB, LOW);
    for(int i=0; i<6; i++) keyStates_[i] = SPI.transfer(0x00);
    digitalWrite(STB, HIGH);
}

bool VFD_AC41::isPressed(uint8_t key) {
    return keyStates_[key/8] & (1<<(key%8));
}

uint8_t VFD_AC41::readSwitch() {
    sendCommand(0x43);
    digitalWrite(STB, LOW);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(STB, HIGH);
    return val;
}

bool VFD_AC41::switchRead(uint8_t sw) {
    return readSwitch() & (1 << sw);
}

// ------------------- Отправка буфера -------------------
void VFD_AC41::update() {
    sendCommand(0x40);
    digitalWrite(STB, LOW);
    SPI.transfer(0xC0);
    for(int g=0; g<NUM_GRID; g++){
        SPI.transfer(displayBuffer[g][0]);
        SPI.transfer(displayBuffer[g][1]);
        SPI.transfer(displayBuffer[g][2]);
    }
    digitalWrite(STB, HIGH);
}