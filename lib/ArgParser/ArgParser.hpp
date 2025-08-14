#pragma once

#include <cstddef>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "Handlers.hpp"

struct ParserConfig {
    std::string output_file;
    std::size_t stats;
    std::size_t window_size;
    std::size_t from;
    std::size_t to;
    bool must_print;
};

class ArgParser {
public:
    ArgParser();
    bool Parse(int argc, char** argv);
    ParserConfig GetConfig() const;

private:
    std::vector<std::unique_ptr<Handler>> handlers_;
    std::string output_file_;
    std::size_t stats_ = 10;
    std::size_t window_size_ = 0;
    std::size_t from_ = 0;
    std::size_t to_ = std::numeric_limits<std::size_t>::max();
    bool must_print_ = false;

    template<typename T>
    void AddHandler(auto& variable, std::vector<std::string> names);
};

template<typename T>
void ArgParser::AddHandler(auto& variable, std::vector<std::string> names) {
    handlers_.emplace_back(std::make_unique<T>(variable, std::move(names)));
}