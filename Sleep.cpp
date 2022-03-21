#include "Sleep.hpp"

#include<iostream>
Sleep::Sleep(tm *startTime) : 
startTimeOfSleepingHour(startTime->tm_hour),
startTimeOfSleepingMin(startTime->tm_min){

}

void Sleep::UpdateSleepTime(tm *currentTime, Glib::RefPtr<Gtk::Label> label){
    std::string text = "Czas snu: ";
    int hour = currentTime->tm_hour - startTimeOfSleepingHour;
    int minutes = currentTime->tm_min - startTimeOfSleepingMin;
    text += std::to_string(hour) + ":" + std::to_string(minutes);
    label->set_text(text);
}