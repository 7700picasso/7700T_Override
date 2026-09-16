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
motor leftMotor = motor(PORT1, ratio18_1, true);
motor rightMotor = motor(PORT10, ratio18_1, false);
motor bleftMotor = motor(PORT2, ratio18_1, true);
motor brightMotor = motor(PORT9, ratio18_1, false);
motor Liftone = motor(PORT19, ratio6_1, false); //right
motor Lifttwo = motor(PORT11, ratio6_1, false); //left
controller Potato_Controller = controller(primary);
digital_out claw =  digital_out(Brain.ThreeWirePort.A);



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Fu1aznctions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/




void liftUp(float speed){
  Liftone.spin(fwd, speed*-1, percent);
  Lifttwo.spin(fwd, speed, percent);
  wait(100, msec);
}

void liftHold(){
  Liftone.stop(hold);
  Lifttwo.stop(hold);
}

void Drive(float lspeed, float rspeed, int wt){
  leftMotor.spin(forward, lspeed, percent);
  bleftMotor.spin(forward, lspeed, percent);
  rightMotor.spin(forward, rspeed, percent);
  brightMotor.spin(forward, rspeed, percent);
  wait(wt, msec);
}

void Brake(){
  leftMotor.stop(brake);
  bleftMotor.stop(brake);
  rightMotor.stop(brake);
  brightMotor.stop(brake);
}
void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
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


//Displays information on the brain
void Display()
{
	double leftFrontCurr = leftMotor.current(amp);
	double leftFrontTemp = leftMotor.temperature(celsius);
	double leftBackCurr = bleftMotor.current(amp);
	double leftBackTemp = bleftMotor.temperature(celsius);
	double rightFrontCurr = rightMotor.current(amp);
	double rightFrontTemp = rightMotor.temperature(celsius);
	double rightBackCurr = brightMotor.current(amp);
	double rightBackTemp = brightMotor.temperature(celsius);
	double LiftoneCurr = Liftone.current(amp);
	double LiftoneTemp = Liftone.temperature(celsius);
	double LifttwoCurr = Lifttwo.current(amp);
	double LifttwoTemp = Lifttwo.temperature(celsius);


	if (leftMotor.installed()){
		MotorDisplay(1, leftFrontCurr, leftFrontTemp);
		Brain.Screen.printAt(300, YOFFSET + 1, "LeftFront");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 1, "LeftFront Problem");
	}
	
	
	if (bleftMotor.installed()){
		MotorDisplay(31, leftBackCurr, leftBackTemp);
		Brain.Screen.printAt(300, YOFFSET + 31, "LeftBack");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 31, "LeftBack Problem");
	}


	if (rightMotor.installed()) {
		MotorDisplay(61, rightFrontCurr, rightFrontTemp);
		Brain.Screen.printAt(300, YOFFSET + 61, "RightFront");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 61, "RightFront Problem");
	}
	
	
	if (brightMotor.installed()) {
		MotorDisplay(91, rightBackCurr, rightBackTemp);
		Brain.Screen.printAt(300, YOFFSET + 91, "RightBack");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 91, "RightBack Problem");
	}

	if (Liftone.installed()) {
		MotorDisplay(121, LiftoneCurr, LiftoneTemp);
		Brain.Screen.printAt(300, YOFFSET + 121, "RightLift");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 121, "RightLift Problem");
	}

	if (Lifttwo.installed()) {
		MotorDisplay(151, LifttwoCurr, LifttwoTemp);
		Brain.Screen.printAt(300, YOFFSET + 151, "LeftLift");
	} else {
		Brain.Screen.printAt(5, YOFFSET + 151, "LeftLift");
	}

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
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
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
  while (1) {
    float prct = 1;
	float leftSpeed=Potato_Controller.Axis3.position(pct)+Potato_Controller.Axis1.position(pct);
	float rightSpeed=Potato_Controller.Axis3.position(pct)-Potato_Controller.Axis1.position(pct);
	//This is Tank Drive
    // float leftSpeed = Potato_Controller.Axis3.position();
    // float rightSpeed = Potato_Controller.Axis2.position();
    if(Potato_Controller.ButtonL2.pressing()){
	  prct = 0.75;
		liftUp(150);
      
    } else if(Potato_Controller.ButtonL1.pressing()){
      prct = 1;
	  liftUp(-100);
    }
    Drive(leftSpeed*prct, rightSpeed*prct, 10);
    Display();
    // if (Potato_Controller.ButtonX.pressing()) {
    //       Brake();
    // }
	// if (Controller1.ButtonL1.pressing()) {
	// 		Pneu1.set(true);

	// } 
	// else if (Controller1.ButtonL2.pressing()) {
	// 		Pneu1.set(false);
	// } c

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
