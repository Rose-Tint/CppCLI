#include <sstream>

#include "ColorCode.hpp"


std::ostream& operator << (std::ostream& os, cli::ColorCode cc)
{
    // ESC << [ << code << 'm'
    os << "\033["<< (int)(cc) << "m" << std::flush;
    return os;
}


std::string operator + (cli::ColorCode a, cli::ColorCode b)
{
    std::stringstream ss;
    ss << "\033[" << int(a) << ";" << int(b) << "m";
    return ss.str();
}


std::string operator + (cli::ColorCode cc, std::string str)
{
    std::stringstream ss;
    ss << "\033[" << int(cc) << "m" << str;
    return ss.str();
}


std::string operator + (std::string str, cli::ColorCode cc)
{
    std::stringstream ss;
    ss << str << "\033[" << int(cc) << "m";
    return ss.str();
}

