#include "conveyor.h"
#include "misc.h"

pros::Motor conveyorL(3, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS);
pros::Motor conveyorR(4, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_ROTATIONS);
extern pros::ADILineSensor reached(2);
bool Conveyor::isRunning = false;
int Conveyor::mode = 0;
bool Conveyor::is_reached = false;
int Conveyor::count = 0;

Conveyor::Conveyor() {}

Conveyor::~Conveyor() {}

void Conveyor::zero()
{
    reached.calibrate();
}

void Conveyor::reset()
{
    move(0);
    mode = 4;
}

double Conveyor::get_line(){
    return reached.get_value_calibrated();
}

void Conveyor::tarePos()
{
    conveyorL.tare_position();
    conveyorR.tare_position();
}

void Conveyor::setBrakeType(pros::motor_brake_mode_e_t type_)
{
    conveyorL.set_brake_mode(type_);
    conveyorR.set_brake_mode(type_);
}

int Conveyor::getMode()
{
    return mode;
}

void Conveyor::start(void *ignore)
{
    if (!isRunning)
    {
        pros::delay(500);
        Conveyor *that = static_cast<Conveyor *>(ignore);
        that->run();
    }
}

void Conveyor::run()
{
    isRunning = true;
    while (isRunning)
    {
        switch (mode)
        {
            count = 0;
            if(reached.get_value_calibrated()==100){
                count = 1;
            }
            // Score 1 ball
            case 1:
            {
                position(1);
                break;
            }

            // Score 2 balls 
            case 2:
            {                
                position(2);
                break;
            }

            // Score 3 balls
            case 3:
            {
                position(3);
                break;
            }

            case 4:
            {
                move(0);
                break;
            }

            case 5:
            {
                move(127);
                break;
            }

            default:
            {
                if (pros::competition::is_autonomous())
                    conveyorL.set_brake_mode(MOTOR_BRAKE_HOLD);
                    conveyorR.set_brake_mode(MOTOR_BRAKE_HOLD);
                    reached.calibrate();
                break;
            }
        }
        pros::delay(10);
    }
}

void Conveyor::switchMode(double tarmode){
    mode = tarmode;
}

void Conveyor::stop()
{
    isRunning = false;
    reset();
}

void Conveyor::move(int speed)
{
    conveyorL.move(speed);
    conveyorR.move(speed);
}

void Conveyor::position(int balls){
    while(count<balls){
        if(reached.get_value_calibrated()>100){
            move(127);
        } else{
            while(reached.get_value_calibrated()<90){
                move(70);
                pros::delay(10);
            }
            count++;
        } pros::delay(10);
    } 
    while(reached.get_value_calibrated()>100){
        move(127);
        pros::delay(10);
    } move(0);
}