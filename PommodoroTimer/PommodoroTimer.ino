/*
 
 Pommodoro timer

 Press the START/STOP button to start the countdown.

 Sketch based on
 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

#include <stdarg.h>

// include the library code:
#include <LiquidCrystal.h>

enum devicestate {
  pommodoro,
  waiting
};

enum endreason {
  none,
  completed,
  aborted
};

devicestate state = waiting;

unsigned long pommodoro_end_time;
unsigned long waiting_start;

endreason endReason;

#define POMMODORO_MINUTES 25

#define LED_POMMODORO 6
#define LED_WAITING 7

#define SWITCH_START_STOP 9
#define SWITCH_RESERVED 8

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#define DEBOUNCE_TIME_MS 50

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte startSymbol[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
};

class Button {
  // Switch debounce - based on https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
  private:
    int input;
    long lastChangeTime;
    int lastState;

  public:
    Button(int _input) : input(_input) {};

    void init() {
      lastChangeTime = millis();
      lastState = digitalRead(input);
    };

    // Returns true if button state has changed from from to to since last call
    // Limitations: doesn't buffer changes, so it must be called often. If the
    // button is pressed and released between two calls, it's not detected.
    boolean stateChanged(int from, int to) {
      if (millis() >= lastChangeTime + DEBOUNCE_TIME_MS) {
        int newState = digitalRead(input);
        boolean change = (lastState == from && newState == to);
        if (lastState != newState) {
          lastState = newState;
          lastChangeTime = millis();
        }
        return change;
      }
      else {
        // Assume that the HW status is unstable - don't read
        return false;
      }
    }
};

// outputBuffer must have length of LCD_WIDTH+1
void outputBufferClear(char *output) {
  memset(output, ' ', LCD_WIDTH);
  output[LCD_WIDTH] = '\0';
}

void lcdPrintCentered(int line, const char* text) {
  char output[LCD_WIDTH+1];
  outputBufferClear(output);
  int text_len = strlen(text);
  memcpy(output + (LCD_WIDTH-text_len+1)/2, text, text_len);
  lcd.setCursor(0, line);
  lcd.print(output);
}

void lcdPrintfCentered(int line, const char* format, ...) {
  char s[LCD_WIDTH+1];
  va_list ap;
  va_start(ap, format);
  vsnprintf(s, sizeof(s), format, ap);
  s[LCD_WIDTH] = '\0';
  lcdPrintCentered(line, s);
  va_end(ap);
}

void lcdDrawBarGraph(int line, long qty, long qtyMax) {
  char output[LCD_WIDTH+1];
  outputBufferClear(output);
  // 0xFF in default charset: character with all pixels set
  memset(output, '\377', (qty * LCD_WIDTH + qtyMax-1) / qtyMax); // round up
  output[LCD_WIDTH] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(output);
}

void formatTimeUnit(char *s, int s_maxlen, long timeSeconds) {
  long t;
  const char *unit;
  if (timeSeconds > 84600) {
    t = timeSeconds/84600;
    unit = "d";
  } else if (timeSeconds > 3600) {
    t = timeSeconds/3600;
    unit = "h";
  } else if (timeSeconds > 60) {
    t = timeSeconds/60;
    unit = "m";
  } else {
    t = timeSeconds;
    unit = "s";
  }
  snprintf(s, s_maxlen, "%ld%s", t, unit);
}

Button startStopButton(SWITCH_START_STOP);

void setup() {
  lcd.createChar(1, startSymbol);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);

  pinMode(LED_WAITING, OUTPUT);
  pinMode(LED_POMMODORO, OUTPUT);
  pinMode(SWITCH_RESERVED, INPUT);
  pinMode(SWITCH_START_STOP, INPUT);

  startStopButton.init();

  waiting_start = millis();
}

void loop() {
  char output[LCD_WIDTH + 1];
  
  switch (state) {
    case pommodoro:
      digitalWrite(LED_WAITING, LOW);
      digitalWrite(LED_POMMODORO, HIGH);

      // Centered print number of seconds remaining in the current pommodoro
      int secondsRemaining;
      secondsRemaining = (pommodoro_end_time-millis()+999) / 1000;
      lcdPrintfCentered(1, "%d sec", secondsRemaining);
      
      // Draw a bar graph of time remaining
      lcdDrawBarGraph(0, secondsRemaining, POMMODORO_MINUTES*60);

      if (secondsRemaining == 0) {
        state = waiting;
        waiting_start = millis();
        endReason = completed;
      }
      if (startStopButton.stateChanged(LOW, HIGH)) {
        state = waiting;
        waiting_start = millis();
        endReason = aborted;
      }
      break;
      
    case waiting:
      long waitingTimeSec;

      // Blink the green LED
      waitingTimeSec = (millis()-waiting_start) / 1000;
      digitalWrite(LED_WAITING, (waitingTimeSec % 2 != 0) ? HIGH : LOW);
      digitalWrite(LED_POMMODORO, LOW);

      formatTimeUnit(output, sizeof(output), waitingTimeSec);
      if (endReason != none) {
        lcdPrintfCentered(0, "Last: %s", (endReason == completed ? "completed" : "aborted"));
        lcdPrintfCentered(1, "%s ago", output);
      } else {
        lcdPrintfCentered(0, "\001/\377 to start");
        lcdPrintfCentered(1, "%s", output);
      }
     
      if (startStopButton.stateChanged(LOW, HIGH)) {
        pommodoro_end_time = millis() + POMMODORO_MINUTES * 60L * 1000L;
        state = pommodoro;
      }
      break;
  }
}
