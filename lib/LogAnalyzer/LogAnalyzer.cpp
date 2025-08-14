#include "LogAnalyzer.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

void LogAnalyzer::Analyze() {
    std::string line;
    while (std::getline(log_file, line)) {
        std::optional<LogEntry> entry = log_parser.ParseLogLine(line);
        if (!entry.has_value())
            continue;
        entries_.push_back(std::move(*entry));
    }
}

bool LogAnalyzer::OpenFile(const std::string& filename) {
    log_file.open(filename);
    if (!log_file.is_open()) {
        std::cerr << "Could not open logfile: " << filename << std::endl;
        return false;
    }
    return true;
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

bool LogAnalyzer::WriteErrors(std::ostream& out, std::size_t n) const {
    for (auto [request, frequency] : GetTop5xxRequests(n)) {
        std::cout << request << " | frequency = " << frequency << std::endl;
    }
    return true;
}

bool LogAnalyzer::WriteErrors(const std::string& filename, std::size_t n) const {
    std::ofstream error_file(filename);
    if (!error_file.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return false;
    }
    return WriteErrors(error_file, n);
}

WindowParams LogAnalyzer::CalculateWindowWithMaxRequestCount(
        std::size_t window_size, std::size_t from, std::size_t to) {
    std::sort(entries_.begin(), entries_.end(),
            [](const LogEntry& a, const LogEntry& b) {
                return a.timestamp < b.timestamp;
            });

    WindowParams window_params;
    std::size_t l = 0;
    for (std::size_t r = 0; r < entries_.size(); ++r) {
        if (entries_[r].timestamp > to)
            break;
        if (entries_[l].timestamp < from) {
            ++l;
            continue;
        }
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
