#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

// Map big digit to number of segments it has.
std::map<int, int> DIGIT_TO_SEGMENT =  {{0, 6}, 
                                        {1, 2}, 
                                        {2, 5}, 
                                        {3, 5}, 
                                        {4, 4}, 
                                        {5, 5}, 
                                        {6, 6}, 
                                        {7, 3}, 
                                        {8, 7}, 
                                        {9, 6}};

constexpr int kNumInputs = 10;
constexpr int kNumOutputs = 4;
constexpr int kNumWordsPerLine = 15;
using Inputs = std::array<std::string, kNumInputs>;
using Outputs = std::array<std::string, kNumOutputs>;

struct Line {
    Inputs inputs;
    Outputs outputs;
};

void PrintLine(Line line) {
    std::cout << "Inputs are " << std::endl;
    for (auto& input: line.inputs) {
        std::cout << input << ", ";
    }
    std::cout << std::endl << "Outputs are " << std::endl;
    for (auto& output: line.outputs) {
        std::cout << output << ", ";
    }
    std::cout << std::endl;
    return;
}

int CountUniqueOutputDigits(std::vector<Line> all_lines) {
    int count = 0;
    for (auto& line : all_lines) {
        for (auto& output : line.outputs) {
            if (output.size() == DIGIT_TO_SEGMENT[1] || 
                output.size() == DIGIT_TO_SEGMENT[4] || 
                output.size() == DIGIT_TO_SEGMENT[7] || 
                output.size() == DIGIT_TO_SEGMENT[8]) {
                    count++;
                }
        }
    }
    return count;
}

int main()
{
    std::string word;
    std::vector<Line> all_lines;
    std::ifstream myfile("input.txt");
    int counter = 0;
    Line line;
    if (myfile.is_open())
    {
        while (myfile >> word)
        {
            std::cout << word << std::endl;
            if (counter < kNumInputs) {
                line.inputs[counter] = word;
            } else if (counter > 10) {
                line.outputs[counter - 11] = word;
            }

            ++counter;

            if (counter == kNumWordsPerLine) {
                counter = 0;
                all_lines.push_back(line);
                // PrintLine(line);
            }
        }
        myfile.close();
    }

    std::cout << "Found " << all_lines.size() << " lines. " << std::endl;

    int num_unique_digits = CountUniqueOutputDigits(all_lines);
    std::cout << "Found " << num_unique_digits << " unique digits in the output." << std::endl;

    return 0;
}