#include <Keypad.h>


const int segA = 10;
const int segB = 11;
const int segC = 12;
const int segD = 13;
const int segE = A0;
const int segF = A1;
const int segG = A2;
const int segDP = A3;


const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};    // R1, R2, R3, R4
byte colPins[COLS] = {5, 4, 3, 2};   // C1, C2, C3, C4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


const byte numberSegments[10] = {
  B11111100,  // 0: a,b,c,d,e,f
  B01100000,  // 1: b,c
  B11011010,  // 2: a,b,d,e,g
  B11110010,  // 3: a,b,c,d,g
  B01100110,  // 4: b,c,f,g
  B10110110,  // 5: a,c,d,f,g
  B10111110,  // 6: a,c,d,e,f,g
  B11100000,  // 7: a,b,c
  B11111110,  // 8: all segments
  B11110110   // 9: a,b,c,d,f,g
};


const byte letterSegments[6] = {
  B11101110,  // A: a,b,c,e,f,g
  B00111110,  // b: c,d,e,f,g
  B10011100,  // C: a,d,e,f
  B01111010,  // d: b,c,d,e,g
  B10011110,  // E: a,d,e,f,g
  B10001110   // F: a,e,f,g
};

void setup() {
  
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segDP, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    Serial.println(key);
    
    if (key >= '0' && key <= '9') {
      displayNumber(key - '0');
    }
    else if (key >= 'A' && key <= 'F') {
      displayLetter(key - 'A');
    }
    else if (key == '*') {
      displayClear();  // Clear display
    }
    else if (key == '#') {
      displayNumber(8);  // Show 8 as test
    }
    else {
      displayClear();  // Clear for any other key
    }
  }
}

void displayNumber(int num) {
  if (num >= 0 && num <= 9) {
    digitalWrite(segA, bitRead(numberSegments[num], 7));
    digitalWrite(segB, bitRead(numberSegments[num], 6));
    digitalWrite(segC, bitRead(numberSegments[num], 5));
    digitalWrite(segD, bitRead(numberSegments[num], 4));
    digitalWrite(segE, bitRead(numberSegments[num], 3));
    digitalWrite(segF, bitRead(numberSegments[num], 2));
    digitalWrite(segG, bitRead(numberSegments[num], 1));
    digitalWrite(segDP, bitRead(numberSegments[num], 0));
  }
}

void displayLetter(int let) {
  if (let >= 0 && let <= 5) {
    digitalWrite(segA, bitRead(letterSegments[let], 7));
    digitalWrite(segB, bitRead(letterSegments[let], 6));
    digitalWrite(segC, bitRead(letterSegments[let], 5));
    digitalWrite(segD, bitRead(letterSegments[let], 4));
    digitalWrite(segE, bitRead(letterSegments[let], 3));
    digitalWrite(segF, bitRead(letterSegments[let], 2));
    digitalWrite(segG, bitRead(letterSegments[let], 1));
    digitalWrite(segDP, bitRead(letterSegments[let], 0));
  }
}

void displayClear() {
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  digitalWrite(segDP, LOW);
}
