#pragma once
#include <iostream>
#include <map>

#define SLOT_MATCH 1
#define SLOT_SKILLS 2

struct typeName
{
    std::string match;
    std::string skills;
};

class Autonomous
{
public:
    Autonomous();

    void runAuton();
    void addAuton(int type_, std::string autonName, std::string autonAbbv, void (*function)());

    int getSlot();
    void setSlot(int slot_);
    int getType();
    void setType(int type_);

    int getSize(int type_);
    std::string getName(int type_, int slot_);
    std::string getAbbv(int type_, int slot_);

    void run();
    static void start(void *ignore);

private:
    static bool isInitialized;
    static int type, slot;

    static std::map<int, void (*)()> matchAuton;
    static std::map<int, void (*)()> skillsAuton;
    static std::map<int, typeName> slotName;
    static std::map<int, typeName> slotAbbv;
};

/*===========================================
  AUTON DECLARATIONS
===========================================*/

void tester();

// Red
void matchhomerow();
void matchhomecenter();

// Skills
void skills1();
