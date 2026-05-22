#pragma once
#include <sstream>
#include <unordered_set>
#include <fstream>
#include <iostream>

class Dungeon_state;
struct Input_data;
struct Room;

class Parser {
    public:
    bool parse_input(const std::string& filename, Input_data& data, std::string& error);
    private:
    bool parse_line(const std::string& line, Room& room, std::string& error);
    bool graph_validator(const Input_data& data);
};