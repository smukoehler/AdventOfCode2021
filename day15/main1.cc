#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int** GetRiskMap(std::vector<std::string> all_lines, int num_rows, int num_cols) {
    int** riskmap = new int*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        riskmap[row] = new int[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            riskmap[row][col] = all_lines[row][col] - '0';  // VERY IMPORTANT TO CAST FROM CHAR TO INT CORRECTLY ):
        }
    }
    return riskmap;
}

int** InitializeCostToGo(int num_rows, int num_cols) {
    int** costtogo = new int*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        costtogo[row] = new int[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            costtogo[row][col] = -1;
        }
    }
    return costtogo;
}

void PrintRiskMap(int** riskmap, int num_rows, int num_cols) {
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            std::cout << riskmap[row][col];
        }
        std::cout << std::endl;
    }
    return;
}

int MinCost(int** riskmap, int** costtogo, int row, int col) {
    if (row < 0 || col < 0) {
        return std::numeric_limits<int>::max();
    } else if (row == 0 && col == 0)
        return 0;
    else {
        if (row > 0 && costtogo[row - 1][col] != -1 && col > 0 && costtogo[row][col - 1] != -1) {
            return riskmap[row][col] + std::min(costtogo[row - 1][col], costtogo[row][col - 1]);
        } else if (row > 0 && costtogo[row - 1][col] != -1) {
            return riskmap[row][col] + std::min(costtogo[row - 1][col], MinCost(riskmap, costtogo, row, col - 1));
        } else if (col > 0 && costtogo[row][col - 1] != -1) {
            return riskmap[row][col] + std::min(MinCost(riskmap, costtogo, row - 1, col), costtogo[row][col - 1]);
        } else {
            costtogo[row][col] = riskmap[row][col] +
               std::min(MinCost(riskmap, costtogo, row - 1, col),
                        MinCost(riskmap, costtogo, row, col - 1));
            return costtogo[row][col];
        }
    }
}


int main()
{
    std::string line;
    std::vector<std::string> all_lines;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> line) {
            all_lines.push_back(line);
        }
        myfile.close();
    }

    int num_rows = all_lines.size();
    int num_cols = all_lines[0].size();
    int** riskmap = GetRiskMap(all_lines, num_rows, num_cols);
    // PrintRiskMap(riskmap, num_rows, num_cols);

    int** costtogo = InitializeCostToGo(num_rows, num_cols);

    std::cout << MinCost(riskmap, costtogo, num_rows - 1, num_cols - 1) << std::endl;

    // Free the memory that's been allocated.
    for (int row = 0; row < num_rows; ++row) {
        delete [] riskmap[row];
    }
    delete [] riskmap;

    return 0;
}