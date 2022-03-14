#include <gtkmm.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include <ctime>

#include <chrono>
#include <thread>
#include <vector>

//#include <wiringPi.h>

#include "Alarm.hpp"

const size_t MAIN_PAGE_CLOCK = 0;
const size_t MAIN_PAGE_SETTINGS = 1;
const size_t MAIN_PAGE_PULSE = 2;
const size_t MAIN_PAGE_ADD_ALARM = 3;
const size_t MAIN_PAGE_SLEEPING = 4;
const size_t MAIN_PAGE_WAKE_UP = 5;


Glib::RefPtr<Gtk::Builder> builder;
Alarm* currentAlarm;
std::vector<Alarm*> alarms;

void printHello(std::string name) { 
    std::cout << "Hello, " << name << '\n';
}

void UpdateClockHMS(Glib::RefPtr<Gtk::Label> clockLabel, tm *currenttime) {
    std::stringstream timestream;
    timestream
        << std::setw(2)
        << std::setfill('0')
        << currenttime->tm_hour
        << ':'
        << std::setw(2)
        << std::setfill('0')
        << currenttime->tm_min
        << ':'
        << std::setw(2)
        << std::setfill('0')
        << currenttime->tm_sec;
    std::string timestr;
    timestream >> timestr;
    clockLabel->set_text(timestr);
}

void UpdateClockHM(Glib::RefPtr<Gtk::Label> clockLabel, tm *currenttime) {
    std::stringstream timestream;
    timestream
        << std::setw(2)
        << std::setfill('0')
        << currenttime->tm_hour
        << ':'
        << std::setw(2)
        << std::setfill('0')
        << currenttime->tm_min;
    std::string timestr;
    timestream >> timestr;
    clockLabel->set_text(timestr);
}

void ShowAlarm(const Alarm& alarm){
// Gtk::Window *ringWindow;
    // builder->get_widget("RingWindow", ringWindow);
    // ringWindow->show();
    auto pageContainer = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    auto alarmHourLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AlarmHourWakeUpLabel"));
    auto alarmModeLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AlarmModeWakeUpLabel"));
    auto alarmNameLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AlarmNameWakeUpLabel"));
    pageContainer->set_current_page(MAIN_PAGE_WAKE_UP);
    alarmHourLabel->set_text("Godzina:" + std::to_string(alarm.hour) + ":" + std::to_string(alarm.minute));
    alarmNameLabel->set_text(alarm.name);
}

void Ring(){
    while(system("aplay yoo.wav") == 0);
}

void StopAlarm() {
    system("killall aplay");
    auto pageContainer = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    pageContainer->set_current_page(MAIN_PAGE_CLOCK);
}

void CheckAlarms(tm *currenttime){
    for(auto it : alarms){
        if(currenttime->tm_hour == it->hour && currenttime->tm_min == it->minute && it->enable){
            ShowAlarm(*it);
            std::thread ringThread(Ring);
            ringThread.detach();
            it->enable = false;
            break;
        }
    }
}

void KeepUpdatingClock(Glib::RefPtr<Gtk::Label> clockLabel, Glib::RefPtr<Gtk::Label> smallclockLabel) {
    time_t timer;
    tm *currentTime;
    while(true) { 
        time(&timer);
        currentTime = localtime(&timer);
        UpdateClockHM(clockLabel, currentTime);
        UpdateClockHMS(smallclockLabel, currentTime);
        CheckAlarms(currentTime);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

bool OnMainClockClicked(GdkEventButton*){
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    pages->set_current_page(MAIN_PAGE_SETTINGS);
    return true;
}

bool OnSecoundClockClicked(GdkEventButton*){
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    pages->set_current_page(MAIN_PAGE_CLOCK);
    return true;
}

void EditAlarm(int i){
    Glib::RefPtr<Gtk::Label> addHourLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddHourLabel"));
    Glib::RefPtr<Gtk::Label> addMinuteLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddMinutesLabel"));
    Glib::RefPtr<Gtk::Entry> nameInput = Glib::RefPtr<Gtk::Entry>::cast_dynamic(builder->get_object("AlarmNameInput"));

    if(i != -1){
        currentAlarm = alarms[i];
        alarms.erase(alarms.begin()+i);
    }

    std::stringstream timestream;
    std::stringstream timestream2;
    std::string s;
    timestream 
        << std::setw(2)
        << std::setfill('0')
        << currentAlarm->hour;
    timestream >> s;
    addHourLabel->set_text(s);
    timestream2 
        << std::setw(2)
        << std::setfill('0')
        << currentAlarm->minute;
    timestream2 >> s;
    addMinuteLabel->set_text(s);
    nameInput->set_text(currentAlarm->name);

    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    Glib::RefPtr<Gtk::Notebook> pages2 = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("AddAlarmPages"));
    pages->set_current_page(MAIN_PAGE_ADD_ALARM);
    pages2->set_current_page(0);
}

void OnAlarmAddClicked(){
    currentAlarm = new Alarm();
    EditAlarm(-1);
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    Glib::RefPtr<Gtk::Notebook> pages2 = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("AddAlarmPages"));
    pages->set_current_page(MAIN_PAGE_ADD_ALARM);
    pages2->set_current_page(0);
}

void OnCheckPulseClicked(){
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    pages->set_current_page(MAIN_PAGE_PULSE);
}

void AddHour(){
    Glib::RefPtr<Gtk::Label> addHourLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddHourLabel"));
    int i = 1;
    if(currentAlarm->hour + i < 0){
        i -= currentAlarm->hour;
        currentAlarm->hour = 24 - i;
    }else if(currentAlarm->hour + i > 23){
        i -= 24 - currentAlarm->hour;
        currentAlarm->hour = i;
    }else{
        currentAlarm->hour += i;
    }
    std::stringstream labelStream;
    labelStream << std::setw(2) << std::setfill('0') << currentAlarm->hour;
    std::string labelText;
    labelStream >> labelText;
    addHourLabel->set_text(labelText);
}

void AddMinute()
{
    Glib::RefPtr<Gtk::Label> addMinuteLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddMinutesLabel"));
    int i = 1;
    if(currentAlarm->minute + i < 0){
        i -= currentAlarm->minute;
        currentAlarm->minute = 60 - i;
    }else if(currentAlarm->minute + i > 59){
        i -= 60 - currentAlarm->minute;
        currentAlarm->minute = i;
    }else{
        currentAlarm->minute += i;
    }
    std::stringstream labelStream;
    labelStream << std::setw(2) << std::setfill('0') << currentAlarm->minute;
    std::string labelText;
    labelStream >> labelText;
    addMinuteLabel->set_text(labelText);
}

void MinusMinute()
{
    Glib::RefPtr<Gtk::Label> addMinuteLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddMinutesLabel"));
    int i = -1;
    if(currentAlarm->minute + i < 0){
        i -= currentAlarm->minute;
        currentAlarm->minute = 58 - i;
    }else if(currentAlarm->minute + i > 59){
        i -= 60 - currentAlarm->minute;
        currentAlarm->minute = i;
    }else{
        currentAlarm->minute += i;
    }
    std::stringstream labelStream;
    labelStream << std::setw(2) << std::setfill('0') << currentAlarm->minute;
    std::string labelText;
    labelStream >> labelText;
    addMinuteLabel->set_text(labelText);
}

void MinusHour(){
    Glib::RefPtr<Gtk::Label> addHourLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("AddHourLabel"));
    int i = -1;
    if(currentAlarm->hour + i < 0){
        i -= currentAlarm->hour;
        currentAlarm->hour = 22 - i;
    }else if(currentAlarm->hour + i > 23){
        i -= 24 - currentAlarm->hour;
        currentAlarm->hour = i;
    }else{
        currentAlarm->hour += i;
    }
    std::stringstream labelStream;
    labelStream << std::setw(2) << std::setfill('0') << currentAlarm->hour;
    std::string labelText;
    labelStream >> labelText;
    addHourLabel->set_text(labelText);
}

void SwitchEnable(int i){
    alarms[i]->enable = !(alarms[i]->enable);
}

void UpdateAlarms(){
    Glib::RefPtr<Gtk::ListBox> alarmsList = Glib::RefPtr<Gtk::ListBox>::cast_dynamic(builder->get_object("AlarmsList"));
    for(auto it : alarmsList->get_children()){
        alarmsList->remove(*it);
        delete it;
    }
    for(int i = 0; i < alarms.size(); i++){
        Gtk::Box* box = new Gtk::Box();
        Gtk::Label* label = new Gtk::Label("AAA");

        std::stringstream textStream;
        textStream
            << std::setw(2)
            << std::setfill('0')
            << alarms[i]->hour
            << ':'
            << std::setw(2)
            << std::setfill('0')
            << alarms[i]->minute;
        
        std::string text;
        textStream >> text;
        
        std::string id = std::to_string(i+1) + ".       ";
        text.insert(0, id);
        text += "   " + alarms[i]->name;
        label->set_text(text);

        Gtk::CheckButton *is_enable = new Gtk::CheckButton();
        is_enable->set_active(alarms[i]->enable);
        is_enable->signal_clicked().connect(sigc::bind<int>(sigc::ptr_fun(&SwitchEnable), i));

        Gtk::Button *editButton = new Gtk::Button();
        editButton->set_label("Edytuj");
        editButton->signal_clicked().connect(sigc::bind<int>(sigc::ptr_fun(&EditAlarm), i));

        box->pack_start(*label, Gtk::PACK_EXPAND_PADDING);
        box->pack_start(*is_enable, Gtk::PACK_EXPAND_PADDING);
        box->pack_start(*editButton, Gtk::PACK_EXPAND_PADDING);

        Gtk::ListBoxRow* row = new Gtk::ListBoxRow();
        row->add(*box);
        alarmsList->append(*row);
        
    }  
    alarmsList->show_all_children();  
}
void AddAlarms(){

    Glib::RefPtr<Gtk::Entry> nameInput = Glib::RefPtr<Gtk::Entry>::cast_dynamic(builder->get_object("AlarmNameInput"));
    currentAlarm->name = nameInput->get_text();
    
    Glib::RefPtr<Gtk::Box> repeatingCheckBoxContainer = Glib::RefPtr<Gtk::Box>::cast_dynamic(builder->get_object("AlarmRepeating"));

    size_t i = 0;
    for(auto it : repeatingCheckBoxContainer->get_children()) {
        
        Gtk::CheckButton *checkbox = (Gtk::CheckButton*) it;
        if(checkbox->get_active()) {
            currentAlarm->repeating_days[i] = true;
        }
        else currentAlarm->repeating_days[i] = false;
        i++;
    }

    alarms.push_back(currentAlarm);
    
    UpdateAlarms();

    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("PageContainer"));
    pages->set_current_page(MAIN_PAGE_SETTINGS);
}

void NextPage(){
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("AddAlarmPages"));
    pages->next_page();
}

void PreviousPage(){
    Glib::RefPtr<Gtk::Notebook> pages = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(builder->get_object("AddAlarmPages"));
    pages->prev_page();
}

int main(int argc, char *argv[]){
    auto app = Gtk::Application::create(argc, argv, "pl.edu.zse.BigBenTen");
    builder = Gtk::Builder::create_from_file("window.glade");
    Gtk::Window *mainwindow;
    builder->get_widget("MainWindow", mainwindow);
    currentAlarm = new Alarm();
/*
    wiringPiSetup();
    pinMode(22, OUTPUT);
    digitalWrite(22, 1);
*/


    Glib::RefPtr<Gtk::Label> clockLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("ClockLabel"));
    Glib::RefPtr<Gtk::Label> smallClockLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("ClockLabel2"));
    Glib::RefPtr<Gtk::EventBox> clockEvent = Glib::RefPtr<Gtk::EventBox>::cast_dynamic(builder->get_object("ClockEventBox"));
    Glib::RefPtr<Gtk::EventBox> smallclockEvent = Glib::RefPtr<Gtk::EventBox>::cast_dynamic(builder->get_object("ClockEventBox2"));

    Glib::RefPtr<Gtk::Button> checkPulseButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("CheckPulseButton"));
    Glib::RefPtr<Gtk::Button> addAlarmButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("AddAlarmButton"));
    Glib::RefPtr<Gtk::Button> backButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("BackButton"));

    Glib::RefPtr<Gtk::Button> minusHButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("minusH"));
    Glib::RefPtr<Gtk::Button> minusMButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("minusM"));
    Glib::RefPtr<Gtk::Button> plusHButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("plusH"));
    Glib::RefPtr<Gtk::Button> plusMButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("plusM"));

    Glib::RefPtr<Gtk::Button> addAlarmsButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("addAlarm"));
    Glib::RefPtr<Gtk::Button> nextButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("NextButton"));
    Glib::RefPtr<Gtk::Button> previousButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("PreviousButton"));
    Glib::RefPtr<Gtk::Button> cancelButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("CancelButton"));

    Glib::RefPtr<Gtk::Button> stopAlarmButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(builder->get_object("StopAlarmButton"));



    clockEvent->signal_button_press_event().connect(sigc::ptr_fun(&OnMainClockClicked));
    smallclockEvent->signal_button_press_event().connect(sigc::ptr_fun(&OnSecoundClockClicked));
    checkPulseButton->signal_clicked().connect(sigc::ptr_fun(&OnAlarmAddClicked));
    addAlarmButton->signal_clicked().connect(sigc::ptr_fun(&OnAlarmAddClicked));
    backButton->signal_button_press_event().connect(sigc::ptr_fun(&OnMainClockClicked));

    stopAlarmButton->signal_clicked().connect(sigc::ptr_fun(&StopAlarm));
    plusHButton->signal_clicked().connect(sigc::ptr_fun(&AddHour));
    minusHButton->signal_clicked().connect(sigc::ptr_fun(&MinusHour));
    plusMButton->signal_clicked().connect(sigc::ptr_fun(&AddMinute));
    minusMButton->signal_clicked().connect(sigc::ptr_fun(&MinusMinute));

    addAlarmsButton->signal_clicked().connect(sigc::ptr_fun(&AddAlarms));
    nextButton->signal_clicked().connect(sigc::ptr_fun(&NextPage));
    previousButton->signal_clicked().connect(sigc::ptr_fun(&PreviousPage));
    cancelButton->signal_button_press_event().connect(sigc::ptr_fun(&OnMainClockClicked));  

    std::thread clockUpdater(KeepUpdatingClock,clockLabel,smallClockLabel);
    app->run(*mainwindow);
}

