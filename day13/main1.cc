#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Coordinate {
    int x = 0;
    int y = 0;
};

int GetCoordinates(const std::vector<std::string>& words, std::vector<Coordinate>& coords) {
    Coordinate coord;
    int num_words = words.size();
    for (int i = 0; i < num_words; ++i) {
        auto word = words[i];
        if (word.compare("fold") == 0) {
            return i;
        } 
        
        std::stringstream coordstream(word); //create string stream from the string
        int coordidx = 0;
        while (coordstream.good())
        {
            std::string value;
            std::getline(coordstream, value, ','); //get first string delimited by comma
            if (coordidx == 0) {
                coord.x = std::stoi(value);
                ++coordidx;
            } else {
                coord.y = std::stoi(value);
                coords.push_back(coord);
            }
        }
    }

    return -1;
}

void PrintCoords(const std::vector<Coordinate>& coords) {
    for (auto & coord : coords) {
        std::cout << "(" << coord.x << "," << coord.y << ")" << std::endl;
    }
}

using Instructions = std::vector<std::pair<char,int>>;

Instructions GetFoldInstructions(const std::vector<std::string>& words, int foldindex) {
    Instructions instructions;

    for (int i = foldindex; i < words.size(); ++i) {
        auto word = words[i];
        if (word.find('x') != std::string::npos) {
            instructions.push_back({'x', std::stoi(word.substr(2, word.size()))});
        }

        if (word.find('y') != std::string::npos) {
            instructions.push_back({'y', std::stoi(word.substr(2, word.size()))});
        }
    }

    return instructions;
}

void PrintInstructions(const Instructions& instructions) {
    for (auto& step : instructions) {
        std::cout << step.first << ": " << step.second << std::endl;
    }
}

int CountAndPrintGrid(const std::vector<Coordinate> &coords)
{
    // PrintCoords(coords);
    int count = 0;
    int maxX = 0;
    int maxY = 0;
    for (auto &coord : coords)
    {
        if (coord.x > maxX)
        {
            maxX = coord.x + 2;
        }
        if (coord.y > maxY)
        {
            maxY = coord.y + 2;
        }
    }

    // std::cout << "maxX : " << maxX << ", maxY : " << maxY << std::endl;

    int grid[maxX][maxY];
    for (int i = 0; i < maxX; ++i)
    {
        for (int j = 0; j < maxY; ++j)
        {
            grid[i][j] = 0;
        }
    }

    for (auto &coord : coords)
    {
        // std::cout << "Gridding " << coord.x << "," << coord.y << std::endl;
        grid[coord.x][coord.y] = 1;
    }

    for (int y = 0; y < maxY; ++y)
    {
        for (int x = 0; x < maxX; ++x)
        {
            count += grid[x][y];
            if (grid[x][y] == 1)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "grid counted: " << count << std::endl;

    return count;
}


Coordinate FoldX(const Coordinate& coord, int foldx) {
    if (coord.x > foldx) {
        int newx = coord.x - 2 * (coord.x - foldx);
        Coordinate new_coord{.x = newx, .y = coord.y};
        return new_coord;
    } else {
        return coord;
    }
}

Coordinate FoldY(const Coordinate& coord, int foldy) {
    if (coord.y > foldy) {
        int newy = coord.y - 2 * (coord.y - foldy);
        Coordinate new_coord{.x = coord.x, .y = newy};
        return new_coord;
    } else {
        return coord;
    }
}

std::vector<Coordinate> Fold(const std::vector<Coordinate>& coords, const Instructions& instructions) {
    std::vector<Coordinate> folded_coords = coords;
    for (auto& step : instructions) {
        std::vector<Coordinate> initial_coords = folded_coords;
        folded_coords.clear();
        if (step.first == 'x') {
            int foldx = step.second;
            std::cout << "Folding x at " << foldx << std::endl;
            for (auto& coord : initial_coords) {
                Coordinate new_coord = FoldX(coord, foldx);
                folded_coords.push_back(new_coord);
            }
        }
        if (step.first == 'y') {
            int foldy = step.second;
            std::cout << "Folding y at " << foldy << std::endl;
            for (auto& coord : initial_coords) {
                Coordinate new_coord = FoldY(coord, foldy);
                folded_coords.push_back(new_coord);
            }
        }

        // PrintCoords(folded_coords);
        std::cout << "Updated num of coords: " << folded_coords.size() << std::endl;
        CountAndPrintGrid(folded_coords);
    }

    return folded_coords;
}

int main()
{
    std::string word;
    std::vector<std::string> words;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> word) {
            // std::cout << word << std::endl;
            words.push_back(word);
        }
        myfile.close();
    }

    std::vector<Coordinate> coords;
    int foldindex = GetCoordinates(words, coords);
    // PrintCoords(coords);
    std::cout << "initial num of coords: " << coords.size() << std::endl;
    CountAndPrintGrid(coords);

    auto instructions = GetFoldInstructions(words, foldindex);
    PrintInstructions(instructions);

    auto finalcoords = Fold(coords, instructions);
    // PrintCoords(finalcoords);

    // int count = CountAndPrintGrid(finalcoords);
    // std::cout << "visible dots: " << count << std::endl;

    return 0;
}