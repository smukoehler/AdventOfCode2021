#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string word;
    const int numdigits = 12;
    std::bitset<numdigits> bnumber;
    std::array<int, numdigits> bit_sums = {};
    int num_lines = 0;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> word)
        {
            std::cout << word << std::endl;
            bnumber = std::bitset<numdigits>(word);
            std::cout << "bitset " << bnumber << std::endl;
            std::cout << "The 0 bit is " << bnumber[0] << std::endl;

            // Count the number of lines
            ++num_lines;

            // Count the number of 1s in each bit.
            for (int i = 0; i < numdigits; ++i)
            {
                if (bnumber[i] == 1)
                    ++bit_sums[i];
            }
        }
        myfile.close();
    }

    std::cout << "number of lines: " << num_lines << std::endl;
    std::cout << "half the number of lines: " << num_lines / 2 << std::endl;

    // Construct the bitset based on the most common bit.
    std::bitset<numdigits> gamma;
    for (int i = 0; i < numdigits; ++i)
    {
        gamma[i] = 0;
        if (bit_sums[i] > num_lines / 2) {
            gamma[i] = 1;

            std::cout << "bit " << i << " has " << bit_sums[i] << " 1s." << std::endl;
        }
    }

    std::bitset<numdigits> epsilon = gamma;
    epsilon.flip();

    std::cout << "gamma bitset: " << gamma << std::endl;
    std::cout << "epsilon bitset: " << epsilon << std::endl;

    std::cout << "gamma integer: " << gamma.to_ulong() << std::endl;
    std::cout << "epsilon integer: " << epsilon.to_ulong() << std::endl;

    int power = gamma.to_ulong() * epsilon.to_ulong();
    std::cout << "power: " << power << std::endl;

    return 0;
}