#include <iostream>
#include <fstream>

#include "helper.h"

std::vector<XCandidate> FindXCandidates(Target target) {
    int num_x_targets = target.x_max - target.x_min + 1;

    // Collect pairs of (initial speed, final x position).
    std::vector<XCandidate> x_candidates;

    for (int i = 0; i < num_x_targets; ++i) {
        int target_x = target.x_min + i;    
        StatePosVel x_state;
        x_state.pos = target_x;
        x_state.vel = 0;
        int num_steps = 0;

        while (x_state.pos > 0) {
            x_state = BackwardStepX(x_state);
            ++num_steps;
        }

        if (x_state.pos == 0) {
            std::cout << "Found initial speed " << x_state.vel << " gets us to target x pos " << target_x;
            std::cout << " in " << num_steps << " steps." << std::endl;
            XCandidate candidate{.initial_speed = x_state.vel, .target_position = target_x, .num_steps = num_steps};
            x_candidates.push_back(candidate);
        }
    }

    return x_candidates;
}

int FindMinimumInitialXVel(std::vector<XCandidate> x_candidates) {
    int min_speed = x_candidates[0].initial_speed;
    for (auto& pair : x_candidates) {
        if (pair.initial_speed < min_speed) {
            min_speed = pair.initial_speed;
        }
    }

    return min_speed;
}

std::vector<YCandidate> FindYCandidates(Target target, std::vector<XCandidate> x_candidates) {
    int num_y_pos_targets = target.x_max - target.x_min + 1;

    std::vector<YCandidate> y_candidates;

    for (auto& x_candidate : x_candidates) {
        int initial_y_speed = 1;
        for (int initial_y_speed = 1; initial_y_speed < 100; ++initial_y_speed) {
            State state;
            state.x.pos = 0;
            state.x.vel = x_candidate.initial_speed;
            state.y.pos = 0;
            state.y.vel = initial_y_speed;

            int num_steps = 0;
            while(!ReachedTarget(state, target) && !MissedTarget(state, target)) {
                state = Step(state);
                ++num_steps;
            }

            if (ReachedTarget(state, target)) {
                // std::cout << "Reached Target with initial y speed: " << initial_y_speed << std::endl;
                YCandidate candidate{.initial_speed = initial_y_speed, .target_position = state.y.pos, .target_speed = state.y.vel, .num_steps = num_steps};
                y_candidates.push_back(candidate);
            }
        }
    }

    return y_candidates;
}

int FindMaxInitialYVel(std::vector<YCandidate> y_candidates) {
    int max_speed = y_candidates[0].initial_speed;
    for (auto& pair : y_candidates) {
        if (pair.initial_speed > max_speed) {
            max_speed = pair.initial_speed;
        }
    }
    return max_speed;
}

int FindMaxYPosition(State initial_state, Target target) {
    int max_height = 0;
    State state = initial_state;
    while (!ReachedTarget(state, target) && !MissedTarget(state, target)) {
        state = Step(state);

        if (state.y.pos > max_height) {
            max_height = state.y.pos;
        }
    }

    return max_height;
}

int main()
{
    std::vector<std::string> lines = ReadFile();

    Target target = GetTarget(lines);
    std::cout << "Target: x: [" << target.x_min << "," << target.x_max << "], ";
    std::cout << "y: [" << target.y_min << "," << target.y_max << "]" << std::endl;


    // First find slowest possible initial x speed that can get you to the target.
    std::vector<XCandidate> x_candidates = FindXCandidates(target);
    int min_x_vel = FindMinimumInitialXVel(x_candidates);
    std::cout << "Found min initial x speed: " << min_x_vel << std::endl;

    std::vector<YCandidate> y_candidates = FindYCandidates(target, x_candidates);
    int max_y_vel = FindMaxInitialYVel(y_candidates);
    std::cout << "Found max initial y speed: " << max_y_vel << std::endl;


    State initial_state;
    initial_state.x.pos = 0;
    initial_state.y.pos = 0;
    initial_state.x.vel = min_x_vel;
    initial_state.y.vel = max_y_vel;
    std::cout << "Found initial state: " << std::endl;
    PrintState(initial_state);
    // PrintTrajectory(initial_state, target);
    int max_height = FindMaxYPosition(initial_state, target);
    std::cout << "Found max height " << max_height << std::endl;

    return 0;
}