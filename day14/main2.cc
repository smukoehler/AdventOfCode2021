#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>


// Map (pair, insertchar)
using Rules = std::map<std::string, std::string>;

// (pair, number of occurences)
using Pairs = std::map<std::string, long>;

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

Pairs Pairify(const std::string& text) {
    Pairs pairs;
    int num_chars = text.size();
    for (int i = 0; i < num_chars - 1; ++i) {
        std::string pair = text.substr(i, 2);
        if (pairs.find(pair) == pairs.end()) {
            pairs.insert({pair, 1});
        } else {
            ++pairs[pair];
        }
    }
    return pairs;
}

void PrintPairs(Pairs pairs) {
    for (auto& pair : pairs) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
}

long CountPairs(Pairs& pairs) {
    long count = 0;
    for (auto& pair : pairs) {
        count += pair.second;
    }
    return count;
}

Pairs Step(Pairs& pairs, Rules& rules) {
    Pairs new_pairs;

    long num_pairs = CountPairs(pairs);
    for (auto& pair : pairs) {
        // std::cout << "Found pair " << pair.first << std::endl;
        long paircount = pair.second;
        std::string insertchar = rules[pair.first];
        std::string firstnewpair = pair.first[0] + insertchar;
        std::string secondnewpair = insertchar + pair.first[1];
        // std::cout << "Creates pairs " << firstnewpair << "," << secondnewpair << std::endl;

        if (new_pairs.find(firstnewpair) == new_pairs.end()) {
            new_pairs.insert({firstnewpair, paircount});
        } else {
            new_pairs[firstnewpair] += paircount;
        }

        if (new_pairs.find(secondnewpair) == new_pairs.end()) {
            new_pairs.insert({secondnewpair, paircount});
        } else {
            new_pairs[secondnewpair] += paircount;
        }

    }
    return new_pairs;
}

long ComputeScore(Pairs& pairs, std::string starting_template) {
    std::map<char, long> letter_scores;
    for (auto& pair : pairs) {
        char first_letter = pair.first[0];
        long score = pair.second;
        if (letter_scores.find(first_letter) == letter_scores.end()) {
            letter_scores.insert({first_letter, score});
        } else {
            letter_scores[first_letter] += score;
        }
    }

    char last_letter = starting_template[starting_template.size() - 1];
    letter_scores[last_letter]++;


    long min_score = letter_scores.begin()->second;
    char min_char = letter_scores.begin()->first;
    long max_score = min_score;
    char max_char = min_char;
    for (auto& letterscore : letter_scores) {
        if (letterscore.second > max_score) {
            max_score = letterscore.second;
            max_char = letterscore.first;
        }
        if (letterscore.second < min_score) {
            min_score = letterscore.second;
            min_char = letterscore.first;
        }
    }

    std::cout << "letter scores " << std::endl;
    for (auto & score : letter_scores) {
        std::cout << score.first << " : " << score.second << std::endl;
    }

    std::cout << "Least character: " << min_char << " with score " << min_score <<  std::endl;
    std::cout << "Most character: " << max_char << " with score " << max_score <<  std::endl;

    return max_score - min_score;
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

    std::cout << "starting_template: " << starting_template << std::endl;
    Pairs initial_pairs = Pairify(starting_template);
    PrintPairs(initial_pairs);
    std::cout << "num initial pairs :" << CountPairs(initial_pairs) << std::endl;
    Pairs pairs = initial_pairs;
    for (int step = 0; step < 40; ++step) {
        pairs = Step(pairs, rules);
        // std::cout << "output after " << step << " steps:" << std::endl;
        // PrintPairs(pairs);
        std::cout << "num pairs after " << step << " steps:" << CountPairs(pairs) << std::endl;
    }
    // PrintPairs(pairs);

    long score = ComputeScore(pairs, starting_template);
    std::cout << "Computed score: " << score << std::endl;

    return 0;
}