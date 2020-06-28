#include "main.h"

#include "chassis.h"
#include "conveyor.h"
#include "intake.h"
#include "misc.h"

static Chassis chassis;
static Conveyor conveyor;
static Intake intake;

using namespace io;

// Make Trigger

/*===========================================
  DICTIONARY
===========================================*/
// IDLE = 0
// DRIVING_POINT = 1
// DRIVING_DIST = 2
// TURNING = 3
// STRAFING = 4

void tester()
{
    // roller(127);
    // chassis.drive(2100,65,4).withAngle(0, 80).withTol(30).waitUntilSettled();
    // chassis.drive(-300,70,4).withAngle(0, 80).withTol(30).waitUntilSettled();
    // chassis.turn(45,70).withTol(10).waitUntilSettled();
    // chassis.drive(500,40,2).withAngle(45, 80).withTol(30).waitUntilSettled();

    io::master.rumble(" . .");
}

/*===========================================
  MATCH AUTONOMOUSES
===========================================*/

void matchhomerow()
{
  chassis.pid(0,6,0);
    conveyor.switchMode(1);
  chassis.pid(0,-78,3*PI/4); //might hit wall or hit ball
    conveyor.switchMode(5); //might be moved to one line before
    pros::delay(1000); //probably be removed -> how to async and block
    conveyor.switchMode(1);
  chassis.pid(0,-72,PI/2);
    conveyor.switchMode(5);
}

void matchhomecenter(){
  
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1()
{
  //dumps preload during deploy
  chassis.pid(-72, 36, -PI/2);
    while(!chassis.getSettled()){ //might be removed -> autoblocks with while loop
      pros::delay(20);
    }
  chassis.pid(-72,0,-3*PI/4);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(2);
  chassis.pid(0,96,0);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(0,72,-PI);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(2);
  chassis.pid(-72,72,-PI/2);
    conveyor.switchMode(5);
    pros::delay(1000);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(1);
  chassis.pid(-72,108,-PI/2);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(-72,144,-PI/4);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(1);
  chassis.pid(-48,120,PI/2);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(0,120,0);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(1);
  chassis.pid(72,108,PI/2);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(72,144,PI/4);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(2);
  chassis.pid(0,48,-3*PI/4);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(24,72,PI/2);
  chassis.pid(72,72,PI/2);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(3);
  chassis.pid(72,36,PI/2);
    conveyor.switchMode(5);
    pros::delay(1000);
  chassis.pid(36,24,-PI);
  chassis.pid(72,0,3*PI/4);
    while(!chassis.getSettled()){
      pros::delay(20);
    }
    conveyor.switchMode(2);
  chassis.pid(48,24,PI);
    conveyor.switchMode(5);
}
