#include "lib/ArgParser/ArgParser.hpp"

#include <iostream>

int main(int argc, char** argv) {
    ArgParser parser;
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Failed to parse arguments." << std::endl;
        return 1;
    }
    return 0;
}