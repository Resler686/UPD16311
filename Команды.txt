


vfd.printDigit(grid, char);       // Выводит цифру или букву на грид 3–7
vfd.printString(str, startGrid);  // Выводит строку (макс 5 символов) начиная с указанного грида 3–7
vfd.setDash(true/false);           // Включает/выключает тире на гриде 3
vfd.setColon1(true/false);         // Включает/выключает первое двоеточие на гриде 4
vfd.setColon2(true/false);         // Включает/выключает второе двоеточие на гриде 6
vfd.setIcon(grid, mask);           // Включает сегменты/иконки по маске на гриде 0–2, 8–10
vfd.testRaw(grid, mask, extra);    // Устанавливает 16-битный символ и третий байт на выбранный грид
vfd.testGrid3(mask, extra);        // Быстрое управление гридом 3 с дополнительными сегментами
vfd.testGrid4(mask, extra);        // Быстрое управление гридом 4 с дополнительными сегментами
vfd.testGrid6(mask, extra);        // Быстрое управление гридом 6 с дополнительными сегментами
vfd.testByte(grid, byteIndex, val);// Устанавливает конкретный байт (0–2) грида на нужное значение
vfd.ledWrite(ledNum, true/false);  // Включает/выключает LED 0–5
vfd.setBrightness(level);           // Устанавливает яркость дисплея 0–7
vfd.clear();                        // Сбрасывает весь дисплей
vfd.update();                       // Отправляет буфер на дисплей
vfd.readKeypad();                   // Считывает состояние клавиатуры
vfd.isPressed(key);                 // Проверяет, нажата ли клавиша
vfd.readSwitch();                   // Считывает состояние переключателей
vfd.switchRead(sw);                 // Проверяет состояние конкретного переключателя

L_ICON    // Буква "L"
L10DB     // Сегмент 10 dB
L20DB     // Сегмент 20 dB
L30DB     // Сегмент 30 dB
L40DB     // Сегмент 40 dB
L50DB     // Сегмент 50 dB
L60DB     // Сегмент 60 dB
L70DB     // Сегмент 70 dB

R_ICON    // Буква "R"
R10DB     // Сегмент 10 dB
R20DB     // Сегмент 20 dB
R30DB     // Сегмент 30 dB
R40DB     // Сегмент 40 dB
R50DB     // Сегмент 50 dB
R60DB     // Сегмент 60 dB
R70DB     // Сегмент 70 dB

ICON_STD           // Надпись STD
ICON_DBL           // Надпись DBL
ICON_S             // Надпись S
ICON_L             // Надпись L
ICON_P             // Надпись P
ICON_CASS90_270    // Кассета под углом 90–270°
ICON_CASS45_225    // Кассета под углом 45–225°
ICON_CASS180_360   // Кассета под углом 180–360°
ICON_CASS135_315   // Кассета под углом 135–315°
ICON_CASSETTE      // Кассета целиком

ICON_VSR           // VSR индикатор
ICON_AUTO          // AUTO индикатор
ICON_PDC           // PDC индикатор
ICON_VPS           // VPS индикатор
ICON_CIRCLE        // Круг
ICON_ARROWS        // Стрелки

ICON_ME            // ME индикатор
ICON_SECAM         // SECAM индикатор
ICON_NT            // NT индикатор
ICON_PB            // PB индикатор
ICON_PAL           // PAL индикатор
ICON_3_58          // 3.58 MHz
ICON_4_43          // 4.43 MHz

ICON_HI_FI         // Надпись HI-FI
ICON_DUAL          // Надпись DUAL
ICON_ST            // Надпись ST
ICON_NICAM         // Надпись NICAM