/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Student                                                   */
/*    Created:      8/13/2026, 4:51:39 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
// define your global instances of motors and other devices here
motor frontLeft = motor(PORT8, ratio6_1, true);
motor frontRight = motor(PORT1, ratio6_1, false);
motor backLeft = motor(PORT9, ratio6_1, true);
motor backRight = motor(PORT21, ratio6_1, false);
motor Liftone = motor(PORT20, ratio18_1, false); //right
motor Lifttwo = motor(PORT17, ratio18_1, true); //left
controller Potato_Controller = controller(primary);
digital_out claw =  digital_out(Brain.ThreeWirePort.H);
inertial gyrosensor = inertial(PORT6);

double pi = 3.14;
double wheel_diameter = 3.25;
double gear_ratio = 0.6;
int AutonSelected = 0; 


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Fu1aznctions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/




void liftUp(float speed, int wt){
  Liftone.spin(fwd, speed, percent);
  Lifttwo.spin(fwd, speed, percent);
  wait(wt, msec);
  Liftone.stop(brake);
  Lifttwo.stop(brake);
}


  


void liftHold(){
  Liftone.stop(hold);
  Lifttwo.stop(hold);
}

void Drive(float lspeed, float rspeed, int wt){
  frontLeft.spin(forward, lspeed, percent);
  backLeft.spin(forward, lspeed, percent);
  frontRight.spin(forward, rspeed, percent);
  backRight.spin(forward, rspeed, percent);
  wait(wt, msec);
}

void Brake(){
  frontLeft.stop(brake);
  backLeft.stop(brake);
  frontRight.stop(brake);
  backRight.stop(brake);
}



double YOFFSET = 20; //offset for the display
//Writes a line for the diagnostics of a motor on the Brain
void MotorDisplay(double y, double curr, double temp)
{
	Brain.Screen.setFillColor(transparent);
	Brain.Screen.printAt(5, YOFFSET + y, "Current: %.1fA", curr);
	
	if (curr < 1){
		Brain.Screen.setFillColor(green);
	} else if(curr >= 1 && curr  <= 2.5) {
		Brain.Screen.setFillColor(yellow);
	} else {
		Brain.Screen.setFillColor(red);
		Brain.Screen.drawRectangle(140, YOFFSET + y - 15, 15, 15);
	}

	
	Brain.Screen.setFillColor(transparent);
	Brain.Screen.printAt(160, YOFFSET + y, "Temp: %.1fC", temp);
	
	if (temp < 45){
		Brain.Screen.setFillColor(green);
	} else if(temp <= 50 && temp  >= 45){
		// TRUE and TRUE --> True
		// TRUE and FALSE --> False
		// FALSE and FALSE --> False
		Brain.Screen.setFillColor(yellow);
	} else {
		Brain.Screen.setFillColor(red);
		Brain.Screen.drawRectangle(275, YOFFSET + y - 15, 15, 15);
		Brain.Screen.setFillColor(transparent);
	}
}


//Displays information on the brain (Backup)
// void Display() 
// {
// 	double leftFrontCurr = leftMotor.current(amp);
// 	double leftFrontTemp = leftMotor.temperature(celsius);
// 	double leftBackCurr = bleftMotor.current(amp);
// 	double leftBackTemp = bleftMotor.temperature(celsius);
// 	double rightFrontCurr = rightMotor.current(amp);
// 	double rightFrontTemp = rightMotor.temperature(celsius);
// 	double rightBackCurr = brightMotor.current(amp);
// 	double rightBackTemp = brightMotor.temperature(celsius);
// 	double LiftoneCurr = Liftone.current(amp);
// 	double LiftoneTemp = Liftone.temperature(celsius);
// 	double LifttwoCurr = Lifttwo.current(amp);
// 	double LifttwoTemp = Lifttwo.temperature(celsius);


	// if (frontLeft.installed()){
	// 	MotorDisplay(1, leftFrontCurr, leftFrontTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 1, "LeftFront");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 1, "LeftFront Problem");
	// }
	
	
	// if (backLeft.installed()){
	// 	MotorDisplay(31, leftBackCurr, leftBackTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 31, "LeftBack");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 31, "LeftBack Problem");
	// }


	// if (frontRight.installed()) {
	// 	MotorDisplay(61, rightFrontCurr, rightFrontTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 61, "RightFront");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 61, "RightFront Problem");
	// }
	
	
	// if (backRight.installed()) {
	// 	MotorDisplay(91, rightBackCurr, rightBackTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 91, "RightBack");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 91, "RightBack Problem");
	// }

	// if (Liftone.installed()) {
	// 	MotorDisplay(121, LiftoneCurr, LiftoneTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 121, "RightLift");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 121, "RightLift Problem");
	// }

	// if (Lifttwo.installed()) {
	// 	MotorDisplay(151, LifttwoCurr, LifttwoTemp);
	// 	Brain.Screen.printAt(300, YOFFSET + 151, "LeftLift");
	// } else {
	// 	Brain.Screen.printAt(5, YOFFSET + 151, "LeftLift");
	// }



void gyrosensorprint(){
	float rotation = gyrosensor.rotation(deg);
	Brain.Screen.printAt(1,60, "rotation = %.2f. degrees", rotation);
}

void gyrosensorturn(float target){
	float position = 0.0;
	float error = target-position;
	float kp = 0.5;
	float speed = kp * error;

	float accuracy = 2.0;
	gyrosensor.setRotation(0.0, degrees);
while(fabs(error)>=accuracy)
{
speed=kp*error;
Drive(speed, -speed, 10); //turn right always
position=gyrosensor.rotation();  //measure the heading of the robot
error = target-position;  //calculate error
}
Drive(0, 0, 0);  //stop the drive
gyrosensorprint();
}


void pinchdrive(float target){
float position=0;
float error=target-position;
float kp=2.5;
float speed=kp*error;
float accuracy=0.4;
frontLeft.setPosition(0.0,rev);

while(fabs(error)>accuracy){
	Drive(speed, speed, 10);
	position=frontLeft.position(rev) * pi * gear_ratio * wheel_diameter;
	error=target-position;
	speed=kp*error;	
	}
	Brake();
}

void drawAutonButtons() {
  const char* autonNames[] = {"RED 1", "RED 2", "BLUE 1", "BLUE 2", "SKILLS"};
  Brain.Screen.setFont(fontType::mono20);

  for (int i = 0; i < 5; i++) {
    int y = 5 + (i * 46); // Stack buttons vertically (42px tall + 4px gap)

    // Highlight the selected button green; keep others dark gray
    if (AutonSelected == (i + 1)) {
      Brain.Screen.setPenColor(color::white);
      Brain.Screen.setFillColor(color::green);
    } else {
      Brain.Screen.setPenColor(color::white);
      Brain.Screen.setFillColor(color(60, 60, 60)); // Dark gray
    }

    // Draw button rectangle: x, y, width, height
    Brain.Screen.drawRectangle(5, y, 225, 42);

    // Print button text inside
    Brain.Screen.setPenColor(color::black);
    Brain.Screen.printAt(20, y + 28, autonNames[i]);
  }
}

void checkAutonTouch(){
	if (Brain.Screen.pressing()) {
		int touchX = Brain.Screen.xPosition();
		int touchY = Brain.Screen.yPosition();
		if (touchX >= 5 && touchX <= 230) {
			for (int i = 0; i < 5; i++) {
				int buttonY = 5 + (i * 46);
			
			if (touchY >= buttonY && touchY <= (buttonY +42)) {
				AutonSelected  = i+1;
				drawAutonButtons(); //check
				wait(250, msec);

			}
			}
		}
	}
	//
}

color getTempColor(double temperatureC) {
  if (temperatureC <= 30.0) {
    return color::green;
  } else if (temperatureC < 40.0) {
    return color::yellow;
  } else {
    return color::red;
  }
}

void drawMotorBox(const char* name, motor &m, int x, int y, int width, int height) {
  Brain.Screen.setPenColor(color::black);

  if (!m.installed()) {
    // Motor disconnected: White box with clear warning
    Brain.Screen.setFillColor(color::white);
    Brain.Screen.drawRectangle(x, y, width, height);

    Brain.Screen.setFont(fontType::mono15);
    Brain.Screen.printAt(x + 6, y + 20, name);
    Brain.Screen.setFont(fontType::mono12);
    Brain.Screen.printAt(x + 6, y + 40, "UNPLUGGED");
  } else {
    // Read live data from the smart motor
    double tempC = m.temperature(temperatureUnits::celsius);
    double amps  = m.current(currentUnits::amp);

    // Color code the background by temperature
    Brain.Screen.setFillColor(getTempColor(tempC));
    Brain.Screen.drawRectangle(x, y, width, height);

    // Line 1: Motor Name
    Brain.Screen.setFont(fontType::mono15);
    Brain.Screen.printAt(x + 6, y + 20, name);

    // Line 2: Temperature & Current (e.g., "31C 1.8A")
    // Using mono12 ensures both fit neatly across the 115px box width
    Brain.Screen.setFont(fontType::mono12);
    Brain.Screen.printAt(x + 6, y + 42, "%.0fC | %.1fA", tempC, amps);
  }
}

void updateMotorMonitor() {
  int col1X = 240;
  int col2X = 360;
  int boxW  = 115;
  int boxH  = 52;

  // Row 1 (y = 5)
  drawMotorBox("frontLeft", frontLeft,  col1X, 5,   boxW, boxH);
  drawMotorBox("frontRight", frontRight, col2X, 5,   boxW, boxH);

  // Row 2 (y = 63)
  drawMotorBox("backLeft", backLeft,  col1X, 63,  boxW, boxH);
  drawMotorBox("backRight", backRight, col2X, 63,  boxW, boxH);

  // Row 3 (y = 121)
  drawMotorBox("Liftone",  Liftone,  col1X, 121, boxW, boxH);
  drawMotorBox("Lifttwo", Lifttwo, col2X, 121, boxW, boxH);

}

//__________________________________EOF___________________________________________
void pre_auton(void) {
	claw.set(true);

   drawAutonButtons();
  updateMotorMonitor(); //check

  // Keep checking for touches until the match starts
  while (!Competition.isEnabled()) {
    checkAutonTouch();
    updateMotorMonitor(); // Keeps motor temps live while sitting on the field
    wait(50, msec);
  }


  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
	switch (AutonSelected) {

	case 1:
	//RED Adjcant side  
		claw.set(true); 
		liftUp(70, 400);
		liftHold();
		pinchdrive(10.2);
		liftUp(-40, 600);
		claw.set(false);
		// liftUp(50, 500);
		pinchdrive(-9.5);	
		 gyrosensorturn(50);
		pinchdrive(32);
		 gyrosensorturn(90);
		//liftUp(-70, 500);
		//Drive(-75, -75, 900);
		Drive(-30, -30, 200);	
		Drive(60, 60, 600);
		Drive(-30, -30, 200);
		wait(250, msec); 
		Drive(50, 50, 600);
		Drive(-80, -80, 300);

		// pinchdrive(35);
		 Brake();
		break;

	case 2:
	//RED Drive Station 15 second auton
		claw.set(true); 
		liftUp(40, 300);
		liftHold();
		Drive(20, 20, 800);
		wait(300, msec);
		liftUp(-40, 600);
		claw.set(false);
		pinchdrive(-8);
		gyrosensorturn(35);	
		Drive(-50, -50, 600);
		gyrosensorturn(90);
		Drive(100, 100, 600);
		Drive(-40, -40, 200);
		wait(250, msec); 
		Drive(100, 100, 600);
		Drive(-10, -10, 300);

		// pinchdrive(35);
		 Brake();
		break;

	case 3:
	//auton left side 15 second auton

		break;


	}
		
		

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void usercontrol(void) {
  // User control code here, inside the loop
  float prct = 1;
  while (1) {
    
	// Select the speed mode FIRST
if (Potato_Controller.ButtonA.pressing()) {
    prct = 0.25;
}
else if (Potato_Controller.ButtonB.pressing()) {
    prct = 1.0;
}

// Read joystick positions
float leftSpeed = Potato_Controller.Axis3.position();
float rightSpeed = Potato_Controller.Axis2.position();

// Apply the speed multiplier
float leftCommand = leftSpeed * prct;
float rightCommand = rightSpeed * prct;

// Drive the robot
Drive(leftCommand, rightCommand, 10);

// Display speed commands on the V5 Controller
Potato_Controller.Screen.clearLine(1);
Potato_Controller.Screen.setCursor(1, 1);
Potato_Controller.Screen.print("Mode: %s",
    prct == 0.25 ? "SLOW" : "NORMAL");

// Potato_Controller.Screen.clearLine(2);
// Potato_Controller.Screen.setCursor(2, 1);
// Potato_Controller.Screen.print("Left: %.0f%%", leftCommand);

// Potato_Controller.Screen.clearLine(3);
// Potato_Controller.Screen.setCursor(3, 1);
// Potato_Controller.Screen.print("Right: %.0f%%", rightCommand);
	
	
	
	//float prct = 0.7;
	
	//This is Tank Drive
    // float leftSpeed = Potato_Controller.Axis3.position();
    // float rightSpeed = Potato_Controller.Axis2.position();

	// float leftPower = LeftSpeed 

	//    Drive(leftSpeed*prct, rightSpeed*prct, 10);

	//      if(Potato_Controller.ButtonA.pressing()){
	//   	prct = 0.25;
	// 	}
	// 	else if (Potato_Controller.ButtonB.pressing()){
	// 		prct = 1;
	// 	}

	if (Potato_Controller.ButtonL1.pressing()) {
		 Liftone.spin(fwd, 100, percent);
  		Lifttwo.spin(fwd, 100, percent);

	}
	else if (Potato_Controller.ButtonL2.pressing()) { 
		Liftone.spin(reverse, 100, percent);
  		Lifttwo.spin(reverse, 100, percent);

	}
	else {
		Liftone.stop();
		Lifttwo.stop();

	}
 
    // Display();
    // if (Potato_Controller.ButtonX.pressing()) {
    //       Brake();
    // }
	if (Potato_Controller.ButtonR1.pressing()) {
			claw.set(true);
			// prct = 0.5;

	} 
	else if (Potato_Controller.ButtonR2.pressing()) {
			claw.set(false);
			// prct = 0.75;
	}


	// Display();
	//int speed =80; 
    
	// float leftSpeed=Potato_Controller.Axis3.position(pct)+Potato_Controller.Axis1.position(pct);
	// float rightSpeed=Potato_Controller.Axis3.position(pct)-Potato_Controller.Axis1.position(pct);
	//This is Tank Drive
    // float leftSpeed = Potato_Controller.Axis3.position();
    // float rightSpeed = Potato_Controller.Axis2.position();
  
      
    // } else if(Potato_Controller.ButtonB.pressing()){
    //   prct = 1;
	 
    // }
    // Drive(leftSpeed*prct, rightSpeed*prct, 10);
    
    // if (Potato_Controller.ButtonX.pressing()) {
    //       Brake();
    // }
	// if (Potato_Controller.ButtonL1.pressing()) {
	// 		claw.set(true);

	// } 
	// else if (Potato_Controller.ButtonL2.pressing()) {
	// 		claw.set(false);
	// } 

	// if (Potato_Controller.ButtonR1.pressing()){ 
	// 	Liftone.spin(fwd, speed*-1, percent);
  	// 	Lifttwo.spin(fwd, speed, percent);
	// }
	// else if(Potato_Controller.ButtonR2.pressing()) {
	// 	Liftone.spin(reverse, speed*-1, percent);
	// 	Lifttwo.spin(reverse, speed, percent);
	// }
	// else {
	// 	Liftone.stop();
	// 	Lifttwo.stop();
	// }

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }

}
