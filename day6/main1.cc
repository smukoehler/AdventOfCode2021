#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const int num_days = 80;

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

void StepSimulation(std::vector<int>& fishes) {
    int num_fishes = fishes.size();
    for (int i = 0; i < num_fishes; ++i) {
        auto& fish = fishes[i];
        if (fish == 0) {
            fish = 6;
            fishes.push_back(9); // Because it will get updated before the for loop is done.
        } else  {
            --fish;
        }
        if (i == num_fishes - 1) {
            // Update number of fishes because fishes may have been added.
            num_fishes = fishes.size();
        }
    }

    return;
}

void PrintFishes(std::vector<int> fishes) {
    for (auto& fish : fishes) {
        std::cout << fish << ",";
    }
    std::cout << std::endl;
}

std::vector<int> RunSimulation(const std::vector<int>& initial_fishes) {
    std::vector<int> fishes = initial_fishes;
    for (int i = 0; i < num_days; ++i) {
        StepSimulation(fishes);
        // PrintFishes(fishes);
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

    std::vector<int> final_fishes = RunSimulation(initial_fishes);

    PrintFishes(final_fishes);

    std::cout << "Number of fishes = " << final_fishes.size() << std::endl;

    return 0;
}