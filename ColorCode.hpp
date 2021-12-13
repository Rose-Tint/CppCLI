#pragma once

#include <iostream>
#include <string>


namespace cli
{
    enum struct ColorCode : int
    {
        Reset = 0,
        Bold = 1,
        Red = 31,
        Green = 32,
        Blue = 34,
        BrightRed = 91,
        BrightGreen = 92,
        Yellow = 93,
        Cyan = 96,
    };
}


std::ostream& operator << (std::ostream& os, cli::ColorCode cc);
std::string operator + (cli::ColorCode a, cli::ColorCode b);
std::string operator + (cli::ColorCode cc, std::string str);
std::string operator + (std::string str, cli::ColorCode cc);
