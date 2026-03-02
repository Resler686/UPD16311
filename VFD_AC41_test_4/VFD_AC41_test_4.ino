#include <VFD_AC41.h>

VFD_AC41 vfd(10);

// ==========================
// --- Спектрум 9 и 10 грид ---
// ==========================
VFD_ICON spectrumL[] = { L_ICON, L10DB, L20DB, L30DB, L40DB, L50DB, L60DB, L70DB };
VFD_ICON spectrumR[] = { R_ICON, R10DB, R20DB, R30DB, R40DB, R50DB, R60DB, R70DB };

int currentL = 0, currentR = 0;
int peakL = 0, peakR = 0;
unsigned long lastPeakTimeL = 0, lastPeakTimeR = 0;
const int peakHoldDelay = 150;   // мс
const int dropSpeed = 1;

// ==========================
// --- Бегущая строка и часы ---
// ==========================
const char text[] = "   HELLO WORLD     ";
int scrollOffset = 0;
unsigned long lastScroll = 0;
bool showText = true;

unsigned long lastTime = 0;
bool colonState = false;
int hours = 22;
int minutes = 18;
int seconds = 0;

void setup() {
  vfd.begin();
  vfd.clear();
  randomSeed(analogRead(A0));
}

void loop() {
  unsigned long now = millis();

  // ==========================
  // --- обновление спектрума ---
  // ==========================
  int targetL = random(-1, 9);
  int targetR = random(-1, 9);

  if(currentL < targetL) currentL++;
  else if(currentL > targetL) currentL--;

  if(currentR < targetR) currentR++;
  else if(currentR > targetR) currentR--;

  // пики
  if(currentL > peakL) { peakL = currentL; lastPeakTimeL = now; }
  else if(peakL > 0 && now - lastPeakTimeL > peakHoldDelay) { peakL -= dropSpeed; if(peakL < 0) peakL=0; lastPeakTimeL=now; }

  if(currentR > peakR) { peakR = currentR; lastPeakTimeR = now; }
  else if(peakR > 0 && now - lastPeakTimeR > peakHoldDelay) { peakR -= dropSpeed; if(peakR < 0) peakR=0; lastPeakTimeR=now; }

  // гасим сегменты кроме базового
  for(int i = 1; i < 8; i++) {
    vfd.iconOff(spectrumL[i]);
    vfd.iconOff(spectrumR[i]);
  }

  for(int i = 1; i <= currentL; i++) vfd.iconOn(spectrumL[i]);
  for(int i = 1; i <= currentR; i++) vfd.iconOn(spectrumR[i]);

  if(peakL > 0) vfd.iconOn(spectrumL[peakL]);
  if(peakR > 0) vfd.iconOn(spectrumR[peakR]);

  vfd.iconOn(spectrumL[0]);
  vfd.iconOn(spectrumR[0]);

  // ==========================
  // --- бегущая строка или часы ---
  // ==========================
  if (showText) {
    if (now - lastScroll >= 300) {
      lastScroll = now;
      scrollOffset++;
      if(scrollOffset > sizeof(text) - 6) {
        scrollOffset = 0;
        showText = false;  // переключаемся на часы
        for(int g=3; g<=7; g++) vfd.printDigit(g, ' ');
      }

      for(int i=0; i<5; i++){
        char c = (scrollOffset+i < sizeof(text)-1) ? text[scrollOffset+i] : ' ';
        vfd.printDigit(3+i, c);
      }
    }
  } else {
    if(now - lastTime >= 1000) {
      lastTime = now;
      colonState = !colonState;
      vfd.setColon2(colonState);

      // считаем время
      seconds++;
      if(seconds >= 60) { seconds=0; minutes++; if(minutes>=60){minutes=0; hours++; if(hours>=24)hours=0;} }

      vfd.printDigit(4, '0'+hours/10);
      vfd.printDigit(5, '0'+hours%10);
      vfd.printDigit(6, '0'+minutes/10);
      vfd.printDigit(7, '0'+minutes%10);
    }
  }

  vfd.update();
  delay(25); // общий тик
}