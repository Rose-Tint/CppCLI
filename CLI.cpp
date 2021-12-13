#include <iomanip>
#include <iostream>

#include "CLI.hpp"
#include "CliError.hpp"

using namespace cli;


CommandLineInterface::CommandLineInterface(const int argc, const char** argv)
    : args(argv, argv + argc)
{
    s_threads.JoinAll();
    if (argc <= 1)
        return;
    std::vector<Manip_f> actions;
    StringVec::const_iterator iter = args.begin() + 1;
    if (s_commands.count({*iter}) != 0)
    {
        auto cmd = s_commands.find({*iter});
        if (cmd->action != nullptr)
            actions.push_back(cmd->action);
        set_command(*iter++);
    }
    while (iter < args.end() && iter->front() != '-')
    {
        // dont launch in new thread because order might matter
        add_arg(*iter++);
    }

    while (iter < args.end())
    {
        std::string arg = *iter;
        CliSet::const_iterator opts_iter = s_options.find({arg});
        CliSet::const_iterator flgs_iter = s_flags.find({arg});
        if (opts_iter != s_options.end())
        {
            if (opts_iter->action != nullptr)
                actions.push_back(opts_iter->action);
            StringVec opt_args;
            iter += 1;
            while (iter < args.end() && iter->front() != '-')
                opt_args.push_back(*iter++);
            iter -= 1;
            // set_option(arg, opt_args);
            threads.Launch(&CommandLineInterface::set_option, this, arg, opt_args);
        }
        else if (flgs_iter != s_flags.end())
        {
            if (flgs_iter->action != nullptr)
                actions.push_back(flgs_iter->action);
            // add_flag(arg);
            threads.Launch(&CommandLineInterface::add_flag, this, arg);
        }
        else throw CliError("Command line argument not recognized : " + arg, arg);
        iter += 1;
    }
    threads.JoinAll();
    for (Manip_f fn : actions)
        fn(this);
}


void CommandLineInterface::MakeCommand(const std::string name, Manip_f action, std::string desc)
{
    s_threads.Launch([name, action, desc]{
        if (s_options.count({name}) != 0)
            throw CliError("Command overlaps option: " + name, name);
        if (s_flags.count({name}) != 0)
            throw CliError("Command overlaps flag: " + name, name);

        auto cmd = s_commands.find({name});
        if (cmd == s_commands.end())
            s_commands.emplace(name, action, desc);
        else if (cmd->action == nullptr)
        {
            s_flags.erase(cmd);
            s_flags.emplace(name, action, desc);
        }
        else if (action == nullptr)
            throw CliError("Command overlaps pre-existing defined command", name);
        else throw CliError("Could not create or assign command: " + name, name);
    });
}


void CommandLineInterface::MakeOption(const std::string name, Manip_f action, std::string desc)
{
    s_threads.Launch([name, action, desc]{
        if (s_commands.find({name}) != s_commands.end())
            throw CliError("Option overlaps command: " + name, name);
        if (s_flags.count({name}) != 0)
            throw CliError("Option overlaps flag: " + name, name);

        auto iter = s_options.find({name});
        if (iter == s_options.end())
            s_options.emplace(name, action, desc);
        else if (iter->action == nullptr) // why are you calling this twice?
        {
            s_flags.erase(iter);
            s_flags.emplace(name, action, desc);
        }
        else if (action == nullptr)
            throw CliError("Option overlaps pre-existing defined option", name);
        else throw CliError("Could not create or assign option: " + name, name);
    });
}


void CommandLineInterface::DefaultHelp(const CommandLineInterface*)
{
    using std::cout, std::setw, std::left;
    cout << "Commands: [" << s_commands.size() << "]";
    for (auto cmd : s_commands)
    {
        cout << "\n\t";
        if (!cmd.desc.empty())
            cout << left << setw(12)
                << cmd.name << " | " << cmd.desc;
        else cout << cmd.name;
    }
    cout << "\nOptions:  [" << s_options.size() << "]";
    for (auto opt : s_options)
    {
        cout << "\n\t";
        if (!opt.desc.empty())
            cout << left << setw(12)
                << opt.name << " | " << opt.desc;
        else cout << opt.name;
    }
    cout << "\nFlags:    [" << s_flags.size() << "]";
    for (auto flg : s_flags)
    {
        cout << "\n\t";
        if (!flg.desc.empty())
            cout << left << setw(12)
                << flg.name << " | " << flg.desc;
        else cout << flg.name;
    }
    cout << std::endl;
}


void CommandLineInterface::MakeOption(StringIList aliases, Manip_f action, std::string desc)
{
    for (std::string alias : aliases)
        MakeOption(alias, action, desc);
}


void CommandLineInterface::MakeFlag(const std::string name, Manip_f action, std::string desc)
{
    s_threads.Launch([name, action, desc]{
        if (s_commands.find({name}) != s_commands.end())
            throw CliError("Flag overlaps command: " + name, name);
        if (s_options.count({name}) != 0)
            throw CliError("Flag overlaps option: " + name, name);

        auto iter = s_flags.find({name});
        if (name == "--help")
        {
            s_flags.erase(iter);
            s_flags.emplace(name, action, desc);
        }
        else if (iter == s_flags.end())
            s_flags.emplace(name, action, desc);
        else if (iter->action == nullptr) // why are you calling this twice?
        {
            s_flags.erase(iter);
            s_flags.emplace(name, action, desc);
        }
        else if (action == nullptr)
            throw CliError("Flag overlaps pre-existing defined flag", name);
        else throw CliError("Could not create or assign flag: " + name, name);
    });
}


void CommandLineInterface::MakeFlag(const StringIList aliases, Manip_f action, std::string desc)
{
    for (std::string alias : aliases)
        MakeFlag(aliases, action, desc);
}


void CommandLineInterface::Shrink()
{
    s_threads.JoinAll();
    s_commands.clear();
    s_options.clear();
    s_flags.clear();
}


std::optional<CommandLineInterface::StringVec>
    CommandLineInterface::Option(const std::string opt_name) const
{
    auto opt = options.find({opt_name});
    if (opt != options.end() && !opt->second.empty())
        return opt->second;
    else return { };
}


std::optional<CommandLineInterface::StringVec>
    CommandLineInterface::Option(StringIList opts) const
{
    StringVec opt_args;
    for (std::string opt : opts)
    {
        auto iter = options.find(opt);
        if (iter != options.end())
            opt_args.insert(
                opt_args.end(),
                iter->second.begin(),
                iter->second.end()
            );
    }
    if (opt_args.empty())
        return { };
    else return opt_args;
}


bool CommandLineInterface::FlagSet(const std::string flg) const
{
    if (flags.count(flg) != 0)
        return true;
    else return false;
}


bool CommandLineInterface::AnyFlagSet(StringIList flgs) const
{
    for (std::string flg : flgs)
        if (FlagSet(flg))
            return true;
    return false;
}


bool CommandLineInterface::AllFlagsSet(StringIList flgs) const
{
    for (std::string flg : flgs)
        if (!FlagSet(flg))
            return false;
    return true;
}


/***** PRIVATES *****/


void CommandLineInterface::set_command(const std::string name)
{
    std::scoped_lock lk(cmds_mtx);
    if (s_commands.count(name) != 0)
        command = name;
    else throw CliError("Command not recognized: " + name, name);
}


void CommandLineInterface::add_arg(const std::string arg)
{
    std::scoped_lock lk(args_mtx);
    arguments.emplace_back(arg);
}


void CommandLineInterface::set_option(const std::string opt, const StringVec args)
{
    std::scoped_lock lk(opts_mtx);
    if (s_options.count(opt) == 0)
        throw CliError("Option not recognized: " + opt, opt);
    options.emplace(opt, args);
}


void CommandLineInterface::add_flag(const std::string flg)
{
    std::scoped_lock lk(flgs_mtx);
    if (s_flags.count(flg) == 0)
        throw CliError("Flag not recognized: " + flg, flg);
    flags.insert(flg);
}
