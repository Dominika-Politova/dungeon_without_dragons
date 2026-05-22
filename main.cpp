#include "Dungeon.h"
#include "Parser.h"
#include "Alice_strategy.h"
#include "Output.h"
#include "Bot_simulator.h"

#include "iostream"

int main() {
    Input_data data;
    std::string error;

    Parser parser;
    if (!parser.parse_input("in.txt", data, error)) {
        Output output("result.txt");
        output.write_line(error);
        return 0;
    }

    Output output("result.txt");
    std::unique_ptr<Strategy> strategy = std::make_unique<Alice_strategy> ();

    Simulator simulator(data, output, std::move(strategy));
    simulator.simulate();

    return 0;
}