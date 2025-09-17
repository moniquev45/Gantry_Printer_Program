#define X_STEP_PIN 20    // Pulse pin (PUL+)
#define X_DIR_PIN 19     // Direction pin (DIR+)
#define X_ENA_PIN 18     // Enable pin (ENA+) - optional

#define Y_STEP_PIN 17    // Pulse pin (PUL+)
#define Y_DIR_PIN 16     // Direction pin (DIR+)
#define Y_ENA_PIN 15    // Enable pin (ENA+) - optional

#define X_Limit_Switch 14 
#define Y_Limit_Switch 13

String serialData = "";

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

void setup() {
  // put your setup code here, to run once:
  initialise()
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {          //check for serial data to read
    serialData  = Serial.readStringUntil(0x0A);//read incoming serial string till newline
    serialData  = Serial.readStringUntil(0x0A);//read incoming serial string till newline
    delay(10);                              //10ms delay
    comdata.toLowerCase();                  //converts lower case to lowerrcase (insensitive)
    Serial.println(serialData);
  }

  if (serialData == "home"){
    Serial.println("Homing Starting....");
    homingSequence(stepperX, X_LIMIT_PIN, stepperY, Y_LIMIT_PIN);
    Serial.println("Homing Done :)");
    serialData = "";
  }

  if (serialData == "overall_x_steps"){
    Serial.println("Calculating Steps....");
    homingSequence(stepperX, X_LIMIT_PIN, stepperY, Y_LIMIT_PIN);
    howManySteps(1); //Instead of x and y i just did 1 and 0 cause i felt like it if u dont like it bite me.
    serialData = "";
  }

  if (serialData == "overall_y_steps"){
    Serial.println("Calculating Steps....");
    homingSequence(stepperX, X_LIMIT_PIN, stepperY, Y_LIMIT_PIN);
    howManySteps(0); //Instead of x and y i just did 1 and 0 cause i felt like it if u dont like it bite me.
    serialData = "";
  }
}

//BASIC FUNCTIONS

void direction(bool dirMode) {            //Function to change direction - boolean value
  if (dirMode) {                          //When dirMode is true, 
    digitalWrite(DIR_PIN, HIGH);          //Direction Pin goes high (clockwise) 
    //Serial.print("Clockwise\n");          //Prints 'clockwise' in serial port 
  } else {                                //otherwise be false
    digitalWrite(DIR_PIN, LOW);           //Direction pin stays low
    //Serial.print("Anti - Clockwise\n");   //prints 'anti clockwise' in serial port
  }
}

void enable(bool engMode) {              //Function to change engage - boolean value
  if (engMode) {                         //When engMode is true,
    digitalWrite(ENA_PIN, HIGH);         //Enable Pin goes high (motor is then angaged) 
    //Serial.print("Enabled\n");           //Prints 'enable' in serial port 
  } else {                               //otherwise be false
    digitalWrite(ENA_PIN, LOW);          //enable pin stays low
    //Serial.print("Not Enabled\n");       // prints 'not enable' in serial port
  }
}

void turn(int degree){                   //Function to move steps - integer value
  int steps = degree / 1.8;              //to get steps, degrees geven divide by 1.8 degree incremnts
  Serial.print("Number of steps is ");   //The next 2 lines create a line that display
  Serial.println(steps);                 //amount of steps calculated
  for (int i = 0; i < steps; i++) {      //Loops for the amount of steps given
    digitalWrite(STEP_PIN, HIGH);        //Start of step pin pulse (HIGH)
    delayMicroseconds(1000);             //holds for 1s
    digitalWrite(STEP_PIN, LOW);         //End of step pin pulse (LOW)
    delayMicroseconds(1000);             //holds for 1s
    }
}


//HOMING SEQUENCE

void homingSequence(AccelStepper &X, int X_Lim_Pin, AccelStepper &Y, int Y_Lim_Pin) {
  bool X_AT_HOME == false; //Creating variables to see if they are at home yet.
  bool Y_AT_HOME == false;

  X.setMaxSpeed(250); 
  Y.setMaxSpeed(250);
  X.setAcceleration(200); 
  Y.setAcceleration(200);

  X.moveTo(-100000); // toward X switch
  Y.moveTo(-100000); // toward Y switch

  while (!X_AT_HOME || !Y_AT_HOME) {
    if (!X_AT_HOME){
      if (digitalRead(X_Lim_Pin) == LOW) {
        X.stop();
        X.setCurrentPosition(0);
        xHomed = true;
      } else {
        X.run();
      }
    }

    if (!Y_AT_HOME){
      if (digitalRead(Y_Lim_Pin) == LOW) {
        Y.stop();
        Y.setCurrentPosition(0);
        Y_AT_HOME = true;
      } else {
        Y.run();
      }
    }
  }
}

//INITIALISATION

void initialise() {
  pinMode(X_STEP_PIN, OUTPUT);  //Set pins to output
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENA_PIN, OUTPUT);
  pinMode(X_LED_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);  //Set pins to output
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENA_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);

  pinMode(X_LIMIT_PIN, INPUT_PULLUP); //IMPORTANT TO NOTE THAT LOW MEANS CLOSED CIRCUIT, HIGH MEANS OPEN CIRCUIT.
  pinMode(Y_LIMIT_PIN, INPUT_PULLUP);

  Serial.begin(115200);         //Begin serial commuication
  Serial.println("Welcome");
  Serial.print("")
  delay(100);
}

//HOW MANY STEPS THERE IS IN A ROW

void howManySteps(int axis) {
  int degree = 360;
  int stepsDone = 0;
  if axis == 1 {
    stepperX.moveTo(100000); // toward X switch
    while (X_Limit_Switch == HIGH){
      int steps = degree / 1.8;
      for (int i = 0; i < steps; i++) {
        digitalWrite(X_STEP_PIN, HIGH);        //Start of step pin pulse (HIGH)
        delayMicroseconds(1000);             //holds for 1s
        digitalWrite(X_STEP_PIN, LOW);         //End of step pin pulse (LOW)
        delayMicroseconds(1000);             //holds for 1s
      }
      int stepsDone += 1;
    }
  } else {
    stepperY.moveTo(100000); // toward X switch
    while (X_Limit_Switch == HIGH){
      int steps = degree / 1.8;
      for (int i = 0; i < steps; i++) {
        digitalWrite(X_STEP_PIN, HIGH);        //Start of step pin pulse (HIGH)
        delayMicroseconds(1000);             //holds for 1s
        digitalWrite(X_STEP_PIN, LOW);         //End of step pin pulse (LOW)
        delayMicroseconds(1000);             //holds for 1s
      }
      int stepsDone += 1;
    }
  }
  Serial.println(String("The steps done are ") + stepsDone);
}
