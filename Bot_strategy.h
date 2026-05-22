#pragma once
#include <vector>

class Dungeon_state;
struct Input_data;

class Strategy {
    public:
    virtual ~Strategy() = default;

    virtual void start_game(Dungeon_state& state) {}

    virtual int choose_next_room(const Dungeon_state& state) = 0;
    virtual int choose_resources(const Dungeon_state& state, int room_id) = 0;
    virtual bool switch_to_return_phase(const Dungeon_state& state, const Input_data& data) = 0;
    virtual std::vector<int> get_return_path (const Dungeon_state& state, int start, int goal) const = 0;
};