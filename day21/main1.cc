
#include "utility.h"

int main()
{
    std::vector<std::string> lines = ReadFile();

    States states = GetStates(lines);
    PrintStates(states);

    DeterministicDie die;
    // for (int i = 0; i < 101; ++i) {
    //     std::cout << die.Roll() << ", ";
    // }
    // std::cout << std::endl;

    PlayGame(states, die);

    int losing_score = std::min(states.player_1.score, states.player_2.score);
    std::cout << "Losing score: " << losing_score << std::endl;
    int num_die_rolls = die.GetNumberRolls();
    std::cout << "Number of die rolls: " << num_die_rolls << std::endl;

    std::cout << "Final answer: " << losing_score * num_die_rolls << std::endl; 

    return 0;
}
