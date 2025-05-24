#include <iostream>
#include <cmath>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

double formula1(double x) {
    return pow(x, 2) - pow(x, 2) + 4*x - 5*x + x + x;
}

double formula2(double x) {
    return x + x;
}

double formula3(double res1, double res2) {
    return res1 + res2 - res1;
    }

void calculate(int iterations) {
    auto start = high_resolution_clock::now();

    for (int i = 1; i <= iterations; ++i) {
        int x = rand();
        double res1 = formula1(x);
        double res2 = formula2(x);
        double res3 = formula3(res1, res2);
    }

    auto stop = high_resolution_clock::now();
    duration<double> sum_time = stop - start;

    cout << "Iterations: " << iterations << ", Time: "
         << sum_time.count() << " seconds" << endl;
}

int main() {
    calculate(10000);
    calculate(100000);
    return 0;
}
