#include "LogParser.hpp"

#include <chrono>
#include <optional>
#include <sstream>

std::optional<LogEntry> LogParser::ParseLogLine(std::string_view line) {
    LogEntry entry;
    std::size_t pos = 0;
    std::string_view token;

    token = GetNextToken(line, pos, ' ');
    if (token.empty())
        return std::nullopt;
    entry.ip = token;
    
    GetNextToken(line, pos, '[');
    
    token = GetNextToken(line, pos, ']');
    if (token.empty())
        return std::nullopt;
    entry.timestamp = ParseTimeStamp(token);
        
    GetNextToken(line, pos, '"');
    token = GetNextToken(line, pos, '"');
    if (token.empty())
        return std::nullopt;
    entry.request = token;
    
    token = GetNextToken(line, pos, ' ');
    if (token.empty())
        return std::nullopt;
    entry.status = ParseStatus(token);
    return std::move(entry);
}

std::string_view LogParser::GetNextToken(std::string_view line, 
                                         std::size_t& pos, char delimiter) {
    std::size_t start = pos;
    while (pos < line.size() && line[pos] != delimiter) {
        ++pos;
    }
    std::string_view token = line.substr(start, pos - start);
    if (pos < line.size())
        ++pos;
    return token;
}

std::size_t LogParser::ParseStatus(std::string_view status) {
    return std::stoul(std::string(status));
}

std::size_t LogParser::ParseTimeStamp(std::string_view date) {
    std::istringstream iss(std::string(date));
    std::chrono::sys_time<std::chrono::seconds> timestamp;
    iss >> std::chrono::parse("%d/%b/%Y:%H:%M:%S %z", timestamp);
    return static_cast<std::size_t>((timestamp.time_since_epoch()).count());
}
