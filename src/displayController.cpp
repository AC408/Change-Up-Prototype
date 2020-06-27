#include "main.h"

#include "displayController.h"
#include "autonController.h"

#include "misc.h"

#include "chassis.h"
#include "intake.h"
#include "conveyor.h"

static Autonomous Auton;
static Conveyor conveyor;
static Intake intake;
static Chassis chassis;

bool Display::isRunning = false,
     Display::isInitialized = false;

int Display::currScr = 1;

std::string Display::setText = "",
            Display::lastText = "";

std::vector<info> Display::updateInfo;
std::vector<lv_obj_t *> Display::autonm;

static int auton_type = 1;

static lv_style_t style_match;
static lv_style_t style_skills;

static lv_style_t style_match_released;
static lv_style_t style_skills_released;

static lv_obj_t *scr;
static lv_obj_t *tv;

static lv_obj_t *autonName;
static lv_obj_t *intakeLine;
static lv_obj_t *conveyorLine;
static lv_obj_t *intakeLine;
static lv_obj_t *imuHeading;

static lv_obj_t *global_x;
static lv_obj_t *global_y;
static lv_obj_t *global_a;


static lv_res_t btn_click_action(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  switch(id) {
    case 1: conveyor.zero(); break;
    case 2: intake.zero(); break;
    case 3: chassis.resetIMU(); break;

    default: break;
  }

  return LV_RES_OK;
}

static lv_res_t match_click_action(lv_obj_t *btn)
{
    int id = lv_obj_get_free_num(btn);

    Auton.setType(1);
    Auton.setSlot(id);

    return LV_RES_OK;
}

static lv_res_t skills_click_action(lv_obj_t *btn)
{
    int id = lv_obj_get_free_num(btn);

    Auton.setType(2);
    Auton.setSlot(id);

    return LV_RES_OK;
}

Display::Display()
{
    if (!isInitialized)
    {
        // Theme & Style init
        lv_theme_t *th = lv_theme_material_init(0, NULL);

        th->bg->body.main_color = LV_COLOR_HEX(0x1a1a1a);
        th->bg->body.grad_color = LV_COLOR_HEX(0x1a1a1a);

        th->tabview.btn.bg->body.main_color = LV_COLOR_HEX(0x1a1a1a);
        th->tabview.btn.bg->body.grad_color = LV_COLOR_HEX(0x1a1a1a);
        th->tabview.btn.bg->body.shadow.width = 0;

        th->tabview.btn.bg->text.color = LV_COLOR_WHITE;
        // th->tabview.btn.rel->text.color = LV_COLOR_WHITE;
        // th->tabview.btn.tgl_rel->text.color = LV_COLOR_WHITE;

        lv_theme_set_current(th);

        lv_style_plain.body.radius = 1;

        // Splash screen
        lv_obj_t *splashImg = lv_img_create(lv_layer_sys(), nullptr);
        lv_img_set_src(splashImg, &splash);
        lv_obj_set_pos(splashImg, 0, -15);

        static lv_style_t preloadStyle;
        lv_style_copy(&preloadStyle, &lv_style_plain);
        preloadStyle.line.width = 3;
        preloadStyle.line.color = LV_COLOR_RED;

        preloadStyle.body.border.color = LV_COLOR_TRANSP;
        preloadStyle.body.padding.hor = 0;

        lv_obj_t *preload = lv_preload_create(lv_layer_sys(), NULL);
        lv_obj_set_size(preload, 40, 40);
        lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 80);
        lv_preload_set_style(preload, LV_PRELOAD_STYLE_MAIN, &preloadStyle);

        // Overlay
        lv_obj_t *overlay = lv_cont_create(lv_layer_top(), NULL);
        lv_obj_set_size(overlay, 480, 35);
        lv_obj_set_pos(overlay, 0, 215);

        autonName = createLabel(0, 0, "Selected Autonomous: ", overlay);
        lv_obj_align(autonName, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);

        // Auton Btn Style
        lv_style_copy(&style_match, &lv_style_plain);
        style_match.body.main_color = LV_COLOR_RED;
        style_match.body.grad_color = LV_COLOR_RED;
        style_match.body.border.color = LV_COLOR_WHITE;
        style_match.body.border.width = 2;
        style_match.text.color = LV_COLOR_WHITE;

        lv_style_copy(&style_skills, &lv_style_plain);
        style_skills.body.main_color = LV_COLOR_RED;
        style_skills.body.grad_color = LV_COLOR_RED;
        style_skills.body.border.color = LV_COLOR_WHITE;
        style_skills.body.border.width = 2;
        style_skills.text.color = LV_COLOR_WHITE;

        // Released
        lv_style_copy(&style_match_released, &lv_style_plain);
        style_match_released.body.main_color = LV_COLOR_MAKE(48, 48, 48);
        style_match_released.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
        style_match_released.body.border.color = LV_COLOR_RED;
        style_match_released.body.border.width = 2;
        style_match_released.text.color = LV_COLOR_WHITE;

        lv_style_copy(&style_skills_released, &lv_style_plain);
        style_skills_released.body.main_color = LV_COLOR_MAKE(48, 48, 48);
        style_skills_released.body.grad_color = LV_COLOR_MAKE(48, 48, 48);
        style_skills_released.body.border.color = LV_COLOR_RED;
        style_skills_released.body.border.width = 2;
        style_skills_released.text.color = LV_COLOR_WHITE;

        pros::delay(200);

        scr = lv_cont_create(NULL, NULL);
        lv_scr_load(scr);

        tv = lv_tabview_create(scr, NULL);
        lv_obj_set_pos(tv, 0, 0);
        lv_obj_set_size(tv, 480, 250);
        lv_tabview_set_sliding(tv, false);

        tab1 = lv_tabview_add_tab(tv, "Match");
        tab2 = lv_tabview_add_tab(tv, "Skills");
        tab3 = lv_tabview_add_tab(tv, "Sensor");
        tab4 = lv_tabview_add_tab(tv, "Settings");

        tabMatch(tab1);
        tabSkills(tab2);
        tabSensor(tab3);
        tabSetting(tab4);

        lv_tabview_set_tab_act(tv, 0, false);

        isInitialized = true;
    }
}

void Display::cleanUp()
{
    lv_obj_clean(lv_layer_sys());
}

void Display::tabMatch(lv_obj_t *parent)
{
    lv_obj_t *btnmr[50];

    int size = Auton.getSize(SLOT_MATCH);
    for (int i = 0; i < size; i++)
    {
        if ((i + 1) % 2 == 1)
        {
            btnmr[i] = createButton(i, 0, (int)(i / 2) * 35 + 20, 220, 30, Auton.getName(1, i), parent, match_click_action);
        }
        else
        {
            btnmr[i] = createButton(i, 230, (int)(i / 2) * 35 + 20, 220, 30, Auton.getName(1, i), parent, match_click_action);
        }

        lv_btn_set_style(btnmr[i], LV_BTN_STYLE_PR, &style_match);
        lv_btn_set_style(btnmr[i], LV_BTN_STYLE_REL, &style_match_released);
    }
}

void Display::tabSkills(lv_obj_t *parent)
{
    lv_obj_t *btnms[50];

    int size = Auton.getSize(SLOT_SKILLS);
    for (int i = 0; i < size; i++)
    {
        if ((i + 1) % 2 == 1)
        {
            btnms[i] = createButton(i, 0, (int)(i / 2) * 35 + 20, 200, 30, Auton.getName(2, i), parent, skills_click_action);
        }
        else
        {
            btnms[i] = createButton(i, 230, (int)(i / 2) * 35 + 20, 200, 30, Auton.getName(2, i), parent, skills_click_action);
        }

        lv_btn_set_style(btnms[i], LV_BTN_STYLE_PR, &style_skills);
        lv_btn_set_style(btnms[i], LV_BTN_STYLE_REL, &style_skills_released);
    }
}

void Display::tabSensor(lv_obj_t *parent)
{
    lv_obj_t * conveyorTracker = createButton(1, 230, 20, 200, 30, "Reset Conveyor line", parent, btn_click_action, &style_skills, &style_skills_released);
    lv_obj_t * intakeTracker = createButton(2, 230, 55, 200, 30, "Reset Intake line", parent, btn_click_action, &style_skills, &style_skills_released);
    lv_obj_t * resetIMU = createButton(3, 230, 90, 200, 30, "Reset IMU", parent, btn_click_action, &style_skills, &style_skills_released);

    lv_obj_t *intakeOverlay = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(intakeOverlay, 200, 30);
    lv_obj_set_pos(intakeOverlay, 0, 20);

    intakeLine = createLabel(0, 0, "Intake Line: ", intakeOverlay);
    lv_obj_align(intakeLine, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);

    // lv_obj_t *conveyorOverlay = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_size(conveyorOverlay, 200, 30);
    // lv_obj_set_pos(conveyorOverlay, 0, 20);

    // conveyorLine = createLabel(0, 0, "Conveyor Line: ", conveyorOverlay);
    // lv_obj_align(conveyorLine, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);
    
    // lv_obj_t *imuOverlay = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_size(imuOverlay, 200, 30);
    // lv_obj_set_pos(imuOverlay, 0, 20);

    // imuHeading = createLabel(0, 0, "Imu Heading: ", imuOverlay);
    // lv_obj_align(imuHeading, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);
    

    //display Intake Linetracker       //reset Intake Linetracker
    //display Conveyor Linetracker     //reset Conveyor Linetracker
    //display both IMU                 //reset both IMU
    //calculate heading                 

    //encoder?
}

void Display::tabSetting(lv_obj_t *parent){

    lv_obj_t *x_track = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(x_track, 200, 30);
    lv_obj_set_pos(x_track, 0, 20);

    global_x = createLabel(0, 0, "X: ", x_track);
    lv_obj_align(global_x, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);
    
    // lv_obj_t *y_track = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_size(y_track, 200, 30);
    // lv_obj_set_pos(y_track, 0, 20);

    // global_y = createLabel(0, 0, "Y: ", y_track);
    // lv_obj_align(global_y, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);
    
    // lv_obj_t *a_track = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_size(a_track, 200, 30);
    // lv_obj_set_pos(a_track, 0, 20);

    // global_a = createLabel(0, 0, "Theta:", a_track);
    // lv_obj_align(global_a, NULL, LV_ALIGN_IN_LEFT_MID, 5, -4);
    
    //display current x, y, theta
    //    //display current pid for chassis
}

void Display::start(void *ignore)
{
    pros::delay(500);
    Display *that = static_cast<Display *>(ignore);
    that->run();
}

void Display::run()
{
    int nowType, lastType, nowSlot, lastSlot;

    isRunning = true;

    while (isRunning)
    {

        nowType = Auton.getType();
        nowSlot = Auton.getSlot();

        if (nowType != lastType || nowSlot != lastSlot)
        {
            std::string temp = "Selected Autonomous: " + Auton.getName(nowType, nowSlot);
            lv_label_set_text(autonName, temp.c_str());

            lastType = nowType;
            lastSlot = nowSlot;
        }

        for (int i = 0; i < updateInfo.size(); i++)
        {
            if (updateInfo[i].labelObj == nullptr)
                break;

            std::string temp = "";

            switch (updateInfo[i].type)
            {
            case 'i':
                temp = std::to_string(*(int *)updateInfo[i].data);
                break;
            case 'd':
                temp = std::to_string(*(double *)updateInfo[i].data);
                break;
            case 'b':
                temp = std::to_string(*(bool *)updateInfo[i].data);
                break;
            default:
                temp = std::to_string(*(double *)updateInfo[i].data);
                break;
            }

            if (temp != updateInfo[i].last)
            {
                std::string update = updateInfo[i].name + ": " + temp;
                lv_label_set_text(updateInfo[i].labelObj, update.c_str());
                updateInfo[i].last = temp;
            }
        }

        if(lv_tabview_get_tab_act(tv)==lv_tabview_get_tab_act(tab3)){
            std::string temp = "Intake Line: " + std::to_string(intake.get_line());
            lv_label_set_text(intakeLine, temp.c_str());
        }

        if(lv_tabview_get_tab_act(tv)==lv_tabview_get_tab_act(tab4)){
            std::string temp = "X: " + std::to_string(chassis.getX());
            lv_label_set_text(global_x, temp.c_str());
        }

        remoteUpdate();

        // std::cout << "Display Running" << std::endl;

        pros::delay(10);
    }
}

void Display::stop()
{
    isRunning = false;
}

Display &Display::addInfo(std::string name, char type, void *info)
{
    updateInfo.push_back(::info());

    int current = updateInfo.size() - 1;

    updateInfo[current].labelObj = lv_label_create(lv_tabview_get_tab(tv, 3), nullptr);

    if ((current + 1) % 2 == 1)
        lv_obj_set_pos(updateInfo[current].labelObj, 0, (int)(current / 2) * 20 + 20);
    else
        lv_obj_set_pos(updateInfo[current].labelObj, 250, (int)(current / 2) * 20 + 20);

    static lv_style_t infoStyle;
    lv_style_copy(&infoStyle, &lv_style_plain);
    infoStyle.text.color = LV_COLOR_WHITE;
    lv_obj_set_style(updateInfo[current].labelObj, &infoStyle);

    updateInfo[current].name = name;
    updateInfo[current].type = type;
    updateInfo[current].data = info;
    updateInfo[current].last = "";
    return *this;
}

void Display::setRemoteText(std::string text_)
{
    setText = text_;
}

void Display::remoteUpdate()
{
    if (setText != lastText)
    {
        io::master.set_text(0, 0, setText.c_str());
        lastText = setText;
    }
}

lv_obj_t *Display::createLabel(int x, int y, std::string text_, lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent, NULL);
    lv_obj_set_pos(label, x, y);
    lv_label_set_text(label, text_.c_str());

    return label;
}

lv_obj_t *Display::createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t *parent, lv_action_t action, lv_style_t *btn_pr, lv_style_t *btn_rel)
{
    lv_obj_t *button = lv_btn_create(parent, NULL);
    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, width, height);
    lv_obj_set_free_num(button, id);
    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);

    if (btn_pr != nullptr || btn_rel != nullptr)
    {
        lv_btn_set_style(button, LV_BTN_STYLE_PR, btn_pr);
        lv_btn_set_style(button, LV_BTN_STYLE_REL, btn_rel);
    }
    else
    {
        lv_btn_set_style(button, LV_BTN_STYLE_PR, lv_theme_get_current()->btn.pr);
        lv_btn_set_style(button, LV_BTN_STYLE_REL, lv_theme_get_current()->btn.rel);
    }

    lv_obj_t *buttonLabel = createLabel(0, 0, text, button);
    lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

    return button;
}
