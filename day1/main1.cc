#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string line;
    int number;
    int previous_number = -1;
    int counter = 0;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> number) {
            if (previous_number != -1) {
                if (number > previous_number) {
                    ++counter;
                }
            }
            previous_number = number;
        }
        myfile.close();
    }

    std::cout << "Number of increases: " << counter << std::endl;

    return 0;
}