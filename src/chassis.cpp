#include "chassis.h"
#include <math.h>

pros::Motor LF(12, MOTOR_GEARSET_06, true),
    LB(11, MOTOR_GEARSET_06, true),
    RF(9, MOTOR_GEARSET_06),
    RB(10, MOTOR_GEARSET_06);

pros::ADIEncoder Left(10,11), Right(12,13), Middle(14,15);
pros::Imu Imu_L(5), Imu_R(6);

bool Chassis::isRunning = false,
     Chassis::isSettled = true;
int Chassis::mode = IDLE;

double Chassis::kP_drive = 0.102, Chassis::kP_turn = 0.19;

double Chassis::tolerance = 1;

double Chassis::errorLeft = 0, Chassis::errorRight = 0,
       Chassis::outputLeft = 0, Chassis::outputRight = 0;

int Chassis::signLeft = 0, Chassis::signRight = 0,
    Chassis::targetLeft = 0, Chassis::targetRight = 0,
    Chassis::velCap = 0,
    Chassis::speedLimit = 0,
    Chassis::accelLimit = 0;

double Chassis::x = 0, Chassis::y = 0, Chassis::theta = 0, Chassis::base = 0; //wheel to wheel
double Chassis::lastRight = 0, Chassis::lastLeft = 0, Chassis::lastMid = 0, Chassis::h = 0, Chassis::h2 = 0, Chassis::back = 0; //back to center
double Chassis::errorx = 0; double Chassis::errory = 0; double Chassis::errora = 0;
double Chassis::apower = 0, Chassis::bpower = 0;
double Chassis::tarA = 0, Chassis::tarB = 0, Chassis::rad = 0, Chassis::desireda = 0;
double Chassis::diameter = 3.25;

double Chassis::calculatedAngle = 0;


Chassis::Chassis() {}

Chassis::~Chassis()
{
    reset();
}

Chassis &Chassis::withGain(double kP)
{
    kP_drive = kP;
    return *this;
}

Chassis &Chassis::withTurnGain(double kP)
{
    kP_turn = kP;
    return *this;
}

Chassis &Chassis::withTol(double tolerance_)
{
    tolerance = tolerance_;
    return *this;
}

Chassis &Chassis::drive(int distance_, int speed_, int accel_)
{
    reset();
    targetLeft += distance_;
    targetRight += distance_;
    speedLimit = speed_;
    accelLimit = accel_;
    isSettled = false;
    mode = DRIVING;
    return *this;
}

Chassis &Chassis::turn(int distance_, int speed_, int accel_)
{
    reset();
    targetLeft += distance_;
    targetRight -= distance_;
    speedLimit = speed_;
    accelLimit = accel_;
    isSettled = false;
    mode = TURNING;
    return *this;
}

void Chassis::waitUntilSettled()
{
    while (!isSettled)
        pros::delay(20);
}

double Chassis::getLeft()
{
    return Left.get_value()/360*PI*diameter; //inches
}

double Chassis::getRight()
{
    return Right.get_value()/360*PI*diameter;
}

double Chassis::getMid(){
    return Middle.get_value()/360*PI*diameter;
}

void Chassis::tarePos()
{
    LF.tare_position();
    LB.tare_position();
    RF.tare_position();
    RB.tare_position();
}

void Chassis::reset()
{
    targetLeft = targetRight = 0;
    errorLeft = errorRight = 0;
    outputLeft = outputRight = 0;
    signLeft = signRight = 0;

    mode = IDLE;

    LF.move_velocity(0);
    LB.move_velocity(0);
    RF.move_velocity(0);
    RB.move_velocity(0);

    LF.tare_position();
    LB.tare_position();
    RF.tare_position();
    RB.tare_position();
}

void Chassis::lock()
{
    LF.set_brake_mode(MOTOR_BRAKE_HOLD);
    LB.set_brake_mode(MOTOR_BRAKE_HOLD);
    RF.set_brake_mode(MOTOR_BRAKE_HOLD);
    RB.set_brake_mode(MOTOR_BRAKE_HOLD);
}

void Chassis::unlock()
{
    LF.set_brake_mode(MOTOR_BRAKE_COAST);
    LB.set_brake_mode(MOTOR_BRAKE_COAST);
    RF.set_brake_mode(MOTOR_BRAKE_COAST);
    RB.set_brake_mode(MOTOR_BRAKE_COAST);
}

int Chassis::getMode()
{
    return mode;
}

void Chassis::setMode(int mode_)
{
    mode = mode_;
}

void Chassis::start(void *ignore)
{
    if (!isRunning)
    {
        pros::delay(500);
        Chassis *that = static_cast<Chassis *>(ignore);
        that->run();
    }
}

void Chassis::run()
{
    isRunning = true;
    Left.reset();
    Right.reset();
    Middle.reset();
    Imu_L.reset();
    Imu_R.reset();

    while (isRunning)
    {

        if (!pros::competition::is_autonomous())
            goto end;

        switch (mode)
        {
        case DRIVING:
        {
            errorLeft = targetLeft - LF.get_position();
            errorRight = targetRight - RF.get_position();

            signLeft = errorLeft / abs(errorLeft);
            signRight = errorRight / abs(errorRight);

            outputLeft = errorLeft * kP_drive;
            outputRight = errorRight * kP_drive;

            velCap += accelLimit;

            if (velCap > speedLimit)
                velCap = speedLimit;

            if (abs(outputLeft) > velCap)
                outputLeft = velCap;

            if (abs(outputRight) > velCap)
                outputRight = velCap;

            if (abs(errorLeft) < tolerance && abs(errorRight) < tolerance)
            {
                isSettled = true;
                withGain().withTurnGain().withTol().reset();
                break;
            }

            forward(outputLeft);
            break;
        }

        case TURNING:
        {
            errorLeft = targetLeft - LF.get_position();
            errorRight = targetRight - RF.get_position();

            signLeft = errorLeft / abs(errorLeft);
            signRight = errorRight / abs(errorRight);

            outputLeft = errorLeft * kP_turn;
            outputRight = errorRight * kP_turn;

            velCap += accelLimit;

            if (velCap > speedLimit)
                velCap = speedLimit;

            if (abs(outputLeft) > velCap)
                outputLeft = velCap * signLeft;

            if (abs(outputRight) > velCap)
                outputRight = velCap * signRight;

            if (abs(errorLeft) < tolerance && abs(errorRight) < tolerance)
            {
                isSettled = true;
                withGain().withTurnGain().withTol().reset();
                break;
            }

            forward(outputLeft);
            break;
        }

        default:
        {
            break;
        }
    }

        std::cout << "Left Error: " << errorLeft
                  << ", Right Error: " << errorRight
                  << ", Left Output: " << outputLeft
                  << ", Right Output: " << outputRight
                  << ", Left Target: " << targetLeft
                  << ", Right Target: " << targetRight
                  << ", Left Encoder: " << getLeft()
                  << ", Right Encoder: " << getRight()
                  << std::endl;

    end:
        pros::delay(10);
    }
}

void Chassis::stop()
{
    reset();
    isRunning = false;
}

void Chassis::forward(double speed)
{
    LF.move(speed);
    LB.move(speed);
    RF.move(speed);
    RB.move(speed);
}

void Chassis::left(double speed)
{
    LF.move(speed);
    LB.move(speed);
    RF.move(-speed);
    RB.move(-speed);
}

void Chassis::track(){
    calculatedAngle = (Imu_L.get_heading()-Imu_R.get_heading())/2; //replaces ang

    while(1){
        double right = getRight();
        double left = getLeft();
        double mid = getMid();
        double ang = ((left - lastLeft) - (right - lastRight))/base;
        h = left - lastLeft;
        h2 = mid - lastMid;
        if(ang!=0) { //calculations when the robot moves in an arc
            double radS = h/ang;
            h = (2*radS + base)*sin(ang/2); //side 
            double radB = h2/ang;
            h2 = (radB + back)*2*sin(ang/2); //back
        } 
        //x and y calculated by getting x and y component of arc created from turning and strafing
        x = x + h2*cos(theta+ang/2) + h*sin(theta+ang/2);
        y = y - h2*sin(theta+ang/2) + h*cos(theta+ang/2);
        theta += ang;   

        lastRight = right;
        lastMid = mid;
        lastLeft = left;
        pros::delay(10);
    }
}

//might be void
Chassis& Chassis::pid(double targetx, double targety, double targeta){
    isSettled = false;
    errorx = targetx - x;
    errory = targety - y;
    errora = targeta - theta;

    desireda = atan2(targety-y,targetx-x); //calculate angle of closest approach
    rad = errorx*cos(desireda); //radius = distance
    while(!isSettled){
        if(errorx == 0&&errory==0){ //turning
            if(errora == 0){
                isSettled = true;
            } else {
                apower = 0;
                bpower = kP_turn*errora;
            }
        } else{
            //finds closest cardinal direction - using 4 motors are better than 2
            if(desireda>=-PI&&desireda<(-3*PI/4)){
                tarA = -PI;
                tarB = desireda+PI;
            } else if(desireda>=(-3*PI/4)&&desireda<(-PI/4)){
                tarA = -PI/2;
                tarB = desireda+(PI/2);
            } else if(desireda>=(-PI/4)&&desireda<(PI/4)){
                tarA = 0;
                tarB = desireda;
            } else if(desireda>=(PI/4)&&desireda<(3*PI/4)){
                tarA = PI/2;
                tarB = desireda-(PI/2);
            } else{
                tarA = PI;
                tarB = desireda-PI;
            }   
            apower = kP_drive*rad; //linear component
            if (rad>(24.06/34.033)*(tarB-theta)){ //calculated from kinematics
            //if the distance left is less, it's optimal to start heading to desired angle or you'll waste time idling
                bpower = kP_turn*(tarA-theta);
            } else{
                bpower = kP_turn*(tarB-theta); //tarB is error in angle from turning to cardinal direction first
            }
        }

        //possible "async" calling to conveyor to outtake opponent balls
        //use mode 7 - does not overwrite conveyor motors
        
        //average power to prevent rapid change in voltage
        LB.move((apower+bpower)/2);
        LF.move((apower+bpower)/2);
        RB.move((apower-bpower)/2);
        RF.move((apower-bpower)/2);
        //recalculates x, y, theta
        track();
        //recalculates error
        errorx = targetx-x;
        errory = targety-y;
        errora = targeta-theta;
        rad = errorx*cos(desireda);
        pros::delay(10);
        if(errorx<10&&errory<10&&errora<10){
            isSettled = true;
            errorx = errory = errora = 0;
            LB.move(0);
            LF.move(0);
            RB.move(0);
            RF.move(0);
        }
    }
}

bool Chassis::getSettled(){
    return isSettled;
}

double Chassis::getX(){
    return x;
}

double Chassis::getY(){
    return y;
}

double Chassis::getTheta(){
    return theta;
}

// double Chassis::getDriveP(){
//     return drive_P;
// }

// double Chassis::getDriveI(){
//     return drive_I;
// }

// double Chassis::getDriveD(){
//     return drive_D;
// }

// double Chassis::getTurnP(){
//     return turn_P;
// }

// double Chassis::getTurnI(){
//     return turn_I;
// }

// double Chassis::getTurnD(){
//     return turn_D;
// }

double Chassis::getIMUL(){
    return Imu_L.get_heading();
}

double Chassis::getIMUR(){
    return Imu_R.get_heading();
}

void Chassis::resetIMU(){
    Imu_L.reset();
    Imu_R.reset();
}

void Chassis::resetEncoder(){
    Left.reset();
    Right.reset();
    Middle.reset();
}