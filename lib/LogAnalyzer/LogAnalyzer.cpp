#include "LogAnalyzer.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

LogAnalyzer::LogAnalyzer(const std::string& filename) 
        : log_file(filename, std::ios::app) {
    if (!log_file.is_open())
        throw std::runtime_error("Could not open log file: " + filename);
}

void LogAnalyzer::Analyze() {
    std::string line;
    while (std::getline(log_file, line)) {
        std::optional<LogEntry> entry = log_parser.ParseLogLine(line);
        if (!entry.has_value())
            continue;
        entries_.push_back(std::move(*entry));
    }
}

auto LogAnalyzer::GetTop5xxRequests(std::size_t n) const
        -> std::vector<std::pair<std::string, std::size_t>> {
    std::unordered_map<std::string, std::size_t> request_count;
    for (const auto& entry : entries_) {
        ++request_count[entry.request];
    }
    std::vector<std::pair<std::string, std::size_t>> 
                top_requests(request_count.begin(), request_count.end());
            
    std::sort(top_requests.begin(), top_requests.end(), 
                [](const auto& a, const auto& b) {return a.second > b.second;}
            );

    if (top_requests.size() > n)
        top_requests.resize(n);
    return top_requests;
}

bool LogAnalyzer::WriteErrors(const std::string& filename, bool need_print) const {
    std::ofstream error_file(filename);
    if (!error_file.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return false;
    }
    
    for (std::size_t i = 1; const auto& entry : entries_) {
        if (entry.status >= 500 && entry.status <= 599) {
            error_file << i << ". "
                       << entry.ip << " " << entry.request << " "
                       << entry.timestamp << " " << entry.status << "\n";
            if (need_print) {
                std::cout << i << ". "
                          << entry.ip << " " << entry.request << " "
                          << entry.timestamp << " " << entry.status << std::endl;
            }
            ++i;
        }
    }

    return true;
}

WindowParams LogAnalyzer::CalculateWindowWithMaxRequestCount(std::size_t window_size) {
    std::sort(entries_.begin(), entries_.end(),
            [](const LogEntry& a, const LogEntry& b) {
                return a.timestamp < b.timestamp;
            });

    WindowParams window_params;
    std::size_t l = 0;
    for (std::size_t r = 0; r < entries_.size(); ++r) {
        while (entries_[r].timestamp - entries_[l].timestamp > window_size) {
            ++l;
        }
        std::size_t request_count = r - l + 1;
        if (request_count > window_params.max_request_count) {
            window_params.max_request_count = request_count;
            window_params.start = l;
            window_params.end = std::max(r, l + window_size - 1);
        }
    }
    return window_params;
}
