int counter = 0;
const int buttonIncrement = 9;
const int buttonDecrement = 10;


const int segPins[] = {2,3,4,5,6,7,8};


const byte digitPatterns[10] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

void setup() {
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  
  for(int i=0; i<7; i++){
    pinMode(segPins[i], OUTPUT);
  }
  updateDisplay();
}

void loop() {
  if(digitalRead(10) == HIGH){
    if(counter < 9) counter++;
    updateDisplay();
    delay(300); 
  }
  
  if(digitalRead(9) == HIGH){
    if(counter > 0) counter--;
    updateDisplay();
    delay(300);
  }
}

void updateDisplay(){
  byte pattern = digitPatterns[counter];
  for(int i=0; i<7; i++){
    digitalWrite(segPins[i], bitRead(pattern, i));
  }
}
