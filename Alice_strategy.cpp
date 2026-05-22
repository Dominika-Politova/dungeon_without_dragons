#include "Alice_strategy.h"
#include "Dungeon.h"
#include <iostream>

int Alice_strategy::choose_resources(const Dungeon_state& state, int room_id) {
    int best_id = -1;
    int best_val = -1;

    for (int i = 0; i < 4; i++){
        if (state.remaining_resources[room_id][i] <= 0) continue;
        int val = state.resource_value(i);
        if (val > best_val){
            best_id = i;
            best_val = val;
        }
    }
    return best_id;
}

bool Alice_strategy::switch_to_return_phase(const Dungeon_state& state, const Input_data& data) {
    return state.cur_food <= data.M / 2;
}

std::vector<int> Alice_strategy::shortest_path_to_unvisited(const Dungeon_state& state, int start) const{
    std::vector<int> dist(state.N + 1, INT_MAX);
    std::vector<int> ancestor(state.N + 1, -1);
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    int best_target = -1;
    int best_dist = -1;
    while(!q.empty()){
        int room_num = q.front();
        q.pop();
        if (room_num != start && !state.visited[room_num]){
            if (dist[room_num] < best_dist || (dist[room_num] == best_dist && room_num < best_target)) {
                best_dist = dist[room_num];
                best_target = room_num;
            }
        }

        for (int adj : state.graph[room_num]){
            if (dist[adj] != INT_MAX) continue;
            dist[adj] = dist[room_num] + 1;
            ancestor[adj] = room_num;
            q.push(adj);
        }
    }

    std::vector<int> path;
    if (best_target == -1) return path;

    int room_num = best_target;
    while (room_num != -1){
        path.push_back(room_num);
        room_num = ancestor[room_num];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> Alice_strategy::shortest_path_back(const Dungeon_state& state, int start, int end) const {
    std::vector<int> dist(state.N + 1, INT_MAX);
    std::vector<int> ancestor(state.N + 1, -1);
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()){
        int room_num = q.front();
        q.pop();
        for (int adj : state.graph[room_num]){
        if (!state.visited[adj]) continue;
        if (dist[adj] != INT_MAX) continue;
        dist[adj] = dist[room_num] + 1;
        ancestor[adj] = room_num;
        q.push(adj);
        }
    }

    std::vector<int> path;
    if (dist[end] == INT_MAX) return path;

    int room_num = end;
    while (room_num != -1){
        path.push_back(room_num);
        room_num = ancestor[room_num];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int Alice_strategy::choose_next_room(const Dungeon_state& state) {
    if (!state.return_phase) {
        for (int adj : state.graph[state.cur_room]){
            if (!state.visited[adj]) return adj;
        }
        std::vector<int> path = shortest_path_to_unvisited(state, state.cur_room);
        if (path.size() >= 2) return path[1];
        return -1;
    } else {
        std::vector<int> path = shortest_path_back(state, state.cur_room, 0);
        
        if (path.size() >= 2) return path[1];
        return -1;
    }
}

std::vector<int> Alice_strategy::get_return_path(const Dungeon_state& state, int start, int end) const {
    return shortest_path_back(state, start, end);
}