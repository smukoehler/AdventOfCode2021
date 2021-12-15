#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int kRepeats = 5;
const int kLargeNumber = 999999;

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

int** GetFullRiskMap(int** tile, int num_rows, int num_cols) {
    int** fullmap = new int*[num_rows * kRepeats];
    std::cout << "fullmap size " << num_rows * kRepeats << "x" << num_cols * kRepeats << std::endl;
    for (int row = 0; row < num_rows * kRepeats; ++row) {
        fullmap[row] = new int[num_cols * kRepeats];
    }

    for (int row_repeat = 0; row_repeat < kRepeats; ++row_repeat) {
        for (int col_repeat = 0; col_repeat < kRepeats; ++col_repeat) {
            int number_shift = row_repeat + col_repeat;
            for (int row = 0; row < num_rows; ++row) {
                for (int col = 0; col < num_cols; ++col) {
                    int value = tile[row][col] + number_shift;
                    if (value >= 10 ) {
                        value = (value % 10) + 1;
                    }
                    fullmap[row_repeat * num_rows + row][col_repeat * num_cols + col] = value;
                }
            }

        }
    }
    return fullmap;
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

// int MinCost(int** riskmap,  int row, int col) {
//     if (row < 0 || col < 0) {
//         return kLargeNumber;
//     } else if (row == 0 && col == 0)
//         return 0;
//     else {
//         return riskmap[row][col] +
//                std::min(MinCost(riskmap, row - 1, col),
//                         MinCost(riskmap, row, col - 1));
//     }
// }

int MinCost(int** riskmap, int** costtogo, int row, int col) {
    if (row < 0 || col < 0 || row > 499 || col > 499) {
        return kLargeNumber;
    } else if (row == 0 && col == 0) {
        return 0;
    }
    else {
        int leftvalue = (row > 0) ? costtogo[row - 1][col] : -1;
        int upvalue = (col > 0) ? costtogo[row][col - 1] : -1;
        int rightvalue = (row < 499) ? costtogo[row + 1][col] : -1;
        int downvalue = (col < 499) ? costtogo[row][col + 1] : -1;
        if ((leftvalue != -1) && (upvalue != -1) && (rightvalue != -1) && (downvalue != -1)) {
            std::cout << "Costs are not -1: " << leftvalue << ", " << upvalue << std::endl;
            std::cout << "Min is: " <<  std::min(leftvalue, upvalue) << std::endl;
            return riskmap[row][col] + std::min({leftvalue, upvalue, rightvalue, downvalue});
        } else if ((leftvalue != -1) && (upvalue != -1)) {
            std::cout << "Both costs are not -1: " << leftvalue << ", " << upvalue << std::endl;
            std::cout << "Min is: " <<  std::min(leftvalue, upvalue) << std::endl;
            return riskmap[row][col] + std::min({leftvalue, upvalue,
                                                 MinCost(riskmap, costtogo, row + 1, col),
                                                 MinCost(riskmap, costtogo, row, col + 1)});
        }else {
            return riskmap[row][col] + 
               std::min({MinCost(riskmap, costtogo, row - 1, col),
                        MinCost(riskmap, costtogo, row, col - 1),
                        MinCost(riskmap, costtogo, row + 1, col),
                        MinCost(riskmap, costtogo, row, col + 1)});
        }
    }
}


int** InitializeCostToGo(int num_rows, int num_cols, int** riskmap) {
    int** costtogo = new int*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        costtogo[row] = new int[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            costtogo[row][col] = -1;
        }
    }
    
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            std::cout << "Computing cost to go at row " << row << ", col " << col << std::endl;
            costtogo[row][col] = MinCost(riskmap, costtogo, row, col);
        }
    }
    return costtogo;
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
    // std::cout << "tile size: " << num_rows << "x" << num_cols << std::endl;
    int** riskmap = GetRiskMap(all_lines, num_rows, num_cols);
    // std::cout << "tile risk map" << std::endl;
    // PrintRiskMap(riskmap, num_rows, num_cols);
    int** fullmap = GetFullRiskMap(riskmap, num_rows, num_cols);
    // std::cout << "full risk map" << std::endl;
    // PrintRiskMap(fullmap, num_rows * kRepeats, num_cols * kRepeats);

    int** costtogo = InitializeCostToGo(num_rows * kRepeats, num_cols * kRepeats, fullmap);
    // std::cout << "cost to go" << std::endl;
    // PrintRiskMap(costtogo, num_rows * kRepeats, num_cols * kRepeats);
    std::cout << "cost to go of bottom: " << costtogo[num_rows * kRepeats - 1][num_cols * kRepeats - 1] << std::endl;

    std::cout << MinCost(fullmap, costtogo, num_rows * kRepeats - 1, num_cols * kRepeats - 1) << std::endl;

    // Free the memory that's been allocated.
    for (int row = 0; row < num_rows; ++row) {
        delete [] riskmap[row];
    }
    delete [] riskmap;

    for (int row = 0; row < num_rows * kRepeats; ++row) {
        delete [] fullmap[row];
    }
    delete [] fullmap;


    for (int row = 0; row < num_rows * kRepeats; ++row) {
        delete [] costtogo[row];
    }
    delete [] costtogo;

    return 0;
}