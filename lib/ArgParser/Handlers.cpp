#include "Handlers.hpp"

#include <algorithm>
#include <charconv>
#include <cstring>
#include <iostream>

bool Handler::HasNext(int i, int argc) {
    return i + 1 < argc;
}

StringHandler::StringHandler(std::string& str, std::vector<std::string> names)
    : str_(str), names_(std::move(names)) {}

bool StringHandler::CanHandle(std::string_view arg) {
    return std::find(names_.begin(), names_.end(), arg) != names_.end()
        || std::any_of(names_.begin(), names_.end(), [&](const std::string& name) {
            return arg.starts_with(name + "=");
        });
}

bool StringHandler::Handle(int& i, int argc, char** argv) {
    std::string arg = argv[i];
    if (std::find(names_.begin(), names_.end(), arg) != names_.end()) {
        if (HasNext(i, argc)) {
            str_ = argv[++i];
            return true;
        }
        std::cerr << "No string value specified after " << arg << std::endl;
        return false;
    }
    std::size_t equal_pos = arg.find('=');
    str_ = arg.substr(equal_pos + 1);
    return true;
}

FlagHandler::FlagHandler(bool& flag, std::vector<std::string> names)
    : flag_(flag), names_(std::move(names)) {}

bool FlagHandler::CanHandle(std::string_view arg) {
    return std::find(names_.begin(), names_.end(), arg) != names_.end();
}

bool FlagHandler::Handle(int& i, int argc, char** argv) {
    flag_ = true;
    return true;
}

NumericHandler::NumericHandler(std::size_t& number, std::vector<std::string> names)
    : number_(number), names_(std::move(names)) {}

bool NumericHandler::ConvertToNumber(const char* str, std::size_t& number) const {
    auto [_, ec] = std::from_chars(str, str + strlen(str), number);
    return ec == std::errc{};
}

bool NumericHandler::CanHandle(std::string_view arg) {
    return std::find(names_.begin(), names_.end(), arg) != names_.end()
        || std::any_of(names_.begin(), names_.end(), [&](const std::string& name) {
            return arg.starts_with(name + "=");
        });
}

bool NumericHandler::IsNamed(const std::string& arg) const {
    return arg.find('=') != std::string::npos;
}

bool NumericHandler::Handle(int& i, int argc, char** argv) {
    std::string arg = argv[i];
    if (!IsNamed(arg)) {
        if (HasNext(i, argc)) {
            if (ConvertToNumber(argv[++i], number_))
                return true;
            std::cerr << "Invalid numeric value specified after" << arg << std::endl;
            return false;
        }
        std::cerr << "No numeric value specified" << std::endl;
        return false;
    }
    std::size_t equal_pos = arg.find('=');
    if (ConvertToNumber(arg.c_str() + equal_pos + 1, number_))
        return true;
    std::cerr << "Invalid numeric value specified for " << arg << std::endl;
    return false;
}
