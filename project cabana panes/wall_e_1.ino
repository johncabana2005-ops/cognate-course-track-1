int leftW1_A = 2;
int leftW1_B = 3;
int leftW2_A = 4;
int leftW2_B = 5;
int rightW1_A = 6;
int rightW1_B = 7;
int rightW2_A = 8;
int rightW2_B = 9;
int arm_A = 10;
int arm_B = 11;
int crush_A = 12;
int crush_B = 13;


int trig = A0;
int echo = A1;

int solar = A4;  
int led = A3;


int lightThreshold = 500;    
int solarThreshold = 200;   


int dist = 0;
int lightVal = 0;
int solarVal = 0;
int mode = 0;
int count = 0;
bool solarPower = false;
bool batteryLow = false;

void setup() {
  
  pinMode(leftW1_A, OUTPUT);
  pinMode(leftW1_B, OUTPUT);
  pinMode(leftW2_A, OUTPUT);
  pinMode(leftW2_B, OUTPUT);
  pinMode(rightW1_A, OUTPUT);
  pinMode(rightW1_B, OUTPUT);
  pinMode(rightW2_A, OUTPUT);
  pinMode(rightW2_B, OUTPUT);
  pinMode(arm_A, OUTPUT);
  pinMode(arm_B, OUTPUT);
  pinMode(crush_A, OUTPUT);
  pinMode(crush_B, OUTPUT);
  
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  pinMode(solar, INPUT);
  pinMode(led, OUTPUT);
  
  
  allStop();
  
  Serial.begin(9600);
  Serial.println("WALL-E with Solar Ready!");
  Serial.println("=======================");
}

void loop() {
  
  
  solarVal = analogRead(solar);
  dist = readUltrasonic();
  
  
  solarPower = (solarVal > solarThreshold);
  
  
  if (millis() % 1000 < 50) {
    Serial.print("Light: ");
    Serial.print(lightVal);
    Serial.print(" | Solar: ");
    Serial.print(solarVal);
    Serial.print(" | Dist: ");
    Serial.print(dist);
    Serial.print(" | Solar Power: ");
    Serial.println(solarPower ? "YES" : "NO");
  }
  
 
  if (lightVal > lightThreshold || solarPower) {
    digitalWrite(led, HIGH); 
    
    
    if (mode == 0 || mode == 1) search();
    else if (mode == 2) moveToward();
    else if (mode == 3) collect();
    else if (mode == 4) crush();
  }
  else {
   
    allStop();
    digitalWrite(led, LOW);
    mode = 0;
    Serial.println("Nighttime - Sleeping...");
  }
  
  delay(50);
}



int readUltrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration = pulseIn(echo, HIGH);
  int d = duration / 29 / 2;
  return (d == 0 || d > 200) ? 200 : d;
}

void allStop() {
  digitalWrite(leftW1_A, LOW); digitalWrite(leftW1_B, LOW);
  digitalWrite(leftW2_A, LOW); digitalWrite(leftW2_B, LOW);
  digitalWrite(rightW1_A, LOW); digitalWrite(rightW1_B, LOW);
  digitalWrite(rightW2_A, LOW); digitalWrite(rightW2_B, LOW);
  digitalWrite(arm_A, LOW); digitalWrite(arm_B, LOW);
  digitalWrite(crush_A, LOW); digitalWrite(crush_B, LOW);
}

void forward() {
  digitalWrite(leftW1_A, HIGH); digitalWrite(leftW1_B, LOW);
  digitalWrite(leftW2_A, HIGH); digitalWrite(leftW2_B, LOW);
  digitalWrite(rightW1_A, HIGH); digitalWrite(rightW1_B, LOW);
  digitalWrite(rightW2_A, HIGH); digitalWrite(rightW2_B, LOW);
}

void turnR() {
  digitalWrite(leftW1_A, HIGH); digitalWrite(leftW1_B, LOW);
  digitalWrite(leftW2_A, HIGH); digitalWrite(leftW2_B, LOW);
  digitalWrite(rightW1_A, LOW); digitalWrite(rightW1_B, HIGH);
  digitalWrite(rightW2_A, LOW); digitalWrite(rightW2_B, HIGH);
}

void turnL() {
  digitalWrite(leftW1_A, LOW); digitalWrite(leftW1_B, HIGH);
  digitalWrite(leftW2_A, LOW); digitalWrite(leftW2_B, HIGH);
  digitalWrite(rightW1_A, HIGH); digitalWrite(rightW1_B, LOW);
  digitalWrite(rightW2_A, HIGH); digitalWrite(rightW2_B, LOW);
}

void stopW() {
  digitalWrite(leftW1_A, LOW); digitalWrite(leftW1_B, LOW);
  digitalWrite(leftW2_A, LOW); digitalWrite(leftW2_B, LOW);
  digitalWrite(rightW1_A, LOW); digitalWrite(rightW1_B, LOW);
  digitalWrite(rightW2_A, LOW); digitalWrite(rightW2_B, LOW);
}

void armUp() { digitalWrite(arm_A, HIGH); digitalWrite(arm_B, LOW); }
void armDown() { digitalWrite(arm_A, LOW); digitalWrite(arm_B, HIGH); }
void stopArm() { digitalWrite(arm_A, LOW); digitalWrite(arm_B, LOW); }
void crushStart() { digitalWrite(crush_A, HIGH); digitalWrite(crush_B, LOW); }
void crushStop() { digitalWrite(crush_A, LOW); digitalWrite(crush_B, LOW); }



void search() {
  mode = 1;
  
  if (dist < 30 && dist > 0) {
    mode = 2;
    stopW();
    return;
  }
  
  count++;
  
  if (count < 8) forward();
  else if (count < 12) turnR();
  else if (count < 20) forward();
  else if (count < 24) turnL();
  else count = 0;
  
  delay(200);
}

void moveToward() {
  if (dist > 30 || dist == 0) {
    mode = 1;
    stopW();
    return;
  }
  
  if (dist < 10) {
    stopW();
    mode = 3;
    return;
  }
  
  forward();
  delay(100);
}

void collect() {
  armDown();
  delay(800);
  stopArm();
  
  forward();
  delay(400);
  stopW();
  
  armUp();
  delay(800);
  stopArm();
  
  mode = 4;
}

void crush() {
  crushStart();
  delay(2500);
  crushStop();
  
  mode = 1;
  count = 0;
}