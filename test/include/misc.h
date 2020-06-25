#pragma once
#include "api.h"

namespace io
{
    extern pros::Controller master;

    extern pros::Motor rollerL, rollerR;

    // Rotates the rollers in a given speed. Use negative values for going down. Uses RPM for speed.
    void roller(int speed);

    // Rotates the rollers in a given speed to a given point. Use negative values for going down. Uses RPM for speed.
    void roller(double rot, int speed);
}