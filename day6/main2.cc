#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const int num_days = 256;
const int max_counter = 10;

std::vector<int> GetInitialList(std::string longlist){
    std::vector<int> list;
    std::stringstream liststream(longlist); //create string stream from the string
    while (liststream.good())
    {
        std::string number;
        std::getline(liststream, number, ','); //get first string delimited by comma
        list.push_back(std::stoi(number));
    }

    return list;
}

std::array<long, max_counter> GetInitialCounter(const std::vector<int>& fishes) {
    std::array<long, max_counter> fishcounter = {};
    for (auto fish : fishes) {
        fishcounter[fish]++;
    }
    return fishcounter;
}

void PrintFishCounter(std::array<long, max_counter> fishes) {
    for (int i = 0; i < max_counter; ++i) {
        std::cout << "timer = " << i << " : num fishes = " << fishes[i] << std::endl;
    }
    std::cout << std::endl;
}

long CountFishes(std::array<long, max_counter>& fishes) {
    long num_fishes = 0;
    for (int i = 0; i < max_counter; ++i) {
        num_fishes = num_fishes + fishes[i];
    }
    return num_fishes;
}

void StepSimulation(std::array<long, max_counter>& fishes) {
    int num_fishes = CountFishes(fishes);
    for (int i = 0; i < max_counter; ++i) {
        std::cout << "index i = " << i << std::endl;
        if (i == 0) {
            // Index 7 = how many fishes at timer 6.
            fishes[7] = fishes[7] + fishes[i]; // Because it will get updated before the for loop is done. So this will become 6.
            fishes[9] = fishes[9] + fishes[i]; // This will get updated to 8 by the end of the for loop.
            fishes[i] = fishes[i+1];
        } else if (i < max_counter - 1) {
            fishes[i] = fishes[i+1];
        } else {
            fishes[i] = 0;
        }
        if (i == num_fishes - 1) {
            // Update number of fishes because fishes may have been added.
            num_fishes = CountFishes(fishes);
        }

        std::cout << "Updated fishes at i = " << i << " is " << fishes[i] << std::endl;
    }

    return;
}


std::array<long, max_counter> RunSimulation(const std::array<long, max_counter>& initial_fishes) {
    std::array<long, max_counter> fishes = initial_fishes;
    for (int i = 0; i < num_days; ++i) {
        StepSimulation(fishes);
        PrintFishCounter(fishes);
    }
    return fishes;
}

int main()
{
    std::string line;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> line) {
            std::cout << line << std::endl;
        }
        myfile.close();
    }

    std::vector<int> initial_fishes = GetInitialList(line);

    std::array<long, max_counter> fishcounter = GetInitialCounter(initial_fishes);

    PrintFishCounter(fishcounter);

    auto finalcounter = RunSimulation(fishcounter);

    std::cout << "Final number of fish = " << CountFishes(finalcounter) << std::endl;

    return 0;
}