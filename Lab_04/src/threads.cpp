#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

mutex cout_mutex;

double formula1(double x) {
    return pow(x, 2) - pow(x, 2) + 4*x - 5*x + x + x;
}

double formula2(double x) {
    return x + x;
}

double formula3(double res1, double res2) {
    return res1 + res2 - res1;
}

void compute_formulas_1_2(int start, int end, vector<double>& res1, vector<double>& res2) {
    for (int i = start; i <= end; ++i) {
        int x = rand();
        res1[i] = formula1(i+1);
        res2[i] = formula2(i+1);
    }
}

void compute_formula_3(int start, int end, const vector<double>& res1, const vector<double>& res2, double& time_accum) {
    auto start_time = high_resolution_clock::now();

    for (int i = start; i <= end; ++i) {
        double res = formula3(res1[i], res2[i]);
    }

    auto end_time = high_resolution_clock::now();
    double elapsed = duration_cast<microseconds>(end_time - start_time).count() / 1e6;

    lock_guard<mutex> lock(cout_mutex);
    time_accum += elapsed;
}

void benchmark(int iterations) {
    const int num_threads = thread::hardware_concurrency();
    vector<double> results1(iterations);
    vector<double> results2(iterations);

    cout << fixed << setprecision(7);

    // Общее время начала выполнения
    auto total_start = high_resolution_clock::now();

    // Этап 1: Параллельное вычисление формул 1 и 2
    auto stage1_start = high_resolution_clock::now();
    {
        vector<thread> workers;
        int chunk_size = iterations / num_threads;

        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end;
            if (i == num_threads - 1) {
                end = iterations - 1;
            } else {
                end = start + chunk_size - 1;
            }
            workers.emplace_back(compute_formulas_1_2, start, end, ref(results1), ref(results2));
        }

        for (auto& t : workers) {
            t.join();
        }
    }
    auto stage1_end = high_resolution_clock::now();

    // Этап 2: Параллельное вычисление формулы 3
    double stage2_time = 0;
    auto stage2_start = high_resolution_clock::now();
    {
        vector<thread> workers;
        int chunk_size = iterations / num_threads;

        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end;
            if (i == num_threads - 1) {
                end = iterations - 1;
            } else {
                end = start + chunk_size - 1;
            }
            workers.emplace_back(compute_formula_3, start, end, cref(results1), cref(results2), ref(stage2_time));
        }

        for (auto& t : workers) {
            t.join();
        }
    }
    auto stage2_end = high_resolution_clock::now();

    // Расчет временных показателей
    double total_elapsed = duration_cast<microseconds>(stage2_end - total_start).count() / 1e6;
    double stage1_elapsed = duration_cast<microseconds>(stage1_end - stage1_start).count() / 1e6;
    double stage2_elapsed = duration_cast<microseconds>(stage2_end - stage2_start).count() / 1e6;

    // Вывод результатов
    lock_guard<mutex> lock(cout_mutex);
    cout << "=== Iterations: " << iterations << " ===" << endl;
    cout << "Threads used: " << num_threads << endl;
    cout << "Total time: " << total_elapsed << " sec" << endl;
    cout << "---------------------------------" << endl;
}

int main() {
    benchmark(1000);
    benchmark(10000);
    benchmark(100000);
    benchmark(1000000);

    return 0;
}
