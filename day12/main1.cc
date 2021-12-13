#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using AdjacencyMap = std::map<std::string,std::vector<std::string>>;

AdjacencyMap GetMap(const std::vector<std::string>& all_lines) {
    AdjacencyMap adjacency;

    for (auto& line : all_lines) {
        std::stringstream linestream(line); //create string stream from the string
        std::vector<std::string> node_pair;
        while (linestream.good())
        {
            std::string node;
            std::getline(linestream, node, '-'); //get first string delimited by dash
            node_pair.push_back(node);
        }

        // We should've gottan two nodes to add to our adjacency list.
        if (adjacency.find(node_pair[0]) == adjacency.end()) {
            // std::cout << "Adding " << node_pair[0] << " to keys with element " << node_pair[1] << std::endl;
            adjacency.insert({node_pair[0], {node_pair[1]}});
        } else {
            // std::cout << "Adding edge " << node_pair[0] << "-" << node_pair[1] << std::endl;
            adjacency[node_pair[0]].push_back(node_pair[1]);
        }
        if (adjacency.find(node_pair[1]) == adjacency.end()) {
            // std::cout << "Adding " << node_pair[1] << " to keys with element " << node_pair[0] << std::endl;
            adjacency.insert({node_pair[1], {node_pair[0]}});
        } else {
            // std::cout << "Adding edge " << node_pair[1] << "-" << node_pair[0] << std::endl;
            adjacency[node_pair[1]].push_back(node_pair[0]);
        }
    }

    return adjacency;
}

void PrintMap(const AdjacencyMap& map) {
    for (const auto& element: map) {
        std::cout << element.first << " : "; 
        for (const auto& edge : element.second) {
            std::cout << edge << ", ";
        }
        std::cout << std::endl;
    }
}

using Path = std::vector<std::string>;

bool IsUpper(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isupper(c); });
}

bool IsLegal(const Path& path, const std::string& child) {
    // Upper case is always legal.
    if (IsUpper(child)) {
        return true;
    } else {
        auto it = std::find(path.begin(), path.end(), child);
        // Legal if the child is not in the vector.
        if (it == path.end()) {
            return true;
        }
    }
    return false;
}

bool PathEnds(Path path) {
    return (path.back().compare("end") == 0)  || (path.back().compare("deadend") == 0);
}
 
bool AllPathsEnd(std::vector<Path> paths) {
    bool all_paths_end = true;

    for (auto& path : paths) {
        all_paths_end = all_paths_end && PathEnds(path);
    }

    return all_paths_end;
}

// Path DepthFirstSearch(const std::string& parent, AdjacencyMap& adjacency, const Path& path) {
//     if (parent.compare("end") == 0) {
//         return path;
//     }
//     // Explore all the children.
//     for (auto& child : adjacency[parent]) {
//         if (IsLegal(path, child)){
//             auto new_path = path;
//             new_path.push_back(child);
//             return DepthFirstSearch(child, adjacency, new_path);
//         }
//     }
// }
void PrintPaths(std::vector<Path> paths) {
    for (auto& path: paths) {
        for (auto& element : path) {
            std::cout << element << ",";
        }
        std::cout << std::endl;
    }
    return;
}

void PrintLegitPaths(std::vector<Path> paths) {
    for (auto& path: paths) {
        if (path.back().compare("end") == 0) {
            for (auto& element : path) {
                std::cout << element << ",";
            }
            std::cout << std::endl;
        }
    }
    return;
}

int CountLegitPaths(std::vector<Path> paths) {
    int count = 0;
    for (auto& path: paths) {
        if (path.back().compare("end") == 0) {
            ++count;
        }
    }
    return count;
}

std::vector<Path> GetPaths(AdjacencyMap& adjacency) {
    std::vector<Path> paths;

    std::string parent = "start";
    Path path0;
    path0.push_back(parent);
    paths.push_back(path0);
    // std::cout << "Path 0 " << std::endl;
    // PrintPaths(paths);
    int iter = 0;
    while (!AllPathsEnd(paths)) {
        std::vector<Path> new_paths;
        for (auto& path : paths) {
            if (PathEnds(path)) {
                new_paths.push_back(path);
            } else {
                parent = path.back();
                for (auto& child : adjacency[parent]) {
                    if (IsLegal(path, child)) {
                        auto new_path = path;
                        new_path.push_back(child);
                        new_paths.push_back(new_path);
                    } else {
                        auto new_path = path;
                        new_path.push_back("deadend");
                        new_paths.push_back(new_path);
                    }
                }
            }
        }
        // std::cout << "Iteration " << iter << std::endl;
        // PrintPaths(new_paths);
        paths = new_paths;
        ++iter;

        // if (iter == 10) {
        //     return paths;
        // }
    }
    return paths;
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

    AdjacencyMap adjacency = GetMap(all_lines);
    PrintMap(adjacency);

    auto paths = GetPaths(adjacency);
    PrintLegitPaths(paths);
    int count = CountLegitPaths(paths);

    std::cout << "Counted paths: " << count << std::endl;

    return 0;
}