#include <VFD_AC41.h>

VFD_AC41 vfd(10);

const char text[] = "   HELLO WORLD     ";
const uint8_t textLength = sizeof(text) - 1;

unsigned long lastDash = 0;
unsigned long lastColon1 = 0;
unsigned long lastColon2 = 0;

bool dashState = false;
bool colon1State = false;
bool colon2State = false;

const unsigned long dashInterval = 300;    // 1 сек
const unsigned long colon1Interval = 200;   // 0.5 сек
const unsigned long colon2Interval = 100;   // 0.7 сек

int scrollOffset = 0;

void setup() {
  vfd.begin();
  vfd.clear();
}

void loop() {
  unsigned long now = millis();

  // --- Моргаем тире/двоеточиями ---
  if (now - lastDash >= dashInterval) {
    dashState = !dashState;
    vfd.setDash(dashState);
    lastDash = now;
  }

  if (now - lastColon1 >= colon1Interval) {
    colon1State = !colon1State;
    vfd.setColon1(colon1State);
    lastColon1 = now;
  }

  if (now - lastColon2 >= colon2Interval) {
    colon2State = !colon2State;
    vfd.setColon2(colon2State);
    lastColon2 = now;
  }

  // --- Бегущая строка ---
  for (int i = 0; i < 5; i++) {
    if (scrollOffset + i < textLength)
      vfd.printDigit(3 + i, text[scrollOffset + i]);
    else
      vfd.printDigit(3 + i, ' '); // пробел, если конец строки
  }

  vfd.update();

  // шаг бегущей строки каждые 300 мс
  static unsigned long lastScroll = 0;
  if (now - lastScroll >= 1000) {
    scrollOffset++;
    if (scrollOffset > textLength - 5) scrollOffset = 0;
    lastScroll = now;
  }
}
