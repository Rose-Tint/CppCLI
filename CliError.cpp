#include <sstream>

#include "CliError.hpp"
#include "ColorCode.hpp"


using namespace cli;


using CC = ColorCode;


void CliError::SetCommandLine(std::vector<std::string> args)
{
    for (const std::string& arg : args)
        cmdLine.append(arg + ' ');
    cmdLine.pop_back(); // gets rid of extrenous space
}


CliError::CliError(std::string msg, const std::string arg)
    : CliError(std::move(msg), cmdLine.find(' ' + arg) + 1) { }


CliError::CliError(std::string msg, std::size_t idx)
{
    using std::string;

    idx = std::min(idx, cmdLine.size());
    std::stringstream ss;
    ss << CC::Bold << CC::Red <<
    "\nError at column " << idx << ":\n\t" << msg <<
    "\n\t" << CC::BrightGreen << cmdLine << "\n\t" <<
    CC::Cyan << string(idx, '~') <<
    CC::Red << '^' <<
    CC::Cyan << string(cmdLine.size() - idx, '~') <<
    CC::Reset;
    message = ss.str();
}
