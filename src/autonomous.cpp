#include "main.h"

#include "chassis.h"

#include "misc.h"

#include "autonController.h"
#include "displayController.h"

using namespace okapi;
using namespace std;

bool Autonomous::isInitialized = false;
int Autonomous::type = SLOT_MATCH,
    Autonomous::slot = 1;

std::map<int, void (*)()> Autonomous::matchAuton;
std::map<int, void (*)()> Autonomous::skillsAuton;
std::map<int, typeName> Autonomous::slotName;
std::map<int, typeName> Autonomous::slotAbbv;

void autonomous()
{
    Chassis chassis;
    chassis.reset();

    Autonomous Auton;
    Auton.runAuton();
}

Autonomous::Autonomous()
{ // The autons will be stored in this order, starting from 0.
    if (!isInitialized)
    {

        // RED
        addAuton(SLOT_MATCH, "Home row", "RSZns", matchhomerow);
        addAuton(SLOT_MATCH, "Home center", "HC", matchhomecenter);

        // Skills
        addAuton(SLOT_SKILLS, "Tester", "Tester", tester); // Slot 0
        addAuton(SLOT_SKILLS, "Skills 1", "Skills1", skills1);

        isInitialized = true;
    }
}

void Autonomous::runAuton()
{
    switch (type)
    {
    case SLOT_MATCH:
        matchAuton[slot]();
        break;
    case SLOT_SKILLS:
        skillsAuton[slot]();
        break;

    default:
        cout << "Auton not selected" << endl;
        break;
    }
}

void Autonomous::addAuton(int type_, std::string autonName, std::string autonAbbv, void (*function)())
{
    switch (type_)
    {
    case SLOT_MATCH:
    {
        matchAuton.insert(make_pair(matchAuton.size(), function));
        slotName[matchAuton.size() - 1].match = autonName;
        slotAbbv[matchAuton.size() - 1].match = autonAbbv;
        break;
    }
    
    case SLOT_SKILLS:
    {
        skillsAuton.insert(make_pair(skillsAuton.size(), function));
        slotName[skillsAuton.size() - 1].skills = autonName;
        slotAbbv[skillsAuton.size() - 1].skills = autonAbbv;
        break;
    }

    default:
    {
        cout << "Auton name " << autonName << " not registered" << endl;
        break;
    }
    }
}

int Autonomous::getSlot()
{
    return slot;
}

void Autonomous::setSlot(int slot_)
{
    slot = slot_;
}

int Autonomous::getType()
{
    return type;
}

void Autonomous::setType(int type_)
{
    type = type_;
}

int Autonomous::getSize(int type_)
{
    switch (type_)
    {
    case 1:
    {
        return matchAuton.size();
        break;
    }
    case 2:
    {
        return skillsAuton.size();
        break;
    }
    case 3:
    {
        return slotName.size();
        break;
    }

    default:
    {
        return slotName.size();
        break;
    }
    }
}

std::string Autonomous::getName(int type_, int slot_)
{
    switch (type_)
    {
    case SLOT_MATCH:
    {
        return slotName[slot_].match;
        break;
    }

    case SLOT_SKILLS:
    {
        return slotName[slot_].skills;
        break;
    }

    default:
    {
        return "Incorrect Type";
        break;
    }
    }
}

std::string Autonomous::getAbbv(int type_, int slot_)
{
    switch (type_)
    {
    case SLOT_MATCH:
    {
        return slotAbbv[slot_].match;
        break;
    }

    case SLOT_SKILLS:
    {
        return slotAbbv[slot_].skills;
        break;
    }

    default:
    {
        return "Incorrect Type";
        break;
    }
    }
}

void Autonomous::start(void *ignore)
{
    pros::delay(500);
    Autonomous *that = static_cast<Autonomous *>(ignore);
    that->run();
}

void Autonomous::run()
{
    Display display;

    while (true)
    {
        display.setRemoteText(getName(getType(), getSlot()));
        pros::delay(50);
    }
}
