#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int numdigits = 12;

std::vector<std::bitset<numdigits>> FilterList(const std::vector<std::bitset<numdigits>> &input_list, int bit, int digit)
{
    std::vector<std::bitset<numdigits>> output_list;

    for (auto &bnumber : input_list)
    {
        if (bnumber[digit] == bit)
        {
            output_list.push_back(bnumber);

        }
    }

    return output_list;
}

int FindOxygenRating(const std::vector<std::bitset<numdigits>> &bnumber_list)
{
    // Make a copy of the list.
    auto filtered_list = bnumber_list;

    // Start from digit 11 as the "first" bit.
    for (int digit = numdigits - 1; digit >= 0; --digit)
    {
        // Determine the most common bit.
        int threshold = filtered_list.size() / 2 + filtered_list.size() % 2;
        int common_bit = 0;
        int bit_sum = 0; // Total up the bits in this digit location.
        for (auto &bnumber : filtered_list)
        {
            if (bnumber[digit] == 1) {
                ++bit_sum;
            }
        }
        std::cout << "Computed sum of " << bit_sum << " in digit " << numdigits - digit << std::endl;
        std::cout << "Threshold is " << threshold << std::endl;
        if (bit_sum >= threshold)
        {
            common_bit = 1;
        }
        std::cout << "Common bit is " << common_bit << std::endl;

        // Filter out the list for the next iteration.
        filtered_list = FilterList(filtered_list, common_bit, digit);
        std::cout << "Length of filtered list is: " << filtered_list.size() << std::endl;
        std::cout << "First item is: " << filtered_list[0] << std::endl;

        if (filtered_list.size() == 1)
        {
            std::cout << "Found oxygen rating of: " << filtered_list[0] << std::endl;
            std::cout << "Returning in decimal: " << filtered_list[0].to_ulong() << std::endl << std::endl;
            return filtered_list[0].to_ulong();
        }
    }

    std::cout << "Something went wrong because the list is not size 1. " << std::endl;
    return 0;
}

int FindCO2Rating(const std::vector<std::bitset<numdigits>> &bnumber_list)
{
    int threshold = bnumber_list.size() / 2;

    // Make a copy of the list.
    auto filtered_list = bnumber_list;

    // Start from digit 11 as the "first" bit.
    for (int digit = numdigits - 1; digit >= 0; --digit)
    {
        // Determine the least common bit.
        int threshold = filtered_list.size() / 2 + filtered_list.size() % 2;
        int least_common_bit = 0;
        int bit_sum = 0; // Total up the bits in this digit location.
        for (auto &bnumber : filtered_list)
        {
            if (bnumber[digit] == 1) {
                ++bit_sum;
            }
        }
        
        if (bit_sum < threshold)
        {
            least_common_bit = 1;
        }
        std::cout << "Least common bit is " << least_common_bit << std::endl;

        // Filter out the list for the next iteration.
        filtered_list = FilterList(filtered_list, least_common_bit, digit);
        std::cout << "Length of filtered list is: " << filtered_list.size() << std::endl;
        std::cout << "First item is: " << filtered_list[0] << std::endl;

        if (filtered_list.size() == 1)
        {
            std::cout << "Found co2 rating of: " << filtered_list[0] << std::endl;
            std::cout << "Returning in decimal: " << filtered_list[0].to_ulong() << std::endl;
            return filtered_list[0].to_ulong();
        }
    }

    std::cout << "Something went wrong because the list is not size 1. " << std::endl;
    return 0;
}

int main()
{
    int num_lines = 0;
    std::string word;
    std::vector<std::bitset<numdigits>> bnumber_list;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> word)
        {
            std::cout << word << std::endl;
            std::bitset<numdigits> bnumber = std::bitset<numdigits>(word);
            bnumber_list.push_back(bnumber);

            // Count the number of lines
            ++num_lines;
        }
        myfile.close();
    }

    int oxygen = FindOxygenRating(bnumber_list);
    int co2 = FindCO2Rating(bnumber_list);

    std::cout << "Life support rating is: " << oxygen * co2 << std::endl;

    return 0;
}