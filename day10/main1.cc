#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

std::map<char, char> OpenToClose = {{'(', ')'},
                                    {'[', ']'},
                                    {'{', '}'},
                                    {'<', '>'}};
std::map<char, char> CloseToOpen = {{')', '('},
                                    {']', '['},
                                    {'}', '{'},
                                    {'>', '<'}};
std::map<char, int> ErrorScore = {{')', 3},
                                  {']', 57},
                                  {'}', 1197},
                                  {'>', 25137}};

bool IsOpener(char bracket)
{
    return bracket == '(' || bracket == '[' || bracket == '{' || bracket == '<';
}

int FindIllegalClosure(std::string line)
{
    std::string openers = "";
    for (auto &bracket : line)
    {
        if (IsOpener(bracket))
        {
            openers = openers + bracket;
        }
        else if (bracket == OpenToClose[openers.back()])
        {
            openers.pop_back();
        }
        else
        {
            std::cout << "Expected " << OpenToClose[openers.back()] << ", but found " << bracket << "instead." << std::endl;
            return ErrorScore[bracket];
        }
    }

    return 0;
}

int main()
{
    std::string line;
    std::vector<std::string> all_lines;
    std::ifstream myfile("input.txt");

    if (myfile.is_open())
    {
        while (myfile >> line)
        {
            // std::cout << line << std::endl;
            all_lines.push_back(line);
        }
        myfile.close();
    }

    int score = 0;
    for (auto& line : all_lines) {
        score += FindIllegalClosure(line);
    }
    std::cout << "Found error score: " << score << std::endl;

    return 0;
}