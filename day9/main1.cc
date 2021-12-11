#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int** GetHeightMap(std::vector<std::string> all_lines, int num_rows, int num_cols) {
    int** heightmap = new int*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        heightmap[row] = new int[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            heightmap[row][col] = all_lines[row][col] - '0';  // VERY IMPORTANT TO CAST FROM CHAR TO INT CORRECTLY ):
        }
    }
    return heightmap;
}

void PrintHeightMap(int** heightmap, int num_rows, int num_cols) {
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            std::cout << heightmap[row][col];
        }
        std::cout << std::endl;
    }
    return;
}

int ComputeRisk(int** heightmap, int num_rows, int num_cols, int row, int col) {
    int risk = 0;

    int value = heightmap[row][col];

    int left_value = (col > 0) ? heightmap[row][col - 1] : 10;
    int right_value = (col < num_cols - 1) ? heightmap[row][col + 1] : 10;
    int up_value = (row > 0) ? heightmap[row - 1][col] : 10;
    int down_value = (row < num_rows - 1) ? heightmap[row + 1][col] : 10;

    if ((value < left_value) && (value < right_value) && (value < up_value) && (value < down_value)){
        std::cout << "Found risk with value = " << value << std::endl;
        risk = value + 1;
    }

    return risk;
}

int ComputeTotalRisk(int** heightmap, int num_rows, int num_cols) {
    int risk = 0;
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            risk = risk + ComputeRisk(heightmap, num_rows, num_cols, row, col);
        }
    }
    return risk;
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

    int num_cols = all_lines[0].size();
    int num_rows = all_lines.size();
    std::cout << "Parsing " << num_rows << " rows and " << num_cols << " cols " << std::endl;

    int** heightmap = GetHeightMap(all_lines, num_rows, num_cols);
    PrintHeightMap(heightmap, num_rows, num_cols);

    int risk = ComputeTotalRisk(heightmap, num_rows, num_cols);
    std::cout << "Computed total risk of " << risk << std::endl;


    // Free the memory that's been allocated.
    for (int row = 0; row < num_rows; ++row) {
        delete [] heightmap[row];
    }
    delete [] heightmap;


    return 0;
}