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
        //lin motion
        float leftBack = master.get_analog(ANALOG_RIGHT_Y)-master.get_analog(ANALOG_RIGHT_X);
        float leftFront = master.get_analog(ANALOG_RIGHT_Y)+master.get_analog(ANALOG_RIGHT_X);
        float rightFront = master.get_analog(ANALOG_RIGHT_Y)-master.get_analog(ANALOG_RIGHT_X);
        float rightBack = master.get_analog(ANALOG_RIGHT_Y)+master.get_analog(ANALOG_RIGHT_X);
        float motorPwd[4] = {leftBack,leftFront,rightFront,rightBack};
        if(abs(master.get_analog(ANALOG_RIGHT_X)+abs(master.get_analog(ANALOG_RIGHT_Y)))>(127*2)){
            float max = abs(master.get_analog(ANALOG_RIGHT_X))+abs(master.get_analog(ANALOG_LEFT_X));
            for(int i = 0; i < 4; i++){
                motorPwd[i]*= 127/max; //proportion motor to each other and max voltage
            }
        }

        //add rotation
        float max = 0;
        leftBack+=master.get_analog(ANALOG_LEFT_Y);
        leftFront+=master.get_analog(ANALOG_LEFT_Y);
        rightFront-=master.get_analog(ANALOG_LEFT_Y);
        rightBack-=master.get_analog(ANALOG_LEFT_Y);
        for(int i = 0; i < 4; i++){ //find max power
            if(abs(motorPwd[i])>127){
                max = abs(motorPwd[i]);
            }
        } if(max>0){
            for(int i = 0; i<4; i++){
                motorPwd[i]*=127/max; //proportion motor to each other and max voltage
            }
        }

        //set drive motor
        LB.move(leftBack);
        LF.move(leftFront);
        RF.move(rightFront);
        RB.move(rightBack);

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