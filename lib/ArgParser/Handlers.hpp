#pragma once

#include <string>
#include <string_view>
#include <vector>

class Handler {
public:
    virtual bool CanHandle(std::string_view arg) = 0;
    virtual bool Handle(int& i, int argc, char** argv) = 0;
    virtual ~Handler() = default;

protected:
    bool HasNext(int i, int argc);
};

class StringHandler : public Handler {
public:
    StringHandler(std::string& str, std::vector<std::string> names);
    bool CanHandle(std::string_view arg) override;
    bool Handle(int& i, int argc, char** argv) override;

private:
    std::vector<std::string> names_;
    std::string& str_;
};

class FlagHandler : public Handler {
public:
    FlagHandler(bool& flag, std::vector<std::string> names);
    bool CanHandle(std::string_view arg) override;
    bool Handle(int& i, int argc, char** argv) override;

private:
    std::vector<std::string> names_;
    bool& flag_;
};

class NumericHandler : public Handler {
public:
    NumericHandler(std::size_t& number, std::vector<std::string> names);
    bool CanHandle(std::string_view arg) override;
    bool Handle(int& i, int argc, char** argv) override;
    bool IsNamed(const std::string& arg) const;

private:
    std::size_t& number_;
    std::vector<std::string> names_;

    bool ConvertToNumber(const char* str, std::size_t& number) const;
};
