#include <iostream>
#include <fstream>
#include <map>
#include <queue>
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
std::map<char, int> CharacterScore = {{')', 1},
                                      {']', 2},
                                      {'}', 3},
                                      {'>', 4}};

bool IsOpener(char bracket)
{
    return bracket == '(' || bracket == '[' || bracket == '{' || bracket == '<';
}

bool IsIllegalClosure(std::string line)
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
            return true;
        }
    }

    return false;
}

std::string CompleteTheLine(const std::string &line)
{
    std::string completion;

    std::string openers = "";
    for (auto &bracket : line)
    {
        // Check if its an opening bracket.
        if (IsOpener(bracket))
        {
            openers = openers + bracket;
        }

        // It's a closing bracket so this opener is taken care of.
        if (bracket == OpenToClose[openers.back()])
        {
            openers.pop_back();
        }
    }

    // Invert the remaining openers.
    for (auto &bracket : openers)
    {
        completion = OpenToClose[bracket] + completion;
    }

    return completion;
}

std::vector<std::string> FindCompletionCharacters(std::vector<std::string> incomplete_lines)
{
    std::vector<std::string> completion_lines;
    for (auto &line : incomplete_lines)
    {
        std::string completion_chars = CompleteTheLine(line);
        completion_lines.push_back(completion_chars);
    }
    return completion_lines;
}

long UpdateScore(const std::string &completion)
{
    long score = 0;
    for (auto &bracket : completion)
    {
        score = score * 5 + CharacterScore[bracket];
    }

    return score;
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

    std::vector<std::string> incomplete_lines;
    std::cout << "Incomplete lines. " << std::endl;
    for (auto &line : all_lines)
    {
        if (!IsIllegalClosure(line))
        {
            incomplete_lines.push_back(line);
            std::cout << line << std::endl;
        }
    }

    std::vector<std::string> completion_lines = FindCompletionCharacters(incomplete_lines);
    std::priority_queue<long> scores;
    std::cout << "Completion lines. " << std::endl;
    for (auto &completion : completion_lines)
    {
        long score = UpdateScore(completion);
        scores.push(score);
        std::cout << completion << ", " << score << std::endl;
    }

    int middle_index = scores.size() / 2;
    std::cout << "middle index: " << middle_index << std::endl;
    int i = 0;
    while (i < middle_index) {
        scores.pop();
        ++i;
    }
    std::cout << "Middle score: " << scores.top() << std::endl;

    return 0;
}