/*
 
 Pommodoro timer

 Press the START/STOP button to start the countdown.

 Sketch based on
 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

enum devicestate {
  pommodoro,
  waiting
};

devicestate state = waiting;

unsigned long pommodoro_end_time;

#define POMMODORO_MINUTES 25

#define LED_POMMODORO 6
#define LED_WAITING 7

#define SWITCH_START_STOP 9
#define SWITCH_RESERVED 8

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(LED_WAITING, OUTPUT);
  pinMode(LED_POMMODORO, OUTPUT);
  pinMode(SWITCH_RESERVED, INPUT);
  pinMode(SWITCH_START_STOP, INPUT);
}

void loop() {
  switch (state) {
    case pommodoro:
      digitalWrite(LED_WAITING, LOW);
      digitalWrite(LED_POMMODORO, HIGH);

      // Print number of seconds remaining in the current pommodoro
      char output[6];
      lcd.setCursor(0, 1);
      sprintf(output, "%5ld", (pommodoro_end_time-millis()) / 1000);
      lcd.print(output);

      if (millis() >= pommodoro_end_time) {
        state = waiting;
      }
      break;
    case waiting:
      // Blink the green LED
      digitalWrite(LED_WAITING, ((millis() / 1000) % 2 != 0) ? HIGH : LOW);
      digitalWrite(LED_POMMODORO, LOW);

      if (digitalRead(SWITCH_START_STOP)) {
        pommodoro_end_time = millis() + POMMODORO_MINUTES * 60L * 1000L;
        state = pommodoro;
      }
      break;
  }
}
