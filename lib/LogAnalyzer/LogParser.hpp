#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

struct LogEntry {
    std::string ip;
    std::string request;
    std::size_t timestamp;
    std::size_t status;
};

class LogParser {
public:
    std::optional<LogEntry> ParseLogLine(std::string_view line);

private:
    auto GetNextToken(std::string_view line, std::size_t& pos, char delimiter)
            -> std::string_view;
    std::size_t ParseTimeStamp(std::string_view date);
    std::size_t ParseStatus(std::string_view status);
};  