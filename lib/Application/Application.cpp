#include "Application.hpp"

#include <iostream>

bool Application::Run(int argc, char** argv) {
    if (!parser.Parse(argc, argv))
        return false;
        
    ParserConfig config = std::move(parser.GetConfig());
    analyzer.OpenFile("access.log");
    analyzer.Analyze();

    if (!config.output_file.empty()) {
        if (!analyzer.WriteErrors(config.output_file, config.stats))
            return false;
    }
    if (config.must_print && !analyzer.WriteErrors(std::cout, config.stats))
        return false;
    
    WindowParams window_params = analyzer.CalculateWindowWithMaxRequestCount(
            config.window_size, config.from, config.to
        );

    if (config.window_size == 0)
        PrintMaxWindow(window_params);
    return true;    
}

void Application::PrintMaxWindow(WindowParams params) const {
    std::cout << "max window:\n"
              << "start = " << params.start << " | end = " << params.end << '\n'
              << "request count = " << params.max_request_count << '\n';
}
