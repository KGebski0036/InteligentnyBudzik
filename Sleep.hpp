#pragma once

#include <ctime>
#include <gtkmm.h>
#include <string>

class Sleep
{
public:
    Sleep(tm* startTime);
    void UpdateSleepTime(tm *currentTime, Glib::RefPtr<Gtk::Label> label);
    int startTimeOfSleepingHour;
    int startTimeOfSleepingMin;
};
