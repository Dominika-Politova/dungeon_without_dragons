#pragma once
#include <fstream>
#include <array>

class Output{
    private:
    std::ofstream file;
    public:
    Output(const std::string& filename);
    void write_line(const std::string& str);
    void write_result(const std::array<int, 4>& res, int total);
};