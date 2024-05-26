#include "calculate.h"

double calculate_power(double acc, double c) {
    double t = 0.0;

    if (acc < 90) 
        t = 0.0;
    else if (acc <= 94.5) 
        t = (24.0 / 135.0) * std::exp(8.0 / 9.0 * (acc - 95)) + 0.125;
    else if (acc <= 95) 
        t = calculate_power(94.5, 1) + (calculate_power(95.5, 1) - calculate_power(94.5, 1)) * (acc - 94.5) * (acc - 94.5) * (acc - 95.5) / 30.0;
    else if (acc <= 95.5) 
        t = (24.0 / 135.0) * std::exp(2.0 / 3.0 * (acc - 95)) + 0.125;
    else if (acc < 96) 
        t = calculate_power(96, 1) + 2 * (calculate_power(96.5, 1) - calculate_power(96, 1)) * (acc - 96);
    else if (acc < 97.5) 
        t = (80.0 / 297.0) * (std::log(acc - 95) - std::log(5)) + 10.0 / 11.0 * (3 * acc - 250.5) / 40.0;
    else if (acc < 98) 
        t = (80.0 / 297.0) * (std::log(acc - 95) - std::log(5)) + 10.0 / 11.0 * (acc - 76.5) / 20.0;
    else if (acc < 98.5) 
        t = (80.0 / 297.0) * (std::log(acc - 95) - std::log(5)) + 10.0 / 11.0 * (3 * acc - 186.5) / 100.0;
    else if (acc < 99) 
        t = (80.0 / 297.0) * (std::log(acc - 95) - std::log(5)) + 10.0 / 11.0 * (acc - 44) / 50.0;
    else if (acc < 100) 
        t = 8.0 / 27.0 * (std::log(acc - 95) - std::log(5)) + 1;
    else if (acc == 100) 
        t = 1;
    else 
        t = 0;
    
    return t * c;
}
