#include "main.h"

#define ONE_BALL 1

extern pros::Motor conveyorL;
extern pros::Motor conveyorR;
extern pros::ADILineSensor reached;

class Conveyor
{
    public:
        Conveyor();
        ~Conveyor();

        void zero();

        void reset();

        //Motor States
        void tarePos();
        void setBrakeType(pros::motor_brake_mode_e_t type_);

        //Task States
        int getMode();

        //Sensors

        static void start(void *ignore);
        void run();
        void stop();

        void move(int speed);

        void switchMode(double tarmode);
        void position(int ball);

        double get_line();
        void switchGate();

    private:
        static bool isRunning;
        static int mode;
        static bool is_reached;     
        static int count;
        static bool gate;
};