#include <VFD_AC41.h>

VFD_AC41 vfd(10);

// --- бегущая строка ---
const char text[] = "   HELLO WORLD     ";
int scrollOffset = 0;
unsigned long lastScroll = 0;
bool showText = true;

// --- часы ---
unsigned long lastTime = 0;
bool colonState = false;
int hours = 22;   // начальное время
int minutes = 18;
int seconds = 0;

void setup() {
  vfd.begin();
  vfd.clear();
}

void loop() {
  unsigned long now = millis();

  if (showText) {
    // --- бегущая строка каждые 300 мс ---
    if (now - lastScroll >= 1000) {
      lastScroll = now;
      scrollOffset++;
      if (scrollOffset > sizeof(text) - 6) { // конец строки
        scrollOffset = 0;
        showText = false; // переключаемся на часы
        // очищаем гриды 3–7 перед показом часов
        for (int g = 3; g <= 7; g++) vfd.printDigit(g, ' ');
      }

      // выводим 5 символов на гриды 3-7
      for (int i = 0; i < 5; i++) {
        char c = (scrollOffset + i < sizeof(text) - 1) ? text[scrollOffset + i] : ' ';
        vfd.printDigit(3 + i, c);
      }
      vfd.update();
    }
  } else {
    // --- моргающее двоеточие на грид 6 ---
    if (now - lastTime >= 1000) {
      lastTime = now;
      colonState = !colonState;
      vfd.setColon2(colonState);

      // считаем время
      seconds++;
      if (seconds >= 60) {
        seconds = 0;
        minutes++;
        if (minutes >= 60) {
          minutes = 0;
          hours++;
          if (hours >= 24) hours = 0;
        }
      }

      // обновляем часы на гриды 4-7
      vfd.printDigit(4, '0' + hours / 10);
      vfd.printDigit(5, '0' + hours % 10);
      vfd.printDigit(6, '0' + minutes / 10);
      vfd.printDigit(7, '0' + minutes % 10);

      vfd.update();
    }
  }
}