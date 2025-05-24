#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>

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

void child_work(int start, int end) {
    for (int i = start; i <= end; ++i) {
        double res1 = formula1(i);
        double res2 = formula2(i);
        double res3 = formula3(res1, res2);
    }
}

void calculate(int iterations) {
    const int num_processes = sysconf(_SC_NPROCESSORS_ONLN);
    vector<pid_t> pids(num_processes);

    auto start_time = steady_clock::now();

    int chunk = iterations / num_processes;
    for (int i = 0; i < num_processes; ++i) {
        int start = i * chunk + 1;
        int end;
        if (i == num_processes - 1) {
            end = iterations;
        } else {
            end = (i + 1) * chunk;
        }

        pid_t pid = fork();
        if (pid == 0) {
            child_work(start, end);
            exit(0);
        }
        pids[i] = pid;
    }

    for (pid_t pid : pids) {
        waitpid(pid, nullptr, 0);
    }

    double total_time = duration_cast<microseconds>(steady_clock::now() - start_time).count() / 1e6;

    cout << fixed << setprecision(6);
    cout << "=== Iterations: " << iterations << " ===" << endl;
    cout << "Processes used: " << num_processes << endl;
    cout << "Total execution time: " << total_time << " seconds" << endl;
    cout << "---------------------------------" << endl;
}

int main() {
    calculate(1000);
    calculate(10000);
    calculate(100000);
    calculate(1000000);
    
    return 0;
}