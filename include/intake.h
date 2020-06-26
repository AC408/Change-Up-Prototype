#include "main.h"

#define ONE_BALL 1

extern pros::Motor intakeL;
extern pros::Motor intakeR;
extern pros::ADILineSensor in;

class Intake
{
    public:
        Intake();
        ~Intake();

        void zero();

        void reset();

        //Motor States
        void tarePos();
        void setBrakeType(pros::motor_brake_mode_e_t type_);

        //Task States
        int getMode();

        static void start(void *ignore);
        void run();
        void stop();

        void move(int speed);
        void switchMode(double tarmode);
        double get_line();

    private:
        static bool isRunning;
        static int mode;
        static bool is_in;        
};