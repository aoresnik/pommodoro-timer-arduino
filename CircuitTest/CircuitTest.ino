/*
 
 Test of the correct circuit assembly: initializes LCD, turns on all LEDs and shows button state on LCD

 Sketch based on
 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

  // print button state; TODO: debounce and test debouncing
  lcd.print(" ");
  lcd.print(digitalRead(8) ? "on  " : "off ");
  lcd.print(digitalRead(9) ? "on  " : "off ");
}
