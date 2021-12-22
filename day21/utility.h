#include <iostream>
#include <fstream>
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

struct PlayerState {
    int position = 1;
    int score = 0;
};

void PrintPlayerState(PlayerState state) {
    std::cout << "Position: " << state.position << ", Score: " << state.score << std::endl;
    return;
}

struct States {
    PlayerState player_1;
    PlayerState player_2;
};

States GetStates(std::vector<std::string> lines) {
    States states;
    states.player_1.position = std::stoi(lines[4]);
    states.player_2.position = std::stoi(lines[9]);

    return states;
}

void PrintStates(States states) {
    std::cout << "Player 1 is at " << states.player_1.position << std::endl;
    std::cout << "Player 2 is at " << states.player_2.position << std::endl;
    std::cout << "Player 1 score " << states.player_1.score << std::endl;
    std::cout << "Player 2 score " << states.player_2.score << std::endl;
    return;
}

class DeterministicDie {
    public:
        DeterministicDie() {
             current_roll_ = kMinValue - 1;
             number_rolls_ = 0;
        }

        void Reset() {
            current_roll_ = kMinValue - 1;
        }

        int Roll() {
            ++current_roll_;
            if (current_roll_ > kMaxValue) {
                current_roll_ = kMinValue;
            }
            ++number_rolls_;
            return current_roll_;
        }

        int GetNumberRolls() {
            return number_rolls_;
        }

    private:
        int current_roll_;
        const int kMaxValue = 100;
        const int kMinValue = 1;
        int number_rolls_;
};

int AdvancePosition(int position, int advance) {
    int new_position = position + advance;
    if (new_position > 10) {
        new_position = (new_position % 10);
        if (new_position == 0) {
            new_position = 10;
        }
    }
    return new_position;
}

void PlayTurn(States& states, DeterministicDie& die, bool& is_turn_of_player_1) {
    auto& playing_state = (is_turn_of_player_1) ? states.player_1 : states.player_2;

    for (int i = 0; i <3; ++i) {
        int value = die.Roll();
        playing_state.position = AdvancePosition(playing_state.position, value);
    }

    playing_state.score += playing_state.position;

    // std::cout << ((is_turn_of_player_1)? "Player 1 " : "Player 2") << std::endl;
    // PrintPlayerState(playing_state);

    is_turn_of_player_1 = !is_turn_of_player_1;
    return;
}

const int kEndScore = 1000;

void PlayGame(States& states, DeterministicDie& die) {
    bool is_turn_of_player_1 = true;

    while (states.player_1.score < kEndScore && states.player_2.score < kEndScore) {
        PlayTurn(states, die, is_turn_of_player_1);
    }

    return;
}