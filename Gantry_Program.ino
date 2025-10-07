#include <AccelStepper.h>

// x axis 1353
// y axis 1082

//1310
//1040

// ---------------- Pin Definitions ----------------
// X Axis 
#define X_STEP_PIN 11
#define X_DIR_PIN 12
#define X_EN_PIN 13
#define X_LIM_PIN 8

// Y Axis
#define Y_STEP_PIN 3
#define Y_DIR_PIN 4
#define Y_EN_PIN 5
#define Y_LIM_PIN 2
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

bool xHomed = false;
bool yHomed = false;
bool homingDone = false;

String comdata = "";

void setup() {
  Serial.begin(9600);

  pinMode(X_EN_PIN, OUTPUT);
  pinMode(Y_EN_PIN, OUTPUT);
  digitalWrite(X_EN_PIN, LOW);  // Enable drivers (LOW = active)
  digitalWrite(Y_EN_PIN, LOW);

  pinMode(X_LIM_PIN, INPUT_PULLUP);
  pinMode(Y_LIM_PIN, INPUT_PULLUP);

  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);

  // Move toward limit switches
  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(Y_DIR_PIN, LOW);

  homing();
}


void loop() {
  stepperY.run();
  stepperX.run();
  while (Serial.available() > 0) {          //check for serial data to read
    comdata  = Serial.readStringUntil(0x0A);//read incoming serial string till newline
    delay(10);                              //10ms delay
    comdata.toUpperCase();                  //converts lower case to uppercase (insensitive)
    Serial.println(comdata);                //print 'comdata' to serial monitor
  }
  if (comdata == "HOME"){
    homing();
  }
  if (comdata == "BACK") {
    backOff();
    comdata = ""; 
  }
  stepperX.stop();  
  stepperY.stop();  
}

void homing() {
  stepperX.setAcceleration(2000);
  stepperY.setAcceleration(2000);
  stepperX.setMaxSpeed(500);
  stepperY.setMaxSpeed(500);

  stepperY.move(-10000);
  while(!digitalRead(Y_LIM_PIN)){
    stepperY.run();
  }
  stepperY.stop();
  Serial.println("Y Homed");
  stepperY.setCurrentPosition(-42);
  // X Home
  stepperX.move(-10000);
  while(!digitalRead(X_LIM_PIN)){
    stepperX.run();
  }
  stepperX.stop();
  Serial.println("X Homed");
  stepperX.setCurrentPosition(-43);
  Serial.print("Homing Done!!");
  homingDone = true;
  stepperX.enableOutputs();
  stepperY.enableOutputs();
  backOff();
}

void backOff() {
  Serial.println("Moving BACK...");
  digitalWrite(X_EN_PIN, LOW);  // keep driver active
  digitalWrite(Y_EN_PIN, LOW);
  stepperY.run();
  stepperX.run();
  stepperX.enableOutputs();
  stepperY.enableOutputs();

  stepperX.moveTo(1310);
  stepperY.moveTo(1040);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }
  stepperX.enableOutputs();
  stepperY.enableOutputs();
  Serial.println("BACK done, holding position.");
}
