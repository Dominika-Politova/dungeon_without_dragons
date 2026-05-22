#include "Output.h"

Output::Output (const std::string& filename) : file (filename) {}

void Output::write_line(const std::string& str) {
    file << str << std::endl;
}

void Output::write_result(const std::array<int, 4>& res, int total){
    file << "result " << res[0] << ' ' << res[1] << ' ' << res[2] << ' ' <<
    res[3] << ' ' << total << std::endl;
}