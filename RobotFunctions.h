#ifndef RobotFunctions_h
#define RobotFunctions_h

#include <Zumo32U4.h> // Include the library for the Zumo 32U4 robot functions
#include <Timer.h> // Include the library for the timer

// Define states for robot behaviour
enum RobotState {
  MOVE,
  DELIVER_PARCEL,
  STOP
};

void RobotSetup();
void RobotLoop();

#endif