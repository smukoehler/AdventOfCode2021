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

// 1/2 * (x_f - x_i0)^2 + 1/2*|x_f - x_i0| = 1/2 x_f^2 - x_i0*x_f + (drop) + 1/2*|x_f - x_i0|
// d/dx_f = x_f - x_i0 + 1/2*sign(x_f - x_i0)
double ComputeGradient(std::vector<int> list, double x_f) {
    double gradient = 0;
    for (auto& item : list) {
        // Quadratic term and linear term.
        gradient = gradient + x_f - 1.0 * item;
        if (x_f > 1.0 * item) {
            gradient = gradient + 0.5;
        } else if (x_f < 1.0 * item) {
            gradient = gradient - 0.5;
         }
    }
    return gradient;
}

double UpdateIterate(double x_f, int gradient) {
    const double step_size = 0.0001;
    return x_f - step_size * gradient;
}

// 1/2 * (x_f - x_i0)^2 + 1/2*|x_f - x_i0|
double ComputeCost(std::vector<int> list, double x_f) {
    double cost = 0.0;
    for (auto& item : list) {
        double diff = x_f - 1.0 * item;
        cost = cost + 0.5 * diff * diff + 0.5 * fabs(diff);
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
        std::cout.precision(10);
        std::cout << "With cost = " << cost << std::endl;
    }

    double floorposition = floor(x_f);
    double floorcost = ComputeCost(list, floorposition);
    std::cout << "Floor cost: " << floorcost << std::endl;

    double ceilposition = ceil(x_f);
    double ceilcost = ComputeCost(list, ceilposition);
    std::cout << "Ceil cost: " << ceilcost << std::endl;

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

        f(x) = \sum _{I=0}^{x} i = x(x+1)/2 = x^2/2 + x/2

        min sum_{i=0}^{n-1} 1/2 * |u_i|^2 + 1/2 * |u_i|
        s.t. x_i0 + u_i = x_f  , i \in [0, n-1]

        Eliminate u_i

        min sum_{i=0}^{n-1} 1/2 * (x_f - x_i0)^2 + 1/2*|x_f - x_i0|
    */
   
   // Try a gradient descent.
   int x_f = GradientDescent(initial_positions);

    return 0;
}