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
    void Analyze();
    bool OpenFile(const std::string& filename);

    bool WriteErrors(std::ostream&, std::size_t n) const;
    bool WriteErrors(const std::string& filename, std::size_t n) const;
    WindowParams CalculateWindowWithMaxRequestCount(
            std::size_t window_size, std::size_t from, std::size_t to);
    
private:
    std::vector<LogEntry> entries_;
    std::ifstream log_file;
    LogParser log_parser;

    auto GetTop5xxRequests(std::size_t n) const 
            -> std::vector<std::pair<std::string, std::size_t>>;
};
