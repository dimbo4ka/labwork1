#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "LogParser.hpp"

struct WindowParams {
    std::size_t max_request_count;
    std::size_t start;
    std::size_t end;
};

class LogAnalyzer {
public:
    LogAnalyzer(const std::string& filename);

    void Analyze();
    auto GetTop5xxRequests(std::size_t n) const 
            -> std::vector<std::pair<std::string, std::size_t>>;
    bool WriteErrors(const std::string& filename, bool need_print = false) const;
    WindowParams CalculateWindowWithMaxRequestCount(std::size_t window_size);

private:
    std::vector<LogEntry> entries_;
    std::ifstream log_file;
    LogParser log_parser;
};
