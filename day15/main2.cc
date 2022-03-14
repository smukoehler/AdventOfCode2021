#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <utility>
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

int** InitializeDistanceMap(int num_rows, int num_cols) {
    int** map = new int*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        map[row] = new int[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            map[row][col] = std::numeric_limits<int>::max();
        }
    }
    // The first node is special.
    map[0][0] = 0;
    return map;
}

bool** InitializeVisitedNodes(int num_rows, int num_cols) {
    bool** map = new bool*[num_rows];
    for (int row = 0; row < num_rows; ++row) {
        map[row] = new bool[num_cols];
        for (int col = 0; col < num_cols; ++col) {
            map[row][col] = false;
        }
    }
    return map;
}


void PrintMap(int** map, int num_rows, int num_cols) {
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            std::cout << map[row][col];
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

bool AreAllNodesVisited(bool** visited_nodes, int num_rows, int num_cols) {
    bool check = true;
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            check = check && visited_nodes[row][col];
        }
    }
    // std::cout << "all nodes visited? " << check << std::endl;
    return check;
}

// It turns out this is not a good approach because then you are not keeping track of the shortest possible path. It does appear to work
// in all cases where bellman's principle of optimality applies.
void ChooseChronologicalNext(int& current_row, int& current_col) {
    int next_row = current_row;
    int next_col = current_col;
    // If on the diagonal, start at the top of the rows in the next col.
    if (current_row == current_col) {
        next_row = 0;
        next_col++;
    }
    // If row < col, proceed down the col until you almost get to the diagonal. Then jump to col 0.
    if (current_row < current_col) {
        if (current_row == current_col - 1) {
            next_row++;
            next_col = 0;
        } else {
            next_row++;
            next_col = current_col;
        }
    }
    if (current_col < current_row) {
        next_row = current_row;
        next_col++;
    }
    // std::cout << "Next node at: (" << next_row << " , " << next_col << ")" << std::endl;
    // RunDijkstra(distance_map, visited_nodes, fullmap, next_row, next_col, num_rows, num_cols);
    current_row = next_row;
    current_col = next_col;
}

// Always choose the minimum distance unvisited node so that you are always keeping track of the shortest path.
void ChooseMinimumDistance(int** distance_map, std::vector<std::pair<int, int>>&unvisited_nodes, int& current_row, int& current_col) {
    int next_row = unvisited_nodes[0].first;
    int next_col = unvisited_nodes[0].second;
    for (auto& node : unvisited_nodes) {
        if(distance_map[node.first][node.second] < distance_map[next_row][next_col]) {
            next_row = node.first;
            next_col = node.second;
        }
    }
    current_row = next_row;
    current_col = next_col;

    // Remove the selected node from unvisited nodes.
    std::vector<std::pair<int, int>> reduced_nodes;
    for (auto& node : unvisited_nodes) { 
        if (node.first != next_row || node.second != next_col) {
            reduced_nodes.push_back(node);
        }
    }
    unvisited_nodes = reduced_nodes;
    return;
}

void RunDijkstra(int** distance_map, bool** visited_nodes, std::vector<std::pair<int, int>>& unvisited_nodes, int** fullmap, int& current_row, int& current_col, int num_rows, int num_cols) {
    int current_cost = distance_map[current_row][current_col];
    std::cout << "Current node at: (" << current_row << " , " << current_col << ")" << std::endl;
    std::cout << "Current cost: " << current_cost << std::endl;
    
    // Visit the neighbors of the current node.
    // Left neighbor.
    if (current_col > 0) {
        int neighbor_row = current_row;
        int neighbor_col = current_col - 1;
        int distance = (int)fullmap[neighbor_row][neighbor_col] + current_cost;
        if (distance < distance_map[neighbor_row][neighbor_col]) {
            distance_map[neighbor_row][neighbor_col] = distance;
        }
        if (visited_nodes[neighbor_row][neighbor_col] == false) {
            unvisited_nodes.push_back(std::make_pair(neighbor_row, neighbor_col));
        }
    }
    // Right neighbor.
    if (current_col < num_cols - 1) {
        int neighbor_row = current_row;
        int neighbor_col = current_col + 1;
        int distance = (int)fullmap[neighbor_row][neighbor_col] + current_cost;
        if (distance < distance_map[neighbor_row][neighbor_col]) {
            distance_map[neighbor_row][neighbor_col] = distance;
        }
        if (visited_nodes[neighbor_row][neighbor_col] == false) {
            unvisited_nodes.push_back(std::make_pair(neighbor_row, neighbor_col));
        }
    }
    // Up neighbor.
    if (current_row > 0) {
        int neighbor_row = current_row - 1;
        int neighbor_col = current_col;
        int distance = (int)fullmap[neighbor_row][neighbor_col] + current_cost;
        if (distance < distance_map[neighbor_row][neighbor_col]) {
            distance_map[neighbor_row][neighbor_col] = distance;
        }
        if (visited_nodes[neighbor_row][neighbor_col] == false) {
            unvisited_nodes.push_back(std::make_pair(neighbor_row, neighbor_col));
        }
    }
    // Up neighbor.
    if (current_row < num_rows - 1) {
        int neighbor_row = current_row + 1;
        int neighbor_col = current_col;
        int distance = (int)fullmap[neighbor_row][neighbor_col] + current_cost;
        if (distance < distance_map[neighbor_row][neighbor_col]) {
            distance_map[neighbor_row][neighbor_col] = distance;
        }
        if (visited_nodes[neighbor_row][neighbor_col] == false) {
            unvisited_nodes.push_back(std::make_pair(neighbor_row, neighbor_col));
        }
    }

    // Mark the current node as visited.
    visited_nodes[current_row][current_col] = true;

    // Choose the next node to visit.
    // ChooseChronologicalNext(current_row, curent_col);
    ChooseMinimumDistance(distance_map, unvisited_nodes, current_row, current_col);
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

    // Although I wrote a beautiful dynamic programming solution, bellman's equation does not apply in the general case because
    // the lowest risk could include "backwards" (up or left) steps.
    // int** costtogo = InitializeCostToGo(num_rows * kRepeats, num_cols * kRepeats, fullmap);
    // PrintRiskMap(costtogo, num_rows * kRepeats, num_cols * kRepeats);
    // std::cout << MinCost(fullmap, costtogo, num_rows * kRepeats - 1, num_cols * kRepeats - 1) << std::endl;

    // List of all the nodes that have a non-infinite distance, but are yet unvisited.
    std::vector<std::pair<int, int>> unvisited_nodes;
    bool** visited_nodes = InitializeVisitedNodes(num_rows * kRepeats, num_cols * kRepeats);
    int** distance_map = InitializeDistanceMap(num_rows * kRepeats, num_cols * kRepeats);
    // PrintMap(distance_map, num_rows, num_cols);

    int row = 0;
    int col = 0;
    while (!AreAllNodesVisited(visited_nodes, num_rows * kRepeats, num_cols * kRepeats)) {
        RunDijkstra(distance_map, visited_nodes, unvisited_nodes, fullmap, row, col, num_rows * kRepeats, num_cols * kRepeats);
        std::cout << "Next node at: (" << row << " , " << col << ")" << std::endl;
    }
    // PrintMap(distance_map, num_rows, num_cols);

    std::cout << "Risk computation: " << distance_map[num_rows * kRepeats - 1][num_cols * kRepeats - 1] << std::endl;

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
        delete [] visited_nodes[row];
    }
    delete [] visited_nodes;

    for (int row = 0; row < num_rows * kRepeats; ++row) {
        delete [] distance_map[row];
    }
    delete [] distance_map;

    return 0;
}