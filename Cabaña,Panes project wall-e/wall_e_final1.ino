// WALL-E - MASTER (Sensors + Buttons)
// Day/Night detection using solar cell

// ULTRASONIC
int trigF = 10;
int echoF = 11;
int trigL = 12;
int echoL = 13;
int trigR = A0;
int echoR = A1;
int trigB = A2;
int echoB = A3;

// 3 BUTTONS
int btnGlass = 7;
int btnMetal = 8;
int btnPlastic = 9;

// SENSORS
int lightSensor = A4;
int solarSensor = A5;

// COMMANDS TO ARDUINO 2
int cmd0 = 4;
int cmd1 = 5;
int cmd2 = 6;
int cmd3 = 2;

// VARIABLES
int distFront, distLeft, distRight, distBack;
int lightVal, solarVal;
int mode = 0;
int count = 0;

// DAY/NIGHT DETECTION
bool isDaytime = false;
int nightThreshold = 100;  // Below this = NIGHT

bool btnGlassPressed, btnMetalPressed, btnPlasticPressed;
int glassCount = 0, metalCount = 0, plasticCount = 0, totalCrushed = 0;
bool solarPowering = false;

int lightThreshold = 300;
int solarThreshold = 200;
int distanceThreshold = 30;

String currentWaste = "None";
String statusMsg = "Searching...";
String systemStatus = "OFF";  // Track system ON/OFF

// COMMANDS
const int CMD_STOP = 0;
const int CMD_FORWARD = 1;
const int CMD_BACKWARD = 2;
const int CMD_TURN_R = 3;
const int CMD_TURN_L = 4;
const int CMD_ARM_UP = 5;
const int CMD_ARM_DOWN = 6;
const int CMD_CRUSH_ON = 7;
const int CMD_CRUSH_OFF = 8;

void setup() {
  pinMode(trigF, OUTPUT); pinMode(echoF, INPUT);
  pinMode(trigL, OUTPUT); pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT); pinMode(echoR, INPUT);
  pinMode(trigB, OUTPUT); pinMode(echoB, INPUT);

  pinMode(btnGlass, INPUT_PULLUP);
  pinMode(btnMetal, INPUT_PULLUP);
  pinMode(btnPlastic, INPUT_PULLUP);

  pinMode(cmd0, OUTPUT);
  pinMode(cmd1, OUTPUT);
  pinMode(cmd2, OUTPUT);
  pinMode(cmd3, OUTPUT);

  sendCommand(CMD_STOP);

  Serial.begin(9600);
  Serial.println("MASTER - Ready!");
  Serial.println("====================");
  Serial.println("System uses SOLAR to detect DAY/NIGHT");
  Serial.println("====================");
}

void loop() {
  // Read sensors
  distFront = readUltrasonic(trigF, echoF);
  distLeft = readUltrasonic(trigL, echoL);
  distRight = readUltrasonic(trigR, echoR);
  distBack = readUltrasonic(trigB, echoB);

  lightVal = analogRead(lightSensor);
  solarVal = analogRead(solarSensor);
  solarPowering = (solarVal > solarThreshold);

  // DAY/NIGHT DETECTION using solar cell
  if (solarVal > nightThreshold) {
    isDaytime = true;
    systemStatus = "ON";
  }
  else {
    isDaytime = false;
    systemStatus = "OFF";
  }

  // Read buttons
  btnGlassPressed = (digitalRead(btnGlass) == LOW);
  btnMetalPressed = (digitalRead(btnMetal) == LOW);
  btnPlasticPressed = (digitalRead(btnPlastic) == LOW);

  // Print status
  Serial.print("Solar:"); Serial.print(solarVal);
  Serial.print(" | Light:"); Serial.print(lightVal);
  Serial.print(" | Mode:"); Serial.print(systemStatus);
  Serial.print(" | F:"); Serial.print(distFront);
  Serial.print(" G:"); Serial.print(btnGlassPressed);
  Serial.print(" M:"); Serial.print(btnMetalPressed);
  Serial.print(" P:"); Serial.print(btnPlasticPressed);
  Serial.print(" | "); Serial.println(statusMsg);

  // === DAY/NIGHT CHECK ===
  if (!isDaytime) {
    // NIGHT TIME - System OFF
    statusMsg = "NIGHT TIME - OFF";
    sendCommand(CMD_STOP);
    mode = 0;
  }
  else {
    // DAY TIME - System ON
    statusMsg = "Searching...";

    // Button logic
    if (btnGlassPressed) {
      currentWaste = "Glass";
      glassCount++;
      statusMsg = "GLASS!";
      mode = 3;
    }
    else if (btnMetalPressed) {
      currentWaste = "Metal";
      metalCount++;
      statusMsg = "METAL!";
      mode = 3;
    }
    else if (btnPlasticPressed) {
      currentWaste = "Plastic";
      plasticCount++;
      statusMsg = "PLASTIC!";
      mode = 3;
    }

    // Run behavior only during day
    if (lightVal > lightThreshold || solarPowering) {
      if (mode == 0 || mode == 1) searchMode();
      else if (mode == 2) moveToward();
      else if (mode == 3) collectWaste();
      else if (mode == 4) crushWaste();
    }
    else {
      sendCommand(CMD_STOP);
      mode = 0;
      statusMsg = "Waiting for power...";
    }
  }

  delay(50);
}

// Send command to Arduino 2
void sendCommand(int cmd) {
  digitalWrite(cmd0, (cmd & 1) ? HIGH : LOW);
  digitalWrite(cmd1, (cmd & 2) ? HIGH : LOW);
  digitalWrite(cmd2, (cmd & 4) ? HIGH : LOW);
  digitalWrite(cmd3, (cmd & 8) ? HIGH : LOW);
}

// Ultrasonic
int readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH);
  int d = duration / 29 / 2;
  return (d == 0 || d > 200) ? 200 : d;
}

// Behaviors
void searchMode() {
  mode = 1;
  if (distFront < distanceThreshold && distFront > 0) {
    mode = 2;
    sendCommand(CMD_STOP);
    return;
  }
  if (distLeft < distanceThreshold && distLeft > 0) {
    sendCommand(CMD_TURN_L);
    delay(300);
    sendCommand(CMD_STOP);
    return;
  }

  count++;
  if (count < 8) sendCommand(CMD_FORWARD);
  else if (count < 12) sendCommand(CMD_TURN_R);
  else if (count < 20) sendCommand(CMD_FORWARD);
  else if (count < 24) sendCommand(CMD_TURN_L);
  else count = 0;
  delay(200);
}

void moveToward() {
  if (distFront > distanceThreshold || distFront == 0) {
    mode = 1;
    sendCommand(CMD_STOP);
    return;
  }
  if (distFront < 10) {
    sendCommand(CMD_STOP);
    mode = 3;
    return;
  }
  sendCommand(CMD_FORWARD);
  delay(100);
}

void collectWaste() {
  if (currentWaste == "None") { mode = 1; return; }
  
  sendCommand(CMD_ARM_DOWN);
  delay(800);
  sendCommand(CMD_STOP);
  
  sendCommand(CMD_FORWARD);
  delay(400);
  sendCommand(CMD_STOP);
  
  sendCommand(CMD_ARM_UP);
  delay(800);
  sendCommand(CMD_STOP);
  
  mode = 4;
}

void crushWaste() {
  if (currentWaste == "Glass") {
    sendCommand(CMD_CRUSH_ON);
    delay(3000);
    sendCommand(CMD_CRUSH_OFF);
  }
  else if (currentWaste == "Metal") {
    sendCommand(CMD_CRUSH_ON);
    delay(2000);
    sendCommand(CMD_CRUSH_OFF);
  }
  else if (currentWaste == "Plastic") {
    sendCommand(CMD_CRUSH_ON);
    delay(1500);
    sendCommand(CMD_CRUSH_OFF);
  }

  totalCrushed++;
  Serial.print("Total crushed: ");
  Serial.println(totalCrushed);

  currentWaste = "None";
  mode = 1;
  count = 0;
}