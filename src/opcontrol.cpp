#include "main.h"

#include "chassis.h"
#include "misc.h"

#include "displayController.h"
#include "conveyor.h"
#include "intake.h"

void opcontrol()
{
    using namespace io;
    Conveyor conveyor;
    Intake intake;

    LF.set_brake_mode(MOTOR_BRAKE_COAST);
    LB.set_brake_mode(MOTOR_BRAKE_COAST);
    RF.set_brake_mode(MOTOR_BRAKE_COAST);
    RB.set_brake_mode(MOTOR_BRAKE_COAST);

    conveyorL.set_brake_mode(MOTOR_BRAKE_HOLD);
    conveyorR.set_brake_mode(MOTOR_BRAKE_HOLD);
    intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
    intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);

    intake.switchGate();
    conveyor.switchGate();

    while (true)
    {
        LB.move(master.get_analog(ANALOG_RIGHT_Y)-master.get_analog(ANALOG_RIGHT_X)+master.get_analog(ANALOG_LEFT_Y));
        LF.move(master.get_analog(ANALOG_RIGHT_Y)+master.get_analog(ANALOG_RIGHT_X)+master.get_analog(ANALOG_LEFT_Y));
        RF.move(master.get_analog(ANALOG_RIGHT_Y)+master.get_analog(ANALOG_RIGHT_X)+master.get_analog(ANALOG_LEFT_Y));
        RB.move(master.get_analog(ANALOG_RIGHT_Y)-master.get_analog(ANALOG_RIGHT_X)+master.get_analog(ANALOG_LEFT_Y));

        //intake
        if(master.get_digital(DIGITAL_L1)||master.get_digital(DIGITAL_Y)){
            intake.switchMode(1); //forward
        } else if(master.get_digital(DIGITAL_L2)){
            intake.switchMode(2); //backward
        } else{
            intake.switchMode(3); //off
        }

        //conveyor
        if(master.get_digital(DIGITAL_A)){
            conveyor.switchMode(1); //1
        } else if(master.get_digital(DIGITAL_B)){
            conveyor.switchMode(2); //2
        } else if(master.get_digital(DIGITAL_X)){
            conveyor.switchMode(3); //3
        } else if(master.get_digital(DIGITAL_Y)){ //reset ball position -> change intake
            conveyor.switchMode(6); //-127
        } else {
            conveyor.switchMode(4); //off
        }

        pros::delay(10);
    }
}