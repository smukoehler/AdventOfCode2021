#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main()
{
    std::string word;
    int position = 0;
    int depth = 0;
    int aim = 0;
    std::string direction;
    int value;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> word) {
            // std::cout << word << " of length: " << word.size() << std::endl;

            if (word.size() > 1) {
                direction = word;
                std::cout << "Next Direction: " << direction << std::endl;
            } else {
                std::stringstream intconverter(word);
                intconverter >> value;
                std::cout << "Next Value: " << value << std::endl;

                if (direction.compare("forward") == 0) {
                    position = position + value;
                    depth = depth + aim * value;
                    std::cout << "Updating position to: " << position << std::endl;
                    std::cout << "Updating depth to: " << depth << std::endl;
                } else if (direction.compare("up") == 0) {
                    aim = aim - value;
                    std::cout << "Updating aim to: " << aim << std::endl;
                } else if (direction.compare("down") == 0) {
                    aim = aim + value;
                    std::cout << "Updating aim to: " << aim << std::endl;
                } else {
                    std::cout << "Warning, found unparseable direction " << direction << std::endl;
                }
            }
        }
        myfile.close();
    }

    std::cout << "Found horizontal position: " << position << std::endl;
    std::cout << "Found depth: " << depth << std::endl;
    std::cout << "Found aim: " << aim << std::endl;

    std::cout << "Multiplying the two values obtains: " << position * depth << std::endl;

    return 0;
}