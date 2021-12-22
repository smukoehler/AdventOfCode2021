#include <iostream>
#include <fstream>

#include "helper.h"

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