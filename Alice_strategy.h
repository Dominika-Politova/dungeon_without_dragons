#pragma once
#include "Bot_strategy.h"
#include <queue>
#include <algorithm>
#include <vector>

class Alice_strategy : public Strategy {
    private:
    std::vector<int> shortest_path_to_unvisited(const Dungeon_state& state, int start) const;
    std::vector<int> shortest_path_back(const Dungeon_state& state, int start, int end) const;
    public:
    int choose_next_room(const Dungeon_state& state) override;
    int choose_resources(const Dungeon_state& state,
    int room_id) override;
    bool switch_to_return_phase(const Dungeon_state& state, const Input_data& data) override;
    std::vector<int> get_return_path(const Dungeon_state& state, int start, int end) const override;
};