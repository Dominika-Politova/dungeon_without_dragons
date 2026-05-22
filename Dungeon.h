#pragma once
#include <string>
#include <vector>
#include <array>

enum class Resource_type { iron = 0, gold = 1, gems = 2, exp = 3, invalid = 4};

inline int resource_index(const std::string& str) {
    if (str == "iron") return 0;
    if (str == "gold") return 1;
    if (str == "gems") return 2;
    if (str == "exp") return 3;
    return -1;
}

inline std::string resource_name(int idx) {
    static const std::vector<std::string> names = {"iron", "gold", "gems", "exp"};
    if (idx < 0 || idx >= 4) return "";
    return names[idx];
}

inline int base_cost(int idx) {
    static const int v[4] = {7, 11, 23, 1};
    return v[idx];
}

struct Room {
    int id = -1;
    std::vector<int> adjacent;
    std::array<int,4> resources {0,0,0,0};
};

struct Input_data {
    int N = -1;
    int M = -1;
    int target = -1;
    std::vector<Room> rooms;
};

struct Dungeon_state {
    int N = 0;
    int target = -1;
    int cur_room = 0;
    int cur_food = 0;
    bool dead = false;
    bool return_phase = false;

    std::vector<Room> rooms;
    std::vector<std::array<int, 4>> remaining_resources;
    std::vector<bool> visited;
    std::array<int, 4> collected {0,0,0,0};
    std::vector<std::array<bool, 4>> if_collected;
    std::vector<std::vector<int>> graph;

    int resource_value(int type) const {
        int val = base_cost(type);
        if (type == target){
            val *= 2;
        }
        return val;
    }

    int total_val() const {
        int s = 0;
        for (int i = 0; i < 4; i++){
            s += collected[i] * resource_value(i);
        }
        return s;
    }
};