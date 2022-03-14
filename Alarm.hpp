#pragma once

#include <gtkmm.h>
#include <array>


class Alarm
{
public:
    int hour = 0;
    int minute = 0;
    std::string name = "";
    bool enable = true;
    std::array<bool,7> repeating_days {false, false, false, false, false, false, false};
};

