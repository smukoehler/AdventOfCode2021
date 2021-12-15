#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>


// Map (pair, insertchar)
using Rules = std::map<std::string, std::string>;

Rules ParseLines(std::vector<std::string> all_words) {
    Rules rules;
    int num_words = all_words.size();

    std::string pair;
    std::string insertchar;
    for (int i = 0; i < num_words; ++i) {
        if (i % 3 == 1) {
            pair = all_words[i];
        }
        if (i % 3 == 0 && i != 0) {
            insertchar = all_words[i];
            rules.insert({pair, insertchar});
        }
    }
    return rules;
}

void PrintRules(Rules rules) {
    for (auto & rule : rules) {
        std::cout << rule.first << " -> " << rule.second << std::endl;
    }
    return;
}

std::string Step(const std::string& starting_template, Rules& rules) {
    std::string output = "";

    int length = starting_template.size();
    for (int i = 0; i < length - 1; ++i) {
        std::string pair = starting_template.substr(i, 2);
        std::string insertchar = rules[pair];

        if (i ==0) {
            output = pair[0] + insertchar + pair[1];
        } else {
            output = output + insertchar + pair[1];
        }
    }
    return output;
}

int ComputeScore(std::string output) {
    std::map<int, char> letter_scores;
    std::map<char, int> reverse_letter_scores;
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        int score = std::count(output.begin(), output.end(), letter);
        if (score > 0) {
            letter_scores.insert({score, letter});
            reverse_letter_scores.insert({letter, score});
        }
    }
    std::cout << "Least character: " << letter_scores.begin()->second << std::endl;
    std::cout << "Most character: " << letter_scores.rbegin()->second << std::endl;

    for (auto& item : reverse_letter_scores) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    return letter_scores.rbegin()->first - letter_scores.begin()->first;
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

    std::string starting_template = all_lines[0];
    Rules rules = ParseLines(all_lines);
    // PrintRules(rules);

    std::string output = starting_template;
    std::cout << "starting template: " << starting_template << std::endl;
    std::cout << "starting template length: " << starting_template.size() << std::endl;
    for (int step = 0; step < 10; ++step) {
        output = Step(output, rules);
        std::cout << "output after " << step << " steps: " << output << std::endl;
        std::cout << "length after " << step << " steps: " << output.size() << std::endl;
    }

    int score = ComputeScore(output);
    std::cout << "Computed score: " << score << std::endl;

    return 0;
}