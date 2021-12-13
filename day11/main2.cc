#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int kDimension = 10;

void GetOctopusMap(const std::vector<std::string>& all_lines, int (&octopus_map)[kDimension][kDimension]) {
    for (int row = 0; row < kDimension; ++row) {
        for (int col = 0; col < kDimension; ++col) {
            octopus_map[row][col] = all_lines[row][col] - '0';  // VERY IMPORTANT TO CAST FROM CHAR TO INT CORRECTLY ):
        }
    }
    return;
}

void PrintOctopusMap(const int octopus_map[][kDimension]) {
    for (int row = 0; row < kDimension; ++row) {
        for (int col = 0; col < kDimension; ++col) {
            std::cout << octopus_map[row][col];
        }
        std::cout << std::endl;
    } 
}

bool FlashesRemain(int octopus_map[][kDimension], bool flash_grid[][kDimension]) {
    for (int row = 0; row < kDimension; ++row) {
        for (int col = 0; col < kDimension; ++col) {
            if (octopus_map[row][col] > 9 && !flash_grid[row][col]) {
                return true;
            }
        }
    } 
    return false;
}

void IncrementNeighbors(int octopus_map[][kDimension], int row, int col) {
    // Up Left neighbor.
    if (row > 0 && col > 0) {
        ++octopus_map[row - 1][col - 1];
    }
    // Up center neighbor.
    if (row > 0) {
        ++octopus_map[row - 1][col];
    }
    // Up right neighbor.
    if (row > 0 && col < kDimension - 1) {
        ++octopus_map[row - 1][col + 1];
    }

    // Left neighbor.
    if (col > 0) {
        ++octopus_map[row][col - 1];
    }
    // Right neighbor.
    if (col < kDimension - 1) {
        ++octopus_map[row][col + 1];
    }
    // Down Left neighbor.
    if (row < kDimension - 1 && col > 0) {
        ++octopus_map[row + 1][col - 1];
    }
    // Down center neighbor.
    if (row < kDimension - 1) {
        ++octopus_map[row + 1][col];
    }
    // Down right neighbor.
    if (row < kDimension - 1 && col < kDimension - 1) {
        ++octopus_map[row + 1][col + 1];
    }
}

int Step(int octopus_map[][kDimension]) {
    bool flash_grid[kDimension][kDimension];
    // First, the energy level of each octopus increases by 1.
    for (int row = 0; row < kDimension; ++row) {
        for (int col = 0; col < kDimension; ++col) {
            ++octopus_map[row][col];
            flash_grid[row][col] = false; // Set up flash grid to all false.
        }
    }

    // Flash octopi.
    while (FlashesRemain(octopus_map, flash_grid))
    {
        for (int row = 0; row < kDimension; ++row)
        {
            for (int col = 0; col < kDimension; ++col)
            {
                if (octopus_map[row][col] > 9 && !flash_grid[row][col]) {
                    flash_grid[row][col] = true;
                    IncrementNeighbors(octopus_map, row, col);
                }
            }
        }
    }

    // Reset flashed octopic to 0 and count the number of flashes.
    int flashes = 0;
    for (int row = 0; row < kDimension; ++row)
    {
        for (int col = 0; col < kDimension; ++col)
        {
            if (flash_grid[row][col]) {
                ++flashes;
                octopus_map[row][col] = 0;
            }
        }
    }

    return flashes;
}

int main()
{
    std::string line;
    std::vector<std::string> all_lines;
    std::ifstream myfile("input.txt");

    if (myfile.is_open())
    {
        while (myfile >> line) {
            // std::cout << line << std::endl;
            all_lines.push_back(line);
        }
        myfile.close();
    }

    int octopus_map[kDimension][kDimension];
    GetOctopusMap(all_lines, octopus_map);

    PrintOctopusMap(octopus_map);

    int total_flashes = 0;
    for (int step = 0; step < 500; ++step) {
        int flashes = Step(octopus_map);
        total_flashes += flashes;
        if (step % 10 == 0) {
            PrintOctopusMap(octopus_map);
        }

        if (flashes == 100) {
            std::cout << "Everyone flashed at " << step + 1 << std::endl;
            PrintOctopusMap(octopus_map);
            return 0;
        }
    }

    return 0;
}