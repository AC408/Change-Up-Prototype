#include "main.h"

#include "chassis.h"

#include "misc.h"

#include "displayController.h"
#include "autonController.h"
#include "conveyor.h"
#include "intake.h"

void initialize()
{
    okapi::Timer timer;
    double init = timer.millis().convert(millisecond);

    Autonomous Auton;

    // Class Initialization
    Chassis chassis;
    Conveyor conveyor;
    Display Disp;
    Intake intake;

    // Threads
    pros::Task baseController(chassis.start, NULL, "Chassis Controller");

    pros::Task b_display(Disp.start, NULL, "Display Controller");
    b_display.set_priority(TASK_PRIORITY_MIN);

    pros::Task b_auton(Auton.start, NULL, "Auton Controller");
    pros::Task b_intake(intake.start, NULL, "Intake Controller");
    pros::Task b_conveyor(conveyor.start, NULL, "Conveyor Controller");


    std::cout << "Tasks Initialized!" << std::endl;

    Disp.cleanUp();

    double end = timer.millis().convert(millisecond);
    std::cout << "Initialization Done! Took " << end - init << "ms." << std::endl;
}

void disabled()
{
    Chassis chassis;
    Conveyor conveyor;
    Intake intake;

    chassis.withGain().withTurnGain().withTol().reset();
    chassis.setMode(IDLE);

    intake.reset();
    conveyor.reset();
}

void competition_initialize() {}
