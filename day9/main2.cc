#include <iostream>
#include <fstream>
#include <optional>
#include <queue>
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

struct LowPoint {
    int value = 0;
    int row = 0;
    int col = 0;
};

std::optional<LowPoint> CheckForLowPoint(int** heightmap, int num_rows, int num_cols, int row, int col) {
    int value = heightmap[row][col];

    int left_value = (col > 0) ? heightmap[row][col - 1] : 10;
    int right_value = (col < num_cols - 1) ? heightmap[row][col + 1] : 10;
    int up_value = (row > 0) ? heightmap[row - 1][col] : 10;
    int down_value = (row < num_rows - 1) ? heightmap[row + 1][col] : 10;

    if ((value < left_value) && (value < right_value) && (value < up_value) && (value < down_value)){
        LowPoint low_point = {.value = value, .row = row, .col = col};
        return std::optional<LowPoint>{low_point};
    }

    return std::nullopt;
}

std::vector<LowPoint> FindLowPoints(int** heightmap, int num_rows, int num_cols) {
    std::vector<LowPoint> low_points;
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            if (auto low_point = CheckForLowPoint(heightmap, num_rows, num_cols, row, col)) {
                low_points.push_back(*low_point);
            }
        }
    }
    return low_points;
}

void PrintPoint(const LowPoint& point) {
     std::cout << "Point has value = " << point.value << " at row " << point.row << ", col " << point.col << std::endl;
     return;
}

void PrintLowPoints(std::vector<LowPoint>& low_points) {
    for (auto& low_point : low_points) {
        std::cout << "Low point has value = " << low_point.value << " at row " << low_point.row << ", col " << low_point.col << std::endl;
    }
}

struct Basin {
    LowPoint low_point;
    // The points in the basin, aligned by element of each vector.
    std::vector<LowPoint> basin_points;
    int score = 1;
};

std::vector<LowPoint> FindChildNodes(const LowPoint& tree_node, int** height_map, int num_rows, int num_cols){
    std::vector<LowPoint> child_nodes;

    auto& row = tree_node.row;
    auto& col = tree_node.col;

    // Check left value.
    if (tree_node.col > 0) {
        auto left_value = height_map[row][col - 1];
        if (left_value < 9 && left_value >= tree_node.value) {
            child_nodes.push_back(LowPoint{.value = left_value, .row = row, .col = col - 1});
        }
    }

    // Check right value.
    if (tree_node.col < num_cols - 1) {
        auto right_value = height_map[row][col + 1];
        if (right_value < 9 && right_value >= tree_node.value) {
            child_nodes.push_back(LowPoint{.value = right_value, .row = row, .col = col + 1});
        }
    }

    // Check up value.
    if (tree_node.row > 0) {
        auto up_value = height_map[row - 1][col];
        if (up_value < 9 && up_value >= tree_node.value) {
            child_nodes.push_back(LowPoint{.value = up_value, .row = row - 1, .col = col});
        }
    }

    // Check down value.
    if (tree_node.row < num_rows - 1) {
        auto down_value = height_map[row + 1][col];
        if (down_value < 9 && down_value >= tree_node.value) {
            child_nodes.push_back(LowPoint{.value = down_value, .row = row + 1, .col = col});
        }
    }

    return child_nodes;
}

Basin FindBasin(const LowPoint& low_point, int** height_map, int num_rows, int num_cols) {
    Basin basin;
    basin.low_point = low_point;

    bool searched_map[num_rows][num_cols];
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            searched_map[row][col] = false;
        }
    }

    std::deque<LowPoint> searchpoints;
    searchpoints.push_back(low_point);
    while (searchpoints.size() > 0) {
        auto tree_node = searchpoints.front();
        searchpoints.pop_front();
        // std::cout << "Popping new point. " << std::endl;
        PrintPoint(tree_node);
        auto child_nodes = FindChildNodes(tree_node, height_map, num_rows, num_cols);
        // std::cout << "Found child nodes: " << std::endl;
        for (auto& child : child_nodes) {
            if (searched_map[child.row][child.col] == false) {
                searched_map[child.row][child.col] = true;
                basin.basin_points.push_back(child);
                ++basin.score;
                searchpoints.push_back(child);
                PrintPoint(child);
            }
        }
    }

    return basin;
}

void PrintBasin(const Basin& basin) {
    std::cout << "Low value is " << basin.low_point.value << " at row " << basin.low_point.row << ", col " << basin.low_point.col << std::endl;
    for (auto& child : basin.basin_points) {
        std::cout << "Child value is " << child.value << " at row " << child.row << ", col " << child.col << std::endl;
    }
}

int MultiplyTopThree(std::vector<Basin>& basins) {
    std::priority_queue<int> scores;
    for (auto & basin : basins) {
        scores.push(basin.score);
    }
    int score = scores.top();
    scores.pop();
    score = score * scores.top();
    scores.pop();
    return score * scores.top();
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

    auto low_points = FindLowPoints(heightmap, num_rows, num_cols);
    PrintLowPoints(low_points);

    std::vector<Basin> basins;
    for (auto& low_point : low_points) {
        auto basin = FindBasin(low_point, heightmap, num_rows, num_cols);
        basins.push_back(basin);
        // PrintBasin(basin);
    }

    int score = MultiplyTopThree(basins);
    std::cout << "Final score is " << score << std::endl;

    // Free the memory that's been allocated.
    for (int row = 0; row < num_rows; ++row) {
        delete [] heightmap[row];
    }
    delete [] heightmap;


    return 0;
}