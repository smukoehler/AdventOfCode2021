#include <iostream>
#include <fstream>

#include "helper.h"

std::vector<State> FindCandidates(Target target) {
    std::vector<State> states;
    
    int min_x_speed = 0;
    int max_x_speed = 1000;
    int min_y_speed = -1000; // target.y_min;
    int max_y_speed = 1000;

    int num_x_speeds = max_x_speed - min_x_speed + 1;
    int num_y_speeds = max_y_speed - min_y_speed + 1;

    for (int i = 0; i < num_x_speeds; ++i) {
        int initial_x_speed = min_x_speed + i;
        for (int j = 0; j <num_y_speeds; ++j) {
            int initial_y_speed = min_y_speed + j;

            State initial_state;
            initial_state.x.pos = 0;
            initial_state.x.vel = initial_x_speed;
            initial_state.y.pos = 0;
            initial_state.y.vel = initial_y_speed;

            State state = initial_state;
            while(!ReachedTarget(state, target) && !MissedTarget(state, target)) {
                state = Step(state);
            }

            if (ReachedTarget(state, target)) {
                states.push_back(initial_state);
            }
        }
    }
    return states;
}

int main()
{
    std::vector<std::string> lines = ReadFile();

    Target target = GetTarget(lines);
    std::cout << "Target: x: [" << target.x_min << "," << target.x_max << "], ";
    std::cout << "y: [" << target.y_min << "," << target.y_max << "]" << std::endl;


    // Find all the initial states that land in the target.
    std::vector<State> initial_states = FindCandidates(target);

    std::cout << "Found " << initial_states.size() << " initial states. " << std::endl;


    return 0;
}