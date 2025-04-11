#include <iostream>
#include <chrono>
#include <string>
#include <cctype>
#include <cstdlib>

double iteration(double x) {
    return x*x - x*x + x*4 - x*5 + x + x;
}

void run_experiment(int n, int x) {
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i) {
        iteration(x);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sum_time = end_time - start_time;

    std::cout << "Time of " << n << "iterations: " << sum_time.count() << std::endl;
}

bool is_number(std::string s) {

    for (int i = 0; i < s.length(); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    while (true) {
        std::cout << "Enter number of iterations (or non-number to exit): ";
        std::string input;
        std::cin >> input;

        if (!is_number(input)) {
            std::cout << "Program completed" << std::endl;
            break;
        }

        int n = std::stoi(input);

        int x = rand();
        run_experiment(n, x);
    }

    return 0;
}
