#include "main.h"

#include "chassis.h"
#include "misc.h"

static Chassis chassis;

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

}

void matchhomecenter(){
  
}

/*===========================================
  SKILLS AUTONOMOUSES
===========================================*/
void skills1()
{
}
