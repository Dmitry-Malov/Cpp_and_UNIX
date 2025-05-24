#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;


struct ProcessResults {
    double time1;
    double time2;
    double time3;
};

double formula1(int x) {
    return pow(x, 2) - pow(x, 2) +4*x  - 5*x + x + x;
}

double formula2(int x) {
    return x + x;
}

double formula3(double res1, double res2) {
    return res1 + res2 - res1;
}

void child_work(int start, int end, ProcessResults* results) {

    auto t1_start = steady_clock::now();
    for (int i = start; i <= end; ++i) {
        volatile double res = formula1(i);
        (void)res;
    }
    results->time1 = duration_cast<microseconds>(steady_clock::now() - t1_start).count() / 1e6;

    auto t2_start = steady_clock::now();
    for (int i = start; i <= end; ++i) {
        volatile double res = formula2(i);
        (void)res;
    }
    results->time2 = duration_cast<microseconds>(steady_clock::now() - t2_start).count() / 1e6;

    auto t3_start = steady_clock::now();
    for (int i = start; i <= end; ++i) {
        volatile double res = formula3(formula1(i), formula2(i));
        (void)res;
    }
    results->time3 = duration_cast<microseconds>(steady_clock::now() - t3_start).count() / 1e6;
}

void calculate(int iterations) {
    const int num_processes = sysconf(_SC_NPROCESSORS_ONLN);
    vector<ProcessResults> results(num_processes);
    vector<pid_t> pids(num_processes);

    auto total_start = steady_clock::now();

    int chunk = iterations / num_processes;
    for (int i = 0; i < num_processes; ++i) {
        int start = i * chunk + 1;
        int end = (i == num_processes - 1) ? iterations : (i + 1) * chunk;

        pid_t pid = fork();
        if (pid == 0) {
            child_work(start, end, &results[i]);
            exit(0);
        }
        pids[i] = pid;
    }

    for (pid_t pid : pids) {
        waitpid(pid, nullptr, 0);
    }

    double total_time = duration_cast<microseconds>(steady_clock::now() - total_start).count() / 1e6;

    double sum1 = 0, sum2 = 0, sum3 = 0;
    for (const auto& res : results) {
        sum1 += res.time1;
        sum2 += res.time2;
        sum3 += res.time3;
    }

    cout << fixed << setprecision(6);
    cout << "=== Iterations: " << iterations << " ===" << endl;
    cout << "Processes: " << num_processes << endl;
    cout << "Total time: " << total_time << " sec" << endl;
    cout << "Formula1 total: " << sum1 << " sec" << endl;
    cout << "Formula2 total: " << sum2 << " sec" << endl;
    cout << "Formula3 total: " << sum3 << " sec" << endl;
    cout << "---------------------------------" << endl;
}

int main() {
    calculate(1000);
    calculate(10000);
    calculate(100000);
    calculate(1000000);
    return 0;
}
