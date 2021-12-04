#include <iostream>
#include <fstream>
#include <deque>
#include <string>

int main()
{
    std::string line;
    int number;
    int counter = 0;
    std::deque<int> numbers;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> number)
        {
            if (numbers.size() == 4)
            {
                numbers.pop_front();
            }
            numbers.push_back(number);

            std::cout << "Numbers deque " << std::endl;
            for (int n : numbers)
            {
                std::cout << n << ", ";
            }
            std::cout << std::endl;

            if (numbers.size() == 4)
            {
                auto sum1 = numbers[0] + numbers[1] + numbers[2];
                auto sum2 = numbers[1] + numbers[2] + numbers[3];

                std::cout << "sum1: " << sum1 << std::endl;
                std::cout << "sum2: " << sum2 << std::endl;

                if (sum2 > sum1)
                {
                    ++counter;
                }
            }
        }
        myfile.close();
    }

    std::cout << "Number of increases: " << counter << std::endl;
    return 0;
}