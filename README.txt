Zumo32U4 Robotics Project README

Prerequisites:

- Hardware: Pololu Zumo32U4 Robot
- Software: Arduino IDE

Setup Instructions

1. Arduino IDE Configuration:
- Download and install the Arduino IDE.
- Open the Arduino IDE, navigate to the Library Manager and install the 'Timer' and 'Zumo32U4' libraries.

2. Building and Uploading
- Download and unzip the project files and open the 'Project.ino' file in the Arduino IDE, 'Files > Open'.
- Connect the Zumo32U4 to your computer using a USB cable.
- Select the port that corresponds to your Zumo32U4 robot.
- Upload the program by pressing the 'Upload' button in the Arduino IDE to compile and upload the program.

3. Usage Instructions
- Starting the Robot: Place the robot at the start of the maze and press either the A or B button on your Zumo32U4, the LCD screen will display a prompt to perform this action. The A button will make the robot deliver parcels to one house, the B button will make the robot deliver to two.
- Navigating the maze: Once started, the Zumo32U4 will navigate the maze, avoiding the black walls by utilising the line sensors.
- Delivering parcels: When the robot detects a house, using the proximity sensors, it will stop and display a prompt to the LCD screen.
- Returning to start: Once it has delivered the parcels the Zumo32U4 will navigate the maze in order to return to the start.

Libraries used:

- Timer by Stefan Staub
- Zumo32U4 by Pololu