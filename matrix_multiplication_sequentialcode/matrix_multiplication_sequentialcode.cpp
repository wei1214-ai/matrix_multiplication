#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <chrono>

using real = double;

struct TestDimension { int m; int n; };

int main() {
    std::vector<TestDimension> test_sizes = {
       { 1000, 500 },{ 1500, 750 },{ 2000, 1000 },{ 2500, 1250 },{ 3000, 1500 }
    };

    std::cout << std::left << std::setw(18) << "Matrix Size" << "Time (s)\n";
    std::cout << "--------------------------\n";

    for (const auto& dim : test_sizes) {
        const size_t m = dim.m;
        const size_t n = dim.n;

        // Allocate and initialize before timing
        std::vector<real> A(m * n, 1.5);  // m x n
        std::vector<real> B(n * n, 2.0);  // n x n
        std::vector<real> C(m * n, 0.0);  // m x n

        const real* __restrict a = A.data();
        const real* __restrict b = B.data();
        real* __restrict c = C.data();

        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < m; ++i) {
            for (size_t k = 0; k < n; ++k) {
                const real a_ik = a[i * n + k];
                for (size_t j = 0; j < n; ++j) {
                    c[i * n + j] += a_ik * b[k * n + j];
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double seconds = std::chrono::duration<double>(end - start).count();

        std::string size_str = std::to_string(m) + " x " + std::to_string(n);
        std::cout << std::left << std::setw(18) << size_str
            << std::fixed << std::setprecision(4) << seconds << "\n";

        if (C[0] != n * 3.0) std::cerr << "Result check failed\n";
    }
    return 0;
}