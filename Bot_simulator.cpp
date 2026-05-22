#include "Bot_simulator.h"

Simulator::Simulator(const Input_data& data, Output& output, 
    std::unique_ptr<Strategy> strategy) : data(data), output(output),
    strategy(std::move(strategy)) {}

    void Simulator::build_graph() {
    state.graph.assign(state.N + 1, {});

    for (const auto& room : state.rooms) {
        for (int adj : room.adjacent) {
            state.graph[room.id].push_back(adj);
            state.graph[adj].push_back(room.id); 
        }
    }
    
    for (int i = 0; i <= state.N; i++) {
        std::sort(state.graph[i].begin(), state.graph[i].end());
        state.graph[i].erase(
            std::unique(state.graph[i].begin(), state.graph[i].end()), 
            state.graph[i].end()
        );
    }
}

    void Simulator::init_state() {
        state.N = data.N;
        state.target = data.target;
        state.cur_room = 0;
        state.cur_food = data.M;
        state.dead = false;
        state.rooms = data.rooms;
        state.remaining_resources.clear();
        for (const auto& room : state.rooms){
            state.remaining_resources.push_back(room.resources);
        }
        state.visited.assign(state.N + 1, false);
        state.visited[0] = true;
        state.collected = {0,0,0,0};
        state.if_collected.assign(state.N + 1, {false, false, false, false});
        build_graph();
    }

    void Simulator::print_state(int room_id) const {
        std::string line = "state " + std::to_string(room_id);
    for (int i = 0; i < 4; i++) {
        line += " ";
        if (state.if_collected[room_id][i]){
            line += "_";
        } else {
            line += std::to_string(state.remaining_resources[room_id][i]);
        }
    }
    output.write_line(line);
    }

    void Simulator::move(int room_id) {
        if (room_id == state.cur_room || state.dead) return;
        output.write_line("go " + std::to_string(room_id));
        state.cur_room = room_id;
        state.cur_food--;
        if (state.cur_food == 0 && state.cur_room != 0) state.dead = true;

        if (room_id != 0){
        print_state(room_id);
        }
    }

    void Simulator::collect(int room_id){
        if (state.dead) return;
        int targ = strategy->choose_resources(state, room_id);
        if (targ < 0){
            return;
        }
        output.write_line("collect " + resource_name(targ));

        int amount = state.remaining_resources[room_id][targ];
        
        if (amount > 0){
            state.remaining_resources[room_id][targ] = 0;
            state.collected[targ] += amount;
            state.if_collected[room_id][targ] = true;
        }

        if (state.visited[room_id]){
            state.cur_food--;
        }

        print_state(room_id);
        if (state.cur_food == 0 && state.cur_room != 0) state.dead = true;
    }

    void Simulator::enter_room(int room_id){
        collect(room_id);
        state.visited[room_id] = true;
    }

    void Simulator::exploration() {

        while (!state.dead && !strategy->switch_to_return_phase(state, data)) {
            int next_room = strategy->choose_next_room(state);
            if (next_room < 0) break;

            move(next_room);
            if (state.dead) return;

            enter_room(next_room);
        }
    }

    void Simulator::return_back() {
        state.return_phase = true;
        auto path = strategy->get_return_path(state, state.cur_room, 0);
        int distance = path.size() - 1;
        int extra_food = state.cur_food - distance;

        while(!state.dead && state.cur_room != 0){
            int next_room = strategy->choose_next_room(state);

            if (next_room < 0) break;

            move(next_room);
            if (state.dead) return;

            if (state.cur_room != 0) {
                while (extra_food > 0) {
                    int best_resource = strategy->choose_resources(state, state.cur_room);
                    if (best_resource < 0) break;

                    output.write_line("collect " + resource_name(best_resource));

                    int amount = state.remaining_resources[state.cur_room][best_resource];

                    if (amount > 0) {
                        state.remaining_resources[state.cur_room][best_resource] = 0;
                        state.collected[best_resource] += amount;
                        state.if_collected[state.cur_room][best_resource] = true;
                    }

                    print_state(state.cur_room);
                    state.cur_food--;
                    extra_food--;
                }
                    if (state.cur_food == 0 && state.cur_room != 0) {
                        state.dead = true;
                        return;
                    }
                
            }

        }
    }

    void Simulator::write_result() {
        output.write_result(state.collected, state.total_val());
    }

    void Simulator::simulate() {
        init_state();
        enter_room(0);
        exploration();
        if (!state.dead && state.cur_room != 0){
            return_back();
        } 

        if (!state.dead && state.cur_room == 0){
        write_result();
        }
    }