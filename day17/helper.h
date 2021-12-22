#include <string>
#include <vector>

std::vector<std::string> ReadFile() {
    std::string line;
    std::vector<std::string> lines;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> line) {
            lines.push_back(line);
        }
        myfile.close();
    }

    return lines;
}


struct Target {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
};

Target GetTarget(std::vector<std::string> lines) {
    Target target;
    std::string x_line = lines[2];
    std::string y_line = lines[3];

    bool start_record = false;
    bool first_period_found = false;
    std::string temp = "";
    for (int i = 0; i < x_line.size(); ++i) {
        char character = x_line[i];
        
        if (character == '.' && first_period_found) {
            start_record = true;
        } else if (character == '.' && !first_period_found) {
            first_period_found = true;
            start_record = false;
            target.x_min = std::stoi(temp);
            temp.clear();
        } else if (character == ',') {
            target.x_max = std::stoi(temp);
        } else if (start_record) {
            temp = temp + character;
        }

        if (character == '=') {
            start_record = true;
        }
    }

    start_record = false;
    first_period_found = false;
    temp.clear();
    for (int i = 0; i < y_line.size(); ++i) {
        char character = y_line[i];
        if (character == '.' && first_period_found) {
            start_record = true;
        } else if (character == '.' && !first_period_found) {
            first_period_found = true;
            start_record = false;
            target.y_min = std::stoi(temp);
            temp.clear();
        } else if (start_record) {
            temp = temp + character;
        }

        if (character == '=') {
            start_record = true;
        }
    }
    target.y_max = std::stoi(temp);

    return target;
}

struct StatePosVel {
    int pos;
    int vel;
};

struct State {
    StatePosVel x;
    StatePosVel y;
};

int sgn(int value) {
    return (value > 0) ? 1 : ((value < 0)? -1 : 0);
};

State Step(State state) {
    State next_state;

    next_state.x.pos = state.x.pos + state.x.vel;
    next_state.y.pos = state.y.pos + state.y.vel;
    next_state.x.vel = state.x.vel - sgn(state.x.vel);
    next_state.y.vel = state.y.vel - 1;

    return next_state;
}

void PrintPosition(State state) {
    std::cout << "position: (" << state.x.pos << "," << state.y.pos << ")" << std::endl;
    return;
}

void PrintState(State state) {
    std::cout << "position: (" << state.x.pos << "," << state.y.pos << ")" << std::endl;
    std::cout << "velocity: (" << state.x.vel << "," << state.y.vel << ")" << std::endl;
    return;
}

bool ReachedXTarget(State state, Target target) {
    return (state.x.pos >= target.x_min) && (state.x.pos <= target.x_max);
}

bool ReachedTarget(State state, Target target) {
    return ReachedXTarget(state, target) && (state.y.pos >= target.y_min) && (state.y.pos <= target.y_max);
}

bool MissedTarget(State state, Target target) {
    bool overshot_x = (state.x.pos > target.x_max);
    bool overshot_y = (state.y.pos < target.y_min);
    return overshot_x || overshot_y;
}

bool OvershotY(State state, Target target) {
    bool overshot_y = (state.y.pos < target.y_min);
    return overshot_y;
}

void PrintTrajectory(State initial_state, Target target) {
    PrintPosition(initial_state);

    State state = initial_state;
    while (!ReachedTarget(state, target) && !MissedTarget(state, target)) {
        state = Step(state);
        PrintPosition(state);
    }
    return;
}

StatePosVel BackwardStepX(StatePosVel x) {
    StatePosVel backward_state;
    backward_state.vel = x.vel + 1;
    backward_state.pos = x.pos - backward_state.vel;

    return backward_state;
}

// (initial speed, final x position).
struct XCandidate {
    int initial_speed;
    int target_position;
    int num_steps;
};

StatePosVel BackwardStepY(StatePosVel y) {
    StatePosVel backward_state;
    backward_state.vel = y.vel + 1;
    backward_state.pos = y.pos - backward_state.vel;

    return backward_state;
}


struct YCandidate {
    int initial_speed;
    int target_position;
    int target_speed;
    int num_steps;
};