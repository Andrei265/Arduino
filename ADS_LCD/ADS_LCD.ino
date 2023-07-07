#include <Adafruit_ADS1015.h>

#define PRIMARY_ADC_PIN_1 A0
#define PRIMARY_ADC_PIN_2 A1
#define PRIMARY_ADC_PIN_3 A2
#define PRIMARY_ADC_PIN_4 A3

#define SECONDARY_ADC_PIN_1 A4
#define SECONDARY_ADC_PIN_2 A5
#define SECONDARY_ADC_PIN_3 A6
#define SECONDARY_ADC_PIN_4 A7

#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

Adafruit_ADS1115 primary_adc;
Adafruit_ADS1115 secondary_adc;

void setup() {
  // Инициализация модулей АЦП
  primary_adc.begin();
  secondary_adc.begin();

  // Инициализация экрана LCD
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_EN, OUTPUT);
  pinMode(LCD_D4, OUTPUT);
  pinMode(LCD_D5, OUTPUT);
  pinMode(LCD_D6, OUTPUT);
  pinMode(LCD_D7, OUTPUT);

  // Установка начального состояния
  bool isPrimaryActive = true;

  // Основной цикл программы
  while (true) {
    if (isPrimaryActive) {
      // Считываем данные с первого АЦП
      int16_t primaryValue1 = primary_adc.readADC_SingleEnded(PRIMARY_ADC_PIN_1);
      int16_t primaryValue2 = primary_adc.readADC_SingleEnded(PRIMARY_ADC_PIN_2);
      int16_t primaryValue3 = primary_adc.readADC_SingleEnded(PRIMARY_ADC_PIN_3);
      int16_t primaryValue4 = primary_adc.readADC_SingleEnded(PRIMARY_ADC_PIN_4);

      // Проверяем наличие аномалии (в данном случае - отключение питания)
      if (primaryValue1 < 0 || primaryValue2 < 0 || primaryValue3 < 0 || primaryValue4 < 0) {
        // Отключение питания на первом АЦП, активация второго АЦП
        // Подключение пина первого АЦП к GND или измените логику проверки на неисправность
        secondary_adc.startComparator_SingleEnded(SECONDARY_ADC_PIN_1, 0);

        // Выводим сообщение на экран LCD
        lcdClear();
        lcdPrint("Primary ADC failed");
        lcdSetCursor(0, 1);
        lcdPrint("Secondary ADC active");

        isPrimaryActive = false;
      } else {
        // Выводим значения с первого АЦП на экран LCD
        lcdClear();
        lcdPrint("Primary ADC:");
        lcdSetCursor(0, 1);
        lcdPrint(primaryValue1);
        lcdPrint(" ");
        lcdPrint(primaryValue2);
        lcdPrint(" ");
        lcdPrint(primaryValue3);
        lcdPrint(" ");
        lcdPrint(primaryValue4);
      }
    } else {
      // Считываем данные с второго АЦП
      int16_t secondaryValue1 = secondary_adc.readADC_SingleEnded(SECONDARY_ADC_PIN_1);
      int16_t secondaryValue2 = secondary_adc.readADC_SingleEnded(SECONDARY_ADC_PIN_2);
      int16_t secondaryValue3 = secondary_adc.readADC_SingleEnded(SECONDARY_ADC_PIN_3);
      int16_t secondaryValue4 = secondary_adc.readADC_SingleEnded(SECONDARY_ADC_PIN_4);

      // Выводим значения со второго АЦП на экран LCD
      lcdClear();
      lcdPrint("Secondary ADC:");
      lcdSetCursor(0, 1);
      lcdPrint(secondaryValue1);
      lcdPrint(" ");
      lcdPrint(secondaryValue2);
      lcdPrint(" ");
      lcdPrint(secondaryValue3);
      lcdPrint(" ");
      lcdPrint(secondaryValue4);
    }

    delay(1000); // Пауза между измерениями
  }
}

// Функция для очистки экрана LCD
void lcdClear() {
  lcdCommand(0x01);
  delayMicroseconds(2000);
}

// Функция для отправки команды на экран LCD
void lcdCommand(uint8_t command) {
  digitalWrite(LCD_RS, LOW);
  sendData(command);
}

// Функция для отправки данных на экран LCD
void lcdPrint(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  sendData(data);
}

// Функция для установки позиции курсора на экране LCD
void lcdSetCursor(uint8_t col, uint8_t row) {
  if (row == 0) {
    lcdCommand(0x80 + col);
  } else if (row == 1) {
    lcdCommand(0xC0 + col);
  }
}

// Вспомогательная функция для отправки данных на экран LCD
void sendData(uint8_t data) {
  digitalWrite(LCD_D4, data & 0x10);
  digitalWrite(LCD_D5, data & 0x20);
  digitalWrite(LCD_D6, data & 0x40);
  digitalWrite(LCD_D7, data & 0x80);
  pulseEnable();
  digitalWrite(LCD_D4, data & 0x01);
  digitalWrite(LCD_D5, data & 0x02);
  digitalWrite(LCD_D6, data & 0x04);
  digitalWrite(LCD_D7, data & 0x08);
  pulseEnable();
}

// Вспомогательная функция для установки сигнала ENABLE на экране LCD
void pulseEnable() {
  digitalWrite(LCD_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(LCD_EN, HIGH);
  delayMicroseconds(1);
  digitalWrite(LCD_EN, LOW);
  delayMicroseconds(100);
}
