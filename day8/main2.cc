#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <vector>


/***** DATA STRUCTURES AND PRINT HELPERS ****/
// Map big digit to number of segments it has.
std::map<int, int> DIGIT_TO_SEGMENT = {{0, 6},
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

struct Line
{
    Inputs inputs;
    Outputs outputs;
};

void PrintLine(Line line)
{
    std::cout << "Inputs are " << std::endl;
    for (auto &input : line.inputs)
    {
        std::cout << input << ", ";
    }
    std::cout << std::endl
              << "Outputs are " << std::endl;
    for (auto &output : line.outputs)
    {
        std::cout << output << ", ";
    }
    std::cout << std::endl;
    return;
}

// Map each scrambled letter to possible correct letters.
using WireMap = std::map<char, std::vector<char>>;
std::vector<char> kDefaultCharacters = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
const WireMap kDefaultWireMap = {{'a', kDefaultCharacters},
                                 {'b', kDefaultCharacters},
                                 {'c', kDefaultCharacters},
                                 {'d', kDefaultCharacters},
                                 {'e', kDefaultCharacters},
                                 {'f', kDefaultCharacters},
                                 {'g', kDefaultCharacters}};

// Map each scrambled letter to possible correct letters.
using NumberToCharacters = std::map<int, std::vector<char>>;
const NumberToCharacters kDefaultNumberToCharacters = {{0, {}},
                                                       {1, {}},
                                                       {2, {}},
                                                       {3, {}},
                                                       {4, {}},
                                                       {5, {}},
                                                       {6, {}},
                                                       {7, {}},
                                                       {8, {}},
                                                       {9, {}}};

void PopCharacter(std::vector<char> &char_list, char remove_me)
{
    char_list.erase(std::remove(char_list.begin(), char_list.end(), remove_me), char_list.end());
    return;
}

void PrintCharacterList(const std::vector<char> &char_list)
{
    for (auto &character : char_list)
    {
        std::cout << character << ",";
    }
    std::cout << std::endl;
    return;
}

void PrintWireMap(const WireMap &wire_map)
{
    for (auto &element : wire_map)
    {
        std::cout << element.first << " : ";
        PrintCharacterList(element.second);
    }
}

void PrintNumberToCharactersMap(const NumberToCharacters& number_to_characters)
{
    for (auto &element : number_to_characters)
    {
        std::cout << element.first << " : ";
        PrintCharacterList(element.second);
    }
}

/***** LOGIC ****/

std::vector<std::string> FindUniqueDigits(const Line &line)
{
    std::vector<std::string> unique_digits;
    for (auto &input : line.inputs)
    {
        if (input.size() == DIGIT_TO_SEGMENT[1] ||
            input.size() == DIGIT_TO_SEGMENT[4] ||
            input.size() == DIGIT_TO_SEGMENT[7] ||
            input.size() == DIGIT_TO_SEGMENT[8])
        {
            unique_digits.push_back(input);
        }
    }
    return unique_digits;
}


void ReduceMapping(const std::vector<std::string> &unique_digits, WireMap &wire_map, NumberToCharacters &number_to_characters)
{
    for (auto &word : unique_digits)
    {
        if (word.size() == DIGIT_TO_SEGMENT[1])
        {
            // There are two segments for digit 1: c, f.
            // Remove any digits that are not c or f from the digits in the word.
            for (auto character : word)
            {
                std::cout << character << " is part of digit 1" << std::endl;
                PopCharacter(wire_map[character], 'a');
                PopCharacter(wire_map[character], 'b');
                PopCharacter(wire_map[character], 'd');
                PopCharacter(wire_map[character], 'e');
                PopCharacter(wire_map[character], 'g');
                number_to_characters[1].push_back(character);
            }
        }
        else if (word.size() == DIGIT_TO_SEGMENT[4])
        {
            // There are two segments for digit 4: b, c, d, f.
            // Remove any digits that are not b, c, d, f from the digits in the word.
            for (auto character : word)
            {
                std::cout << character << " is part of digit 4" << std::endl;
                PopCharacter(wire_map[character], 'a');
                PopCharacter(wire_map[character], 'e');
                PopCharacter(wire_map[character], 'g');
                number_to_characters[4].push_back(character);
            }
        }
        else if (word.size() == DIGIT_TO_SEGMENT[7])
        {
            // There are two segments for digit 7: a, c, f.
            // Remove any digits that are not a, c, f from the digits in the word.
            for (auto character : word)
            {
                std::cout << character << " is part of digit 7" << std::endl;
                PopCharacter(wire_map[character], 'b');
                PopCharacter(wire_map[character], 'd');
                PopCharacter(wire_map[character], 'e');
                PopCharacter(wire_map[character], 'g');
                number_to_characters[7].push_back(character);
            }
        }
        else if (word.size() == DIGIT_TO_SEGMENT[8])
        {
            // 8 is actually useless because it is all the segments.
        }
    }
    return;
}

void GetMapToA(NumberToCharacters &number_to_characters, WireMap &wire_map)
{
    std::vector<char> character_list_7 = number_to_characters[7];
    for (auto character : number_to_characters[1]) {
        PopCharacter(character_list_7, character);
    }
    // Set this mapping.
    char scrambled_letter_for_a = character_list_7[0];
    wire_map[scrambled_letter_for_a] = {'a'};

    // Delete a as an option from all other letters.
    for (auto element : wire_map) {
        if (element.first != scrambled_letter_for_a) {
            PopCharacter(wire_map[element.first], 'a');
        }
    }
}

int DecodeLine(Line line)
{
    int answer = 0;

    PrintLine(line);
    std::vector<std::string> unique_digits = FindUniqueDigits(line);
    WireMap wire_map = kDefaultWireMap;
    NumberToCharacters number_to_characters = kDefaultNumberToCharacters;
    ReduceMapping(unique_digits, wire_map, number_to_characters);
    PrintWireMap(wire_map);
    PrintNumberToCharactersMap(number_to_characters);
    GetMapToA(number_to_characters, wire_map);
    PrintWireMap(wire_map);


    return answer;
}

/************************ MAIN ************************/

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
            // std::cout << word << std::endl;
            if (counter < kNumInputs)
            {
                line.inputs[counter] = word;
            }
            else if (counter > 10)
            {
                line.outputs[counter - 11] = word;
            }

            ++counter;

            if (counter == kNumWordsPerLine)
            {
                counter = 0;
                all_lines.push_back(line);
                // PrintLine(line);
            }
        }
        myfile.close();
    }

    int line0_answer = DecodeLine(all_lines[0]);
    std::cout << "Answer for line 0: " << line0_answer << std::endl;

    // std::cout << "arbitrary experiment" << std::endl;
    // std::vector<char> arbitrary = {'a', 'r', 'b', 'i'};
    // PrintCharacterList(arbitrary);
    // PopCharacter(arbitrary, 'a');
    // PrintCharacterList(arbitrary);

    return 0;
}