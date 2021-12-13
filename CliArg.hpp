#pragma once

#include <string>


namespace cli
{
    class CommandLineInterface;

    struct CliArg
    {
        using Manip_f = void (*)(const CommandLineInterface*);

        CliArg(std::string _name, Manip_f _action = nullptr, std::string _desc = "")
            : name(_name), action(_action), desc(_desc) { }

        std::string name;
        Manip_f action = nullptr;
        std::string desc = "";

        bool operator == (const CliArg& other) const
            { return name == other.name; }
    };
}


template < >
struct std::hash<cli::CliArg>
{
    std::size_t operator () (const cli::CliArg& arg) const
        { return str_hash(arg.name); }
    std::size_t operator () (const std::string& arg) const
        { return str_hash(arg); }

  private:
    std::hash<std::string> str_hash { };
};
