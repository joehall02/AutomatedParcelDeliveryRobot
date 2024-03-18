#include "RobotFunctions.h"

Zumo32U4Motors motors; // Create an instance for controlling the robot's motors.
Zumo32U4LineSensors lineSensors; // Create an instance for the line sensors.
Zumo32U4ButtonA buttonA; // Create an instance for the A button
Zumo32U4ButtonB buttonB; // Create an instance for the B button
Zumo32U4ProximitySensors proxSensors; // Create an instance for the proximity sensors
Zumo32U4LCD display; // Create and instance for the display
Zumo32U4Buzzer buzzer; // Create an instance for the buzzer
Timer timer; // Create an instance for the timer

int lineSensorValues[5]; // Array to store readings from each of the 5 line sensors.

int turnTime = 300; // Define a delay for turning
int reverseTime = 300; // Define a delay for reversing
int turnSpeed = 200; // Turn speed
int motorSpeed = 100; // motor speed

int lineThreshold = 1000; // Define a threshold for white vs black detection

int sensorThreshold = 6; // Define a threshold for the proximity sensors, 6 being the least sensitive

int rightCount = 0;
int leftCount = 0;

bool twoHouses; // Define a bool to specify if the robot will be searching for one or two houses
bool houseDetected; // Define a bool to identify if a house has been detected
bool deliveriesComplete = false; // Define a bool to track if deliveries have been complete
int numberOfHousesDetected = 0; // Track how many houses the robot detects

// Initialise state
RobotState state = MOVE;

// Function to turn left
void TurnLeft(int delayTime) {
  motors.setSpeeds(-motorSpeed, -motorSpeed); // Reverse
  delay(reverseTime); // Delay for reversing
  motors.setSpeeds(-turnSpeed, turnSpeed); // Turn right
  delay(delayTime); // Delay for turning
}

// Function to turn right
void TurnRight(int delayTime) {
  motors.setSpeeds(-motorSpeed, -motorSpeed); // Reverse
  delay(reverseTime); // Delay for reversing
  motors.setSpeeds(turnSpeed, -turnSpeed); // Turn right
  delay(delayTime); // Delay for turning
}

// Function for turning randomly
void RandomTurn(int delayTime) {
  int random_num = random(2); // Creates random number between 0 and 1

  motors.setSpeeds(-motorSpeed, -motorSpeed); // Reverse
  delay(reverseTime); // Delay for reversing

  if (random_num == 0) {
    // Turn Left
    TurnLeft(delayTime);
    buzzer.play(">g32>>c32"); // Make sound to indicate random turn 

  } else {
    // Turn Right
    TurnRight(delayTime);
    buzzer.play(">g32>>c32"); // Make sound to indicate random turn 

  }
  timer.pause(); // Pauses the timer
  delay(500); // Delay
  timer.start(); // Starts the timer again
}

// Function for line detection
void LineDetection() {
  lineSensors.read(lineSensorValues); // Start reading values from the line sensors

  // Far left sensor
  // If sensor detects black, reverse and move right
  if (lineSensorValues[0] > lineThreshold) {
    rightCount++;

    if (rightCount > 3) {
      TurnLeft(150);
      rightCount = 0;
      leftCount = 0;
    } else {
      TurnRight(150);
      
    }
  }
  // Far right sensor
  // If sensor detects black, reverse and move left
  else if (lineSensorValues[4] > lineThreshold) {
    leftCount++;

    if (leftCount > 3) {
      TurnRight(200);
      leftCount = 0;
      rightCount = 0;
    } else {
      TurnLeft(200);
    }
  }
  else {
    // else, move forward
    motors.setSpeeds(motorSpeed, motorSpeed);
  }
}

void HouseDetection() {
  proxSensors.read(); // Starts taking readings from the front proximity sensor
  int leftValue = proxSensors.countsFrontWithLeftLeds(); // Returns the number of brightness levels for the left LEDs that activated the front proximity sensor
  int rightValue = proxSensors.countsFrontWithRightLeds(); // Returns the number of brightness levels for the right LEDs that activated the front proximity sensor

  houseDetected = leftValue >= sensorThreshold || rightValue >= sensorThreshold; // Bool to signal whether or not a house has been detected by the proximity sensors

  // If a house is detected, set robots state to DELIVER_PARCEL
  if(houseDetected) {
    state = DELIVER_PARCEL;
  }
}

// Move functionality
void Move() {

  // Prevent adding consecutive FORWARD movements
  // if(pathLength == 0 || path[pathLength - 1] != FORWARD){
  //   addMovement(FORWARD);
  // }

  LineDetection(); // Line Detection functionality

  // If 10 seconds has passed, perform random turn either left or right
  if(timer.read() > 20000) {
    RandomTurn(500); // Random Turn functionality 
  }
  
  if (deliveriesComplete == false) {
    HouseDetection(); // House Detection functionality
  }
}

// Deliver parcel functionality
void DeliverParcel() {

  display.clear(); // Clears the display
  display.gotoXY(0,0); // Sets cursor to top left
  display.print("Parcel"); // Prints to display
  display.gotoXY(0, 1); // Set cursor to next line
  display.print("delivered!");
  
  // Play sound to indicate parcel has been identified, 
  // waits for song to finish before proceeding 
  buzzer.play("L16 cdegreg4");
  
  motors.setSpeeds(0, 0);

  while(buzzer.isPlaying());

  display.clear();


  delay(1000); // One second delay


  // If twoHouses is true
  if(twoHouses) {
    numberOfHousesDetected++; // Incriment houses detected by 1

    
    // If number of houses detected is 2, change the state
    if(numberOfHousesDetected == 2) {
      //state = STOP;
      deliveriesComplete = true; // 2 Deliveries completed so sets deliveriesComplete to true
      state = MOVE;
    } else { 
      // if houses detected is not 2, reverse, perform random turn and keep moving until it finds another house
      motors.setSpeeds(-motorSpeed, -motorSpeed); // Reverse
      delay(reverseTime); // Delay for reversing
      houseDetected = false; // set house detected to false
      RandomTurn(500);
      state = MOVE;
    }
  } else {
    // if twoHouses is false, change the robots state
    deliveriesComplete = true; // One delivery complete so set deliveriesComplete to true

    //state = RETURN_TO_START;
    //state = STOP;
    state = MOVE;
  }

}


void Stop() {
  motors.setSpeeds(0, 0); // Set robot movement to 0

  display.clear(); // Clears the display
  display.gotoXY(0,0); // Sets cursor to top left
  display.print("Deliveries"); // Prints to display
  display.gotoXY(0, 1); // Set cursor to next line
  display.print("Complete!");

}

// Setup 
void RobotSetup(){
  lineSensors.initFiveSensors(); // Initialise the five line sensors on the robot.

  proxSensors.initFrontSensor(); // Initialise only the front proximity sensor

  timer.start(); // Start the timer 

  display.clear(); // Clears the display
  display.gotoXY(0,0); // Sets cursor to top left
  display.print("PressA/B"); // Prints to display
  display.gotoXY(0, 1); // Set cursor to next line
  display.print("to start");

  // Wait for A or B button press
  while (!buttonA.isPressed() && !buttonB.isPressed()){
    // Loop does nothing, just waits for button press
  }

  // Checks to see if A or B button is pressed
  if(buttonA.isPressed()) {
    twoHouses = false; // If A button pressed, only detect one house
    buttonA.waitForButton(); // Wait for A button to be pressed before doing anything
  } else if(buttonB.isPressed()) {
    twoHouses = true; // If B button pressed, detect two houses
    buttonB.waitForButton(); // Wait for B button to be pressed before doing anything
  }

  display.clear();

  // Plays music and waits for it to finish before proceeding
  buzzer.play(">g32>>c32");
  while(buzzer.isPlaying());
}

// Loop
void RobotLoop(){
  switch (state) {
    case MOVE:      
      Move();
      break;

    case DELIVER_PARCEL:
      DeliverParcel();
      break;  
    
    case STOP:   
      Stop();   
      break;    
  }
}