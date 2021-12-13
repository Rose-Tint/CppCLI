#pragma once

#include <vector>
#include <initializer_list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <optional>
#include <thread>
#include <mutex>

#include "CliArg.hpp"
#include "ThreadPool.hpp"


// IDEA: add an optional description for each thing for the help msg
//       limitation: thats a lot of memory
namespace cli
{
    class CommandLineInterface final
    {
      public:
        using Manip_f = void (*)(const CommandLineInterface*);
      private:
        using StringIList = std::initializer_list<std::string>;
        using StringVec = std::vector<std::string>;
        using CliSet = std::unordered_set<CliArg>;
        using CliSetIter = CliSet::iterator;
        using FlagResults = std::unordered_set<std::string>;
        using OptionResults = std::unordered_map<std::string, StringVec>;

      public:
        static void DefaultHelp(const CommandLineInterface* cli);

        static void MakeCommand(std::string name, Manip_f = nullptr, std::string desc = "");

        static void MakeOption(std::string name, Manip_f = nullptr, std::string desc = "");
        static void MakeOption(StringIList names, Manip_f = nullptr, std::string desc = "");

        static void MakeFlag(std::string name, Manip_f = nullptr, std::string desc = "");
        static void MakeFlag(StringIList name, Manip_f = nullptr, std::string desc = "");

        static void Shrink();

        CommandLineInterface();
        CommandLineInterface(int argc, const char** argv);

        std::string Command() const { return command; }
        StringVec Args() const { return arguments; }
        OptionResults AllOptions() const { return options; }
        FlagResults Flags() const { return flags; }

        void DefaultHelp() const { DefaultHelp(this); }
        std::optional<StringVec> Option(std::string opt) const;
        std::optional<StringVec> Option(StringIList opts) const;
        bool FlagSet(std::string flg) const;
        bool AnyFlagSet(StringIList flgs) const;
        bool AllFlagsSet(StringIList flgs) const;

      private:
        static inline ThreadPool s_threads = { };
        static inline CliSet s_commands = { };
        static inline CliSet s_options = { };
        static inline CliSet s_flags = { { "--help", &DefaultHelp } };

        // mutices for writing
        mutable std::recursive_mutex cmds_mtx, opts_mtx, flgs_mtx, args_mtx;
        const StringVec args;
        std::string command;
        StringVec arguments; // arguments after possible command
        OptionResults options;
        FlagResults flags;
        ThreadPool threads;

        void set_command(std::string cmd);
        void add_arg(std::string arg);
        void set_option(std::string opt, StringVec args);
        void add_flag(std::string flg);
    };
}
