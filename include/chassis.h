#include "main.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::Imu Imu_L, Imu_R;
extern pros::ADIEncoder Left, Right, Middle;

#define DRIVING 1
#define TURNING 2

class Chassis
{
public:
    Chassis();
    ~Chassis();

    Chassis &withGain(double kP = 0.09);

    Chassis &withTurnGain(double kP = 0.2);

    Chassis &withTol(double tolerance_ = 1);

    Chassis &drive(int distance_, int speed_, int accel_ = 5);

    Chassis &turn(int distance_, int speed_, int accel_ = 5);

    void waitUntilSettled();

    double getLeft();
    double getRight();
    double getMid();

    void tarePos();

    void reset();

    void lock();

    void unlock();

    int getMode();
    void setMode(int mode);

    static void start(void *ignore);
    void run();
    void stop();

    void forward(double speed);
    void left(double speed);

    void track();
    Chassis& pid(double targetx, double targety, double targeta);
    double getX();
    double getY();
    double getTheta();
    double getDriveP();
    double getDriveI();
    double getDriveD();
    double getTurnP();
    double getTurnI();
    double getTurnD();
    double getIMUL();
    double getIMUR();
    void resetIMU();
    void resetEncoder();
    bool getSettled();

private:
    static bool isRunning;
    static bool isSettled;
    static int mode;

    static double kP_drive, kP_turn;
    static double tolerance;

    static double errorLeft, errorRight;
    static double outputLeft, outputRight;
    static int signLeft, signRight;

    static int velCap;
    static int speedLimit;
    static int accelLimit;
    static int targetLeft;
    static int targetRight;
    //FINISH
    static double base;
    static double back;
    static double x;
    static double y;
    static double theta;

    static double lastRight;
    static double lastMid;
    static double lastLeft;
    
    static double h;
    static double h2;

    static double errorx;
    static double errory;
    static double errora;

    static double apower;
    static double bpower;
    static double tarB;
    static double tarA;

    static double rad;
    static double desireda;

    static double diameter;
    static double calculatedAngle;
};