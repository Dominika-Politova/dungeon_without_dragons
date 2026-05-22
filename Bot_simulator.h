#pragma once
#include "Dungeon.h"
#include "Output.h"
#include "Bot_strategy.h"
#include <memory>
#include <algorithm>
#include <iostream>

class Simulator {
    private:
    const Input_data& data;
    Output& output;
    std::unique_ptr<Strategy> strategy;
    Dungeon_state state;

    void init_state();
    void build_graph();
    void print_state(int room_id) const;

    void move(int room_id);
    void collect(int room_id);
    void enter_room(int room_id);
    void return_back();
    void exploration();
    void write_result();

    public:
    Simulator(const Input_data& data, Output& output, std::unique_ptr<Strategy> strategy);
    void simulate();
};