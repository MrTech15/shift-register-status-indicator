// ===============================
// Shift Register Indicator Module
// SN74HC595 Status + Behaviour
// ===============================

/*
Example of how you would use in your main project
void loop() {
  updateIndicator();


  if (system is healthy ) {
    setIndicator(GOOD, SOLID);
  }

  if ( warning condition ) {
    setIndicator(OKAY, SLOW);
  }

  if (serious fault) {
    setIndicator(FAULT, FAST);
  }
}
*/

// SN74HC595 pins
const int dataPin  = 11;
const int clockPin = 12;
const int latchPin = 8;

// my LED positions on shift register
// Note: bit(0) = Q0, bit(1) = Q1, bit(2) = Q2
const byte RED_LED    = 1;
const byte YELLOW_LED = 2;
const byte GREEN_LED  = 3;

//create enums for both status and behaviours
enum Status {
  CLEAR,
  GOOD,
  OKAY,
  FAULT
};

enum Behaviour {
  SOLID,
  SLOW,
  FAST
};

//initialize both status and behaviour to clear and solid
Status currentStatus = CLEAR;
Behaviour currentBehaviour = SOLID;

//led blinking booleans
bool blinkOn = true;
unsigned long lastBlinkTime = 0;
//adjust these times (ms) for how fast or slow you want your LED's to blink
const unsigned long slowInterval = 700;
const unsigned long fastInterval = 200;

// Call once in setup()
void indicatorBegin() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  setIndicator(CLEAR, SOLID);
}

// Call this from your main project
void setIndicator(Status status, Behaviour behaviour) {
  currentStatus = status;
  currentBehaviour = behaviour;

  blinkOn = true;
  lastBlinkTime = millis();

  showLED(currentStatus);
}

// Call this repeatedly inside loop()
void updateIndicator() {
  if (currentBehaviour == SOLID || currentStatus == CLEAR) return;

  unsigned long interval;

  if (currentBehaviour == SLOW) {
    interval = slowInterval;
  } else {
    interval = fastInterval;
  }

  if (millis() - lastBlinkTime >= interval) {
    lastBlinkTime = millis();
    blinkOn = !blinkOn;

    if (blinkOn) {
      showLED(currentStatus);
    } else {
      writeShiftRegister(0);
    }
  }
}

void clearIndicator() {
  setIndicator(CLEAR, SOLID);
}

void showLED(Status status) {
  if (status == GOOD) {
    writeShiftRegister(bit(GREEN_LED));
  }
  else if (status == OKAY) {
    writeShiftRegister(bit(YELLOW_LED));
  }
  else if (status == FAULT) {
    writeShiftRegister(bit(RED_LED));
  }
  else {
    writeShiftRegister(0);
  }
}

void writeShiftRegister(byte value) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
}
