#include "Parser.h"
#include "Dungeon.h"
#include <climits>

std::vector<std::string> parse_by_spaces(const std::string& str){
    std::vector<std::string> res;
    std::istringstream iss(str);
    std::string part;
    while (iss >> part){
        res.push_back(part);
    }
    return res;
}

bool parse_int_strict(const std::string& str, int& value) {
    if (str.empty()) return false;

    size_t pos = 0;

    try {
        long long temp = std::stoll(str, &pos);

        if (pos != str.size()) {
            return false;
        }

        if (temp < INT_MIN || temp > INT_MAX) {
            return false;
        }

        value = static_cast<int>(temp);
        return true;
    } catch (...) {
        return false;
    }
}

bool Parser::parse_line(const std::string& line, Room& room, std::string& error){
    std::vector<std::string> parts = parse_by_spaces(line);
    
    if (parts.size() != 6 && parts.size() != 2){
        error = line;
        return false;
    }

    if (!parse_int_strict(parts[0], room.id)) {
        error = line;
        return false;
    }

    if (parts.size() == 2 && room.id != 0) {
        error = line;
        return false;
    }

    room.adjacent.clear();

    std::string str = parts[1];
    std::string cur;

    for (int i = 0; i <= str.size(); i++){
        if (i == str.size() || str[i] == ','){
            if (cur.empty()) {
                error = line;
                return false;
            }

            int adj;
            if (!parse_int_strict(cur, adj)) {
                error = line;
                return false;
            }

            room.adjacent.push_back(adj);
            cur.clear();
        } else {
        cur.push_back(str[i]);
        }
    }

    if (parts.size() == 2) {
        room.resources = {0, 0, 0, 0};
        return true;
    }

    for (int i = 0; i < 4; i++) {
        int amount;

        if (!parse_int_strict(parts[2 + i], amount)) {
            error = line;
            return false;
        }

        if (amount < 0 || amount > 255) {
            error = line;
            return false;
        }

        room.resources[i] = amount;
    }

    return true;
}

bool Parser::graph_validator(const Input_data& data){
    int n = data.N;
    std::vector<bool> seen(n + 1, false);

    for (const auto& room : data.rooms){
        if (seen[room.id] || room.id < 0 || room.id > n){
            return false;
        }
        seen[room.id] = true;
    }

    for (int i = 0; i <= n; i++){
        if (!seen[i]){
            return false;
        }
    }

    std::vector<std::unordered_set<int>> adj_set (n + 1);
    for (const auto& room : data.rooms) {
        for (int adj : room.adjacent){
            adj_set[room.id].insert(adj);
        }
    }

    return true;
}

bool Parser::parse_input(const std::string& filename, Input_data& data, std::string& error){
    std::ifstream file (filename);
    if (!file){
        return false;
    }

    std::string line;
    
    if (!std::getline(file, line)){
        return false;
    }

    if (!parse_int_strict(line, data.N)) {
    error = line;
    return false;
    }

    if (data.N < 1 || data.N > 255) {
    error = line;
    return false;
    }

    for (int i = 0; i <= data.N; i++){
        if (!std::getline(file, line)){
            return false;
        }

        Room room;
        if (!parse_line(line, room, error)){
            return false;
        }
        data.rooms.push_back(room);
    }

    if (!std::getline(file, line)){
        return false;
    }

    std::vector<std::string> last_str = parse_by_spaces(line);
    if (last_str.size() != 2){
        error = line;
        return false;
    }

    if (!parse_int_strict(last_str[0], data.M)) {
    error = line;
    return false;
    }

    if (data.M < 2 || data.M > 255) {
    error = line;
    return false;
    }

    data.target = resource_index(last_str[1]);
    if (data.target < 0){
        error = line;
        return false;
    }

    if (!graph_validator(data)){
        return false;
    }

    return true;
}