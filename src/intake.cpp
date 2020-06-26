#include "Intake.h"
#include "misc.h"

pros::Motor intakeL(1, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS);
pros::Motor intakeR(2, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS);
pros::ADILineSensor in(1);
bool Intake::isRunning = false;

int Intake::mode = 0;

bool Intake::is_in = false;

Intake::Intake() {}

Intake::~Intake() {}

void Intake::zero()
{
    in.calibrate();
}

double Intake::get_line(){
    return in.get_value_calibrated();
}

void Intake::reset()
{
    move(0);
    mode = 3;
}

void Intake::tarePos()
{
    intakeL.tare_position();
    intakeR.tare_position();
}

void Intake::setBrakeType(pros::motor_brake_mode_e_t type_)
{
    intakeL.set_brake_mode(type_);
    intakeR.set_brake_mode(type_);
}

int Intake::getMode()
{
    return mode;
}

void Intake::start(void *ignore)
{
    if (!isRunning)
    {
        pros::delay(500);
        Intake *that = static_cast<Intake *>(ignore);
        that->run();
    }
}

void Intake::run()
{
    isRunning = true;
    while (isRunning)
    {
        switch (mode)
        {
            // Intake
            case 1:
            {
                move(127);
                break;
            }

            // Outtake
            case 2:
            {
                move(-127);
                break;
            }

            // Stop
            case 3: 
            {
                move(0);
                break;
            }

            // Autointake - auton
            case 4:
            {
                while(true){
                    if(in.get_value_calibrated()>100){
                        move(127);
                    } else{
                        move(0);
                    }
                }
                break;
            }

            default:
            {
                if (pros::competition::is_autonomous()){
                    intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
                    intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
                    in.calibrate();
                    break;
                }
            }
        }
        pros::delay(10);
    }
}

void Intake::switchMode(double tarmode){
    mode = tarmode;
}

void Intake::stop()
{
    isRunning = false;
    reset();
}

void Intake::move(int speed)
{
    intakeL.move(speed);
    intakeR.move(speed);
}