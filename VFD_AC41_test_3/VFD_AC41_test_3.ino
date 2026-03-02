#include <VFD_AC41.h>

VFD_AC41 vfd(10);

// Левый спектр
VFD_ICON spectrumL[] = {
  L_ICON,
  L10DB, L20DB, L30DB, L40DB,
  L50DB, L60DB, L70DB
};

// Правый спектр
VFD_ICON spectrumR[] = {
  R_ICON,
  R10DB, R20DB, R30DB, R40DB,
  R50DB, R60DB, R70DB
};

int currentL = 0;
int currentR = 0;

int peakL = 0;
int peakR = 0;

unsigned long lastPeakTimeL = 0;
unsigned long lastPeakTimeR = 0;

const int peakHoldDelay = 150;   // задержка перед падением пика
const int dropSpeed = 1;        // скорость падения

void setup() {
  vfd.begin();
  vfd.clear();
  randomSeed(analogRead(A0));
}

void loop() {

  // случайный уровень 0-7
  int targetL = random(-1, 9);
  int targetR = random(-1, 9);

  // плавное движение
  if(currentL < targetL) currentL++;
  else if(currentL > targetL) currentL--;

  if(currentR < targetR) currentR++;
  else if(currentR > targetR) currentR--;

  unsigned long now = millis();

  // обновляем пик L
  if(currentL > peakL) {
    peakL = currentL;
    lastPeakTimeL = now;
  } 
  else if(peakL > 0 && now - lastPeakTimeL > peakHoldDelay) {
    peakL -= dropSpeed;
    if(peakL < 0) peakL = 0;
    lastPeakTimeL = now;
  }

  // обновляем пик R
  if(currentR > peakR) {
    peakR = currentR;
    lastPeakTimeR = now;
  } 
  else if(peakR > 0 && now - lastPeakTimeR > peakHoldDelay) {
    peakR -= dropSpeed;
    if(peakR < 0) peakR = 0;
    lastPeakTimeR = now;
  }

  // гасим всё кроме L/R базового сегмента
  for(int i = 1; i < 8; i++) {
    vfd.iconOff(spectrumL[i]);
    vfd.iconOff(spectrumR[i]);
  }

  // зажигаем уровень
  for(int i = 1; i <= currentL; i++)
    vfd.iconOn(spectrumL[i]);

  for(int i = 1; i <= currentR; i++)
    vfd.iconOn(spectrumR[i]);

  // зажигаем пик
  if(peakL > 0)
    vfd.iconOn(spectrumL[peakL]);

  if(peakR > 0)
    vfd.iconOn(spectrumR[peakR]);

  // L и R всегда горят
  vfd.iconOn(spectrumL[0]);
  vfd.iconOn(spectrumR[0]);

  vfd.update();
  delay(25);
}
