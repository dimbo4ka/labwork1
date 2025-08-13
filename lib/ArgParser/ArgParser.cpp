#include "ArgParser.hpp"

#include <iostream>

ArgParser::ArgParser() {
    AddHandler<StringHandler>(output_file_, {"-o", "--output"});
    AddHandler<FlagHandler>(print_, {"-p", "--print"});
    AddHandler<NumericHandler>(stats_, {"-s", "--stats"});
    AddHandler<NumericHandler>(window_size_, {"-w", "--window"});
    AddHandler<NumericHandler>(from_, {"-f", "--from"});
    AddHandler<NumericHandler>(to_, {"-t", "--to"});
}

bool ArgParser::Parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        bool handled = false;
        for (const auto& handler : handlers_) {
            if (handler->CanHandle(argv[i])) {
                if (!handler->Handle(i, argc, argv)) {
                    std::cerr << "Error handling argument: " << argv[i] << std::endl;
                    return false;
                }
                handled = true;
                break;
            }
        }
        if (!handled) {
            std::cerr << "Can not handle argument: " << argv[i] << std::endl;
            return false;
        }
    }
    return true;
}