//сырой скетч с меню вольтметром и осцилом и частотометром и генератором частоты с шим, добавлен нагрузочный резистор
// включаюшайся от кнопки номер 3 в режиме вольтметра 4 версия
#define USE_SSD1306 // Use I2C OLED screen on SSD1306 chipset
#include "U8g2lib.h"
#include <PWM.h>
int led = 9; // пин генератора
int frequency = 50; // частота  1 - 2000000 (Гц)
int brightness = 25;         // частота ШИМ (0-255)
const uint8_t bufferSize = 128;
const uint8_t pinAnalogIn = A7;
uint8_t buffer[bufferSize];
U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int val = 0;
int val2 = 0;
float val3 = 0;
unsigned long timer;
unsigned long timer2;
unsigned long timer3;
unsigned long timer4;
unsigned long timer5;
unsigned long timer6;
unsigned long timer7;
unsigned long timer8;

byte n = 9;
byte q = 0;
byte w = 3;
byte g = 20;
byte u = 0;
byte e = 0;
byte r = 0;
byte k = 15;
byte t = 0;
byte m = 5;
int z = 100;
byte p = 1;

int Htime = 0;      // переменная целого типа для хранения длительности высокого уровня
int Ltime = 0;      // переменная целого типа для хранения длительности низкого уровня
float Ttime;        // переменная для хранения длительности периода
float rtime;        // переменная для хранения длительности периода
int freq;           // переменная для хранения значения частоты
int pwm;            // переменная для хранения значения частоты
float proc;         // переменная для хранения значения частоты
void setup() {
  InitTimersSafe();

  //  Serial.begin(9600);     // сериал порт для отладки
  pinMode(12, INPUT_PULLUP);  // 1 кнопка от юсб разьема
  pinMode(10, INPUT_PULLUP);  // 2 кнопка от юсб разьема
  pinMode(8, INPUT_PULLUP);   // 3 кнопка от юсб разьема
  pinMode(A7, INPUT);         // пин а7 вольтметр и осцилограф
  pinMode(4, INPUT);          // пин 4 ввод данных частотомер
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);        // пин 13 генератор для проверки осцила и частотомера
  u8g2.begin();               // Инициализируем дисплей
  u8g2.clearBuffer();         // Очищаем память
  u8g2.setFont(u8g2_font_10x20_te);  // Выбираем шрифт   u8g2_font_luBIS08_tr  u8g2_font_ncenR08_tr  u8g2_font_4x6_tr
  u8g2.setCursor(0, 13);      // Указываем положение курсора
  u8g2.print("<  VIBOR  >");  // Выводим строку
  u8g2.setCursor(0, 32);
  u8g2.print("| . . . . .");
  u8g2.sendBuffer();          // Отсылаем данные на дисплей
}
void loop() {
//  tone(13, 40, 50000); // выводим тестовый сгнал частоты на 13 пин 100 герц , шим 50%, длительность 50секунд
  if (digitalRead(12) == 0) { // 1 кнопка от юсб разьема обработка нажатия
    pwmWrite(led, 0);
    SetPinFrequency(led, 0);
    q++;
    u = 0;
    r = 0;
    e = 0;
    timer = millis();
    delay(500);
  }
  if (digitalRead(10) == 0) { // 2 кнопка от юсб разьема обработка нажатия
    p++;
    if (p >= 6) {
      p = 1;
    }
    if (p == 1) {
      z = 100;
      m = 5;
      brightness = 25;  // 10% PWM
      k = 10;
    }
    if (p == 2  && e == 3) {  // пропускаем шаг настройки для осцилографа
      p = 3;
    }
    if (p == 2) {
      m = 5;
      z = 100;
      brightness = 89; // 35% PWM
      k = 35;
    }
    if (p == 3) {
      m = 15;
      z = 300;
      brightness = 127;  // 50% PWM
      k = 50;
    }
    if (p == 4) {
      m = 25;
      z = 500;
      brightness = 191;  // 75% PWM
      k = 75;
    }
    if (p == 5) {
      m = 35;
      z = 700;
      brightness = 229;  // 90% PWM
      k = 90;
    }
    delay(500);
  }
  if (e == 5 && digitalRead(8) == 0) { // 3 кнопка от юсб разьема обработка нажатия
    frequency = frequency + 50;
    if (frequency >= 1050)frequency = 50;
    delay(500);
  }
  if (e == 2 && digitalRead(8) == 0) { // 3 кнопка от юсб разьема включаем нагрузочный резистор
    digitalWrite(9, HIGH);
    delay(500);
  }
  if (e == 2 && digitalRead(8) == 1) { // 3 кнопка от юсб разьема выключаем нагрузочный резистор
    digitalWrite(9, LOW);
  }

  if (u == 0 && q == 1 ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("VOLTMETR");
    u8g2.setCursor(0, 32);
    u8g2.print("| . . . . .");
    u8g2.sendBuffer();
    timer = millis();
    u = 1;
    r = 0;
  }
  if (u == 0 && q == 2 ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("OSCILOGRAF");
    u8g2.setCursor(0, 32);
    u8g2.print(". | . . . .");
    u8g2.sendBuffer();
    timer7 = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 3  ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("CHASTOTOMER");
    u8g2.setCursor(0, 32);
    u8g2.print(". . | . . .");
    u8g2.sendBuffer();

    timer3 = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 4  ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("GENERATOR");

    u8g2.setCursor(0, 32);
    u8g2.print(". . . | . .");
    u8g2.sendBuffer();
    timer4 = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 5  ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("CAN LIN TEST");
    u8g2.setCursor(0, 32);
    u8g2.print(". . . . | .");
    u8g2.sendBuffer();
    timer8 = millis();
     u = 1;
    r = 0;
  }
  if ( u == 0 && q == 6  ) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print("TIMER");
    u8g2.setCursor(0, 32);
    u8g2.print(". . . . . |");
    u8g2.sendBuffer();
    timer6 = millis();
    u = 1;
  }
  if (q >= 7) {
    q = 1;
  }
  ///////////////////////////////////////////////////////////////////////////
  if (r == 1 && e == 2)  {  // короткий путь до вольтметра
     if (millis() - timer7 > 500 ) {
      timer7 = millis();
      voltmetr();
    }
  }
  if (millis() - timer > 3000 && q == 1 && r == 0)  { // если выбран вольтметр
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    voltmetr();
  }
  if (u == 0 && q == 0 && millis() - timer8 > 3000) { // само заходит в вольтметр если ничего не нажато
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    u = 1;
    q = 1;
    voltmetr();
  }
  ////////////////
  if (millis() - timer7 > 3000 && q == 2 && r == 0)  { // если выбран осцилограф
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer7 = millis();
    oscilograf();
  }
  if (r == 1 && e == 3)  {  // короткий путь до осцилограф
    oscilograf();
  }
  ////////////////
  if (millis() - timer3 > 3000 && q == 3 && r == 0)  { // если выбран частотомер
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer3 = millis();
    shastotomer();
  }
  if (r == 1 && e == 4)  {  // короткий путь до частотомер
    shastotomer();
  }


  ////////////////
  if (millis() - timer4 > 3000 && q == 4 && r == 0)  { // если выбран генератор
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer4 = millis();
    generator();
  }
  if (r == 1 && e == 5)  {  // короткий путь до генератор
    generator();
  }


  ////////////////
  if (millis() - timer8 > 3000 && q == 5 && r == 0)  { // если выбран кан лин тест
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer8 = millis();
    can_lin_test();
  }
  if (r == 1 && e == 6)  {  // короткий путь до генератор
    can_lin_test();
  }
}
////////////////////////////////////////////////////////////////////////////////////////
void shastotomer() {  // функция частотомер
  Ttime = Htime + Ltime;
  rtime = Ttime - Ltime;
  proc =  Ttime / 100;
  pwm = rtime / proc;
  freq = 1000000 / Ttime; // вычисляем значение частоты учитывая что время у нас в микросекундах
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print("PWM  Hz     ");
  u8g2.setCursor(0, 32);
  u8g2.print(pwm);
  u8g2.print("   ");
  u8g2.print(freq);
  u8g2.sendBuffer();
  if (millis() - timer5 > 1000) {
    Htime = pulseIn(4, HIGH);   // считываем длительность высокого уровня
    Ltime = pulseIn(4, LOW);     // считываем длительность низкого уровня
    timer5 = millis();
  }
  r = 1;
  e = 4;
}
///////////////////////////////
void generator() {  // функция генератор
  pwmWrite(led, brightness);     //0-255   используйте эту функцию вместо analogWrite
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print(k);
  u8g2.print("  PWM");
  u8g2.setCursor(0, 32);
  u8g2.print(frequency);
  u8g2.print("  Hz");
  u8g2.sendBuffer();
  pwmWrite(led, brightness);
  SetPinFrequency(led, frequency);

  r = 1;
  e = 5;
}
/////////////////////////////
void voltmetr(){  // функция вольтметр
  val = analogRead(A7);
  if(val >= val2 + 4 || val <= val2 - 4){
  val2 = val;
    }
   val3 = (52 / 1023.0) * val2;
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  if (digitalRead(8) == 0 && e == 2)u8g2.print("VOLTMETR + N");
  else u8g2.print("VOLTMETR");
  u8g2.setCursor(0, 32);
  u8g2.print(val3);
  u8g2.print(" volt     ");
  u8g2.sendBuffer();
  r = 1;
  e = 2;
}
////////////////////////
void can_lin_test() {  // кан лин тестер
  val = analogRead(A7);
  Serial.println(val);
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print("CAN LIN TEST");
  u8g2.setCursor(0, 32);
  if (val >= 20 && val <= 35  ) {
    u8g2.print("  < CAN L >     ");
    delay(1000);
    u8g2.sendBuffer();
  }
  if (val >= 90 && val <= 105 ) {
    u8g2.print("  < CAN H >      ");
    u8g2.sendBuffer();
    delay(1000);
  }
  if (val >= 150 && val <= 600 ) {
    u8g2.print("  < LIN >     ");
    u8g2.sendBuffer();
    delay(1000);
  }
  if (val >= 0 && val <= 10 ) {
    u8g2.print("  < TEST >      ");
    u8g2.sendBuffer();
  }
  r = 1;
  e = 6;
}
//////////////////////////////////////
void oscilograf() {  // функция осцилограф
  uint32_t start = micros();
  for (uint8_t x = 0; x < bufferSize; ++x) {
    buffer[x] = map(analogRead(pinAnalogIn), 0, z, u8g2.getDisplayHeight() - 1, 0);
  }
  u8g2.firstPage();
  do {
    drawScreen(start);
  } while (u8g2.nextPage());
  r = 1;
  e = 3;
}
void drawScreen(uint32_t duration) {  // функция для работы осцилографа
  for (int8_t v = 5; v >= 0; --v) {
    uint8_t tickY = u8g2.getDisplayHeight() - u8g2.getDisplayHeight() * v / 5;
    for (uint8_t x = 0; x < u8g2.getDisplayWidth(); x += 8)
      u8g2.drawPixel(x, tickY);
  }
  for (uint8_t x = 1; x < bufferSize; ++x) {
    u8g2.drawLine(x - 1, buffer[x - 1], x, buffer[x]);
  }
  u8g2.setCursor(0, 13);
  u8g2.print(m);
}
