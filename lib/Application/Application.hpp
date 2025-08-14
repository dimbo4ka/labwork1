#pragma once

#include "lib/ArgParser/ArgParser.hpp"
#include "lib/LogAnalyzer/LogAnalyzer.hpp"

class Application {
public:
    bool Run(int argc, char** argv);

private:
    ArgParser parser;
    LogAnalyzer analyzer;

    void PrintMaxWindow(WindowParams params) const;
};