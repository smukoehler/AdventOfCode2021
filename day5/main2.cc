#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const int maxpos = 1000;

std::vector<int> ExtractCoordinate(std::string coordinates){
    std::vector<int> coordvector;
    std::stringstream coordstream(coordinates); //create string stream from the string
    while (coordstream.good())
    {
        std::string number;
        std::getline(coordstream, number, ','); //get first string delimited by comma
        coordvector.push_back(std::stoi(number));
    }

    return coordvector;
}

void IncreaseVentCount(std::vector<int> startcoord, std::vector<int> endcoord, int vents[maxpos][maxpos]) {

    if (startcoord[0] == endcoord[0]) {
        int x = startcoord[0];
        int ystart = std::min(startcoord[1], endcoord[1]);
        int yend = std::max(startcoord[1], endcoord[1]);
        for (int y = ystart; y <= yend; ++y) {
            vents[y][x]++;
        }
    } else if (startcoord[1] == endcoord[1]){
        int y = startcoord[1];
        int xstart = std::min(startcoord[0], endcoord[0]);
        int xend = std::max(startcoord[0], endcoord[0]);
        for (int x = xstart; x <= xend; ++x) {
            vents[y][x]++;
        }
    } else {
        int xstart = std::min(startcoord[0], endcoord[0]);
        int xend = std::max(startcoord[0], endcoord[0]);
        int ystart = std::min(startcoord[1], endcoord[1]);
        int yend = std::max(startcoord[1], endcoord[1]);
        int num_points = std::abs(startcoord[0] - endcoord[0]) + 1;

        std::vector<int> xcoords;
        std::vector<int> ycoords;


        int x = startcoord[0];
        int y = startcoord[1];
        for (int i = 0; i < num_points; ++i) {
            vents[y][x]++;
            if (endcoord[0] > startcoord[0]) {
                ++x;
            } else {
                --x;
            }
            if (endcoord[1] > startcoord[1]) {
                ++y;
            } else {
                --y;
            }
        }
    }
    return;
}

void PrintVent(int vents[maxpos][maxpos]) {
    for (int x = 0; x < maxpos; ++x) {
        for (int y = 0; y < maxpos; ++y) {
            std::cout << vents[x][y];
            if (y == maxpos - 1) {
                std::cout << std::endl;
            }
        }
    }
}

int CountVentsFound(int vents[maxpos][maxpos]) {
    int found = 0;
    for (int x = 0; x < maxpos; ++x) {
        for (int y = 0; y < maxpos; ++y) {
            if (vents[y][x] >= 2) {
                ++found;
            }
        }
    }
    return found;
}

int main() {
    std::string word;
    std::ifstream myfile("input.txt");
    int word_index = 0;
    std::vector<int> startcoord;
    std::vector<int> endcoord;
    int vents[maxpos][maxpos] = {};

    std::cout << "Initial vent" << std::endl;
    PrintVent(vents);

    if (myfile.is_open())
    {
        while (myfile >> word) {
            std::cout << word << std::endl;
            if (word_index % 3 == 0 ) {
                startcoord = ExtractCoordinate(word);
            }
            if (word_index % 3 == 2) {
                endcoord = ExtractCoordinate(word);

                std::cout << "Found start coord (" << startcoord[0] << "," << startcoord[1] << ")" << std::endl;
                std::cout << "Found end coord (" << endcoord[0] << "," << endcoord[1] << ")" << std::endl;

                IncreaseVentCount(startcoord,endcoord, vents);
            }

            ++word_index;
        }
        myfile.close();
    }


    std::cout << "Final vent" << std::endl;
    PrintVent(vents);

    int vents_found = CountVentsFound(vents);
    std::cout << "Found vents: " << vents_found << std::endl;

    return 0;
}