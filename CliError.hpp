#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <exception>


namespace cli
{
    class CliError : public std::exception
    {
      public:
        static void SetCommandLine(std::vector<std::string> args);

        CliError(std::string msg, std::size_t idx);
        CliError(std::string msg, std::string arg);

        virtual const char* what() const noexcept override
            { return message.c_str(); }

      protected:
        static inline std::string cmdLine = "";

        std::string message;

        void set_message();
    };
}
