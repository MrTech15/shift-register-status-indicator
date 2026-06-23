// ===============================
// Shift Register Indicator Module
// SN74HC595 Status + Behaviour
// ===============================

/*
Run commands like: 
fault solid
okay fast
good slow
*/

// SN74HC595 pins
const int dataPin  = 11;
const int clockPin = 12;
const int latchPin = 8;

// LED positions
const byte RED_LED    = 1;  // Q0
const byte YELLOW_LED = 2;  // Q1
const byte GREEN_LED  = 3;  // Q2

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

Status currentStatus = CLEAR;
Behaviour currentBehaviour = SOLID;

bool blinkOn = true;
unsigned long lastBlinkTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  setIndicator(CLEAR, SOLID);

  Serial.println("Commands:");
  Serial.println("good solid");
  Serial.println("okay SLOW");
  Serial.println("fault FAST");
  Serial.println("clear");
}

void loop() {
  readSerial();
  updateBlink();
}

//recieving serial commands
void readSerial() {
  if (Serial.available() == 0) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "good solid") {
    setIndicator(GOOD, SOLID);
  }
  else if (cmd == "okay solid") {
    setIndicator(OKAY, SOLID);
  }
  else if (cmd == "fault solid") {
    setIndicator(FAULT, SOLID);
  }
  else if (cmd == "good slow") {
    setIndicator(GOOD, SLOW);
  }
  else if (cmd == "okay slow") {
    setIndicator(OKAY, SLOW);
  }
  else if (cmd == "fault slow") {
    setIndicator(FAULT, SLOW);
  }
  else if (cmd == "good fast") {
    setIndicator(GOOD, FAST);
  }
  else if (cmd == "okay fast") {
    setIndicator(OKAY, FAST);
  }
  else if (cmd == "fault fast") {
    setIndicator(FAULT, FAST);
  }
  else if (cmd == "clear") {
    setIndicator(CLEAR, SOLID);
  }
  else if (cmd == "test") {
    runTest();
  }
  else {
    Serial.println("Unknown command");
  }
}

//test utility
void runTest() {
  Serial.println("Running Test...");

  setIndicator(FAULT, SOLID);
  delay(400);

  setIndicator(OKAY, SOLID);
  delay(400);

  setIndicator(GOOD, SOLID);
  delay(400);

  clearIndicator();

  Serial.println("Test complete");
}

//set the status and led indicator
void setIndicator(Status status, Behaviour behaviour) {
  currentStatus = status;
  currentBehaviour = behaviour;
  blinkOn = true;
  lastBlinkTime = millis();
  showLED(currentStatus);
}

//clear the indicator
void clearIndicator(){
  currentStatus = CLEAR;
  currentBehaviour = SOLID;
  writeShiftRegister(0);
}

//blinking logic
void updateBlink() {
  if (currentBehaviour == SOLID || currentStatus == CLEAR) return;

  unsigned long interval;

  if (currentBehaviour == SLOW) {
    interval = 700;
  } 
  else {
    interval = 200;
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

//allocate the right colours
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

//talk to shift register
void writeShiftRegister(byte value) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
}
