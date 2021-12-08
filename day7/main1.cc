#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> GetInitialPositions(std::string longlist){
    std::vector<int> list;
    std::stringstream liststream(longlist); //create string stream from the string
    while (liststream.good())
    {
        std::string number;
        std::getline(liststream, number, ','); //get first string delimited by comma
        list.push_back(std::stoi(number));
    }

    return list;
}

void PrintPositions(std::vector<int> list) {
  for (auto& item : list) {
      std::cout << item << ", ";
  }
  std::cout << std::endl;
}

int ComputeGradient(std::vector<int> list, double x_f) {
    int gradient = 0;
    for (auto& item : list) {
        if (x_f > 1.0 * item) {
            ++gradient;
        } else if (x_f < 1.0 * item) {
            --gradient;
         }
    }
    return gradient;
}

double UpdateIterate(double x_f, int gradient) {
    const double step_size = 0.01;
    return x_f - step_size * gradient;
}

double ComputeCost(std::vector<int> list, double x_f) {
    double cost = 0.0;
    for (auto& item : list) {
        cost = cost + fabs(x_f - 1.0 * item);
    }
    return cost;
}

int GradientDescent(std::vector<int> list) {
    double x_f = 0.0;
    double gradient = 0;
    for (int i = 0; i < 1000; ++i) {
        gradient = ComputeGradient(list, x_f);

        std::cout << "gradient is " << gradient << std::endl;
        x_f = UpdateIterate(x_f, gradient);

        std::cout << "Next x_f is " << x_f << std::endl;
        
        double cost = ComputeCost(list, x_f);
        std::cout << "With cost = " << cost << std::endl;
    }

    return x_f;
}

int main()
{
    std::string line;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> line) {
            // std::cout << line << std::endl;
        }
        myfile.close();
    }

    std::vector<int> initial_positions = GetInitialPositions(line);

    std::cout << "Printing positions" << std::endl;
    PrintPositions(initial_positions);

    /*  Optimization problem.
        Let there by n crabs.

        min sum_{i=0}^{n-1} |u_i|
        s.t. x_i0 + u_i = x_f  , i \in [0, n-1]
        x_f >= 0

        Matrix form of x_i0 + u_i = x_f  , i \in [0, n-1]
        x_f * 1 - I u_i = x_i0   (where 1 is a vector of 1s)

        Eliminate u_i

        min sum_{i=0}^{n-1} |x_f - x_i0|
        s.t. x_f >= 0
    */
   
   // Try a gradient descent.
   int x_f = GradientDescent(initial_positions);

    return 0;
}