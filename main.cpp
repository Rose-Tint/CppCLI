#include <iostream>
#include <array>
#include <string>
#include <random>

#include "CLI.hpp"
#include "CliError.hpp"
#include "ColorCode.hpp"


using CLI = cli::CommandLineInterface;
using CC = cli::ColorCode;


void compliment(const CLI* cli);
void help(const CLI* cli);
void joke(const CLI* cli);
void pickup(const CLI* cli);
void bday(const CLI*);


int main(int argc, const char** argv)
{
    cli::CliError::SetCommandLine({ argv, argv + argc });

    CLI::MakeCommand("bday", bday, "Tell me your birthday");
    CLI::MakeCommand("help", help, "Display help information");
    CLI::MakeCommand("joke", joke, "Tell a joke");
    CLI::MakeCommand("compliment", compliment, "Compliment me");
    CLI::MakeCommand("pickup", pickup, "Give me a pick-up line");

    CLI::MakeOption("--name");
    CLI::MakeFlag("--help", help);

    const CLI cli(argc, argv);

    CLI::Shrink();
}


/**
* random number in the range [min -> max)
*/
template < class Int >
Int randn(Int min, Int max)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<Int> dist(min, max - 1);
    return dist(engine);
}


template < class It >
It randchoice(It begin, It end)
{
    return std::next(begin, randn({'\0'}, std::distance(begin, end)));
}


template < class Ctr >
auto randchoice(Ctr ctr) -> decltype(std::begin(ctr))
{
    return randchoice(std::begin(ctr), std::end(ctr));
}


void compliment(const CLI* cli)
{
    const std::vector<std::string> comps {
        "You are GLOWING today!",
        "Are you HTML + CSS? 'Cuz you've got style!",
        "Are you a major gui widget toolkit? 'Cuz you are qt!",
    };
    auto args = cli->Args();
    if (args.empty())
        std::cout << CC::BrightRed << *randchoice(comps) << std::endl;
    else
    {
        std::size_t idx = std::stoull(cli->Args().at(0));
        if (idx >= comps.size())
            std::cout << CC::BrightRed << *randchoice(comps) << std::endl;
        else std::cout << CC::BrightRed << comps.at(idx) << std::endl;
    }
}


void help(const CLI* cli)
{
    std::string name;
    auto name_opt = cli->Option("--name");
    if (name_opt.has_value() && !name_opt->empty())
        name = name_opt.value().at(0) + "! ";
    std::cout << CC::BrightRed << "Hello, " << name
        << "My name is " << CC::Red << "Rose" << CC::BrightRed
        << ". What can I do for you?" << std::endl;
    cli->DefaultHelp();
}


void joke(const CLI* cli)
{
    std::vector<std::string> jokes {
        "How does an object-oriented programmer get rich?\nThey inherit",
        "How does a non-binary ninja defeat their enemies?\nthey/them",
        "How do you scare a non-binary person?\n01000010 01101111 01101111 01010001",
        "git push --all --force --up-stream",
        "How many SWEs does it take to change a lightbulb?\nsegmentation fault (core dumped)"
    };
    
    auto args = cli->Args();
    if (args.empty())
        std::cout << CC::BrightRed << *randchoice(jokes) << std::endl;
    else
    {
        std::size_t idx = std::stoull(cli->Args().at(0));
        if (idx >= jokes.size())
            std::cout << CC::BrightRed << *randchoice(jokes) << std::endl;
        else std::cout << CC::BrightRed << jokes.at(idx) << std::endl;
    }
}


void pickup(const CLI* cli)
{
    std::vector<std::string> lines
    {
        "// TODO: you.",
        "You had me at \"Hello, World!\"",
        "Are you a friend class? 'Cuz I'd let you touch my privates.",
        "git commit -m \"me & you forever\"",
        "Lets lock a mutex"
    };
    auto args = cli->Args();
    if (args.empty())
        std::cout << CC::BrightRed << *randchoice(lines) << std::endl;
    else
    {
        std::size_t idx = std::stoull(cli->Args().at(0));
        if (idx >= lines.size())
            std::cout << CC::BrightRed << *randchoice(lines) << std::endl;
        else std::cout << CC::BrightRed << lines.at(idx) << std::endl;
    }
}


void bday(const CLI*)
{
    std::cout << CC::BrightRed <<
        "Thanks for asking! My birthday is YY/MM/DD -> 03/09/03"
        << std::endl;
}
