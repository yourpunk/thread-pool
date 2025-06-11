// ------------------------------------------------------------
// MAIN TEST FILE
//
// This program benchmarks different methods for summing vectors
// using several predefined datasets with varying characteristics.
// Results are shown in a table for comparison.
//
// Datasets test performance under different conditions:
// - Few very long vectors
// - Mixed long and short vectors
// - Many very short vectors
// - Small input unsuitable for parallelism
//
// Each method (sequential, vector-level parallelism, shuffling, etc.)
// is evaluated and times are presented in a clean format.
//
// ------------------------------------------------------------

#include "vector_sum.h"
#include "data_generator.h"
#include "executor.h"
#include "TextTable.h"

#include <string>
#include <ios>
#include <iomanip>
#include <chrono>

// Converts a double to a string with fixed precision
std::string to_string(double x, int precision) {
    std::stringstream s;
    s << std::fixed << std::setprecision(precision) << x;
    return s.str();
}

// Converts optional nanoseconds duration into a readable string (e.g., "3.14 ms")
std::string to_string(std::optional<std::chrono::nanoseconds> test_time) {
    if (!test_time) {
        return "!!!"; // placeholder for missing values
    }

    auto ns = test_time->count();
    if (ns >= 1'000'000'000) return to_string((double)ns / 1e9, 2) + " s";
    else if (ns >= 1'000'000) return to_string((double)ns / 1e6, 2) + " ms";
    else if (ns >= 1'000) return to_string((double)ns / 1e3, 2) + " us";
    else return std::to_string(ns) + " ns";
}

// Adds a row to the result table with formatted timing data
void add_table_row(const std::string& label, const Results& results, TextTable& table) {
    table.add(label);
    table.add(to_string(results.time_ref));
    table.add(to_string(results.time_per_vector));
    table.add(to_string(results.time_shuffle));
    table.add(to_string(results.time_omp_dynamic));
    table.add(to_string(results.time_omp_static));
    table.endOfRow();
}

// Dataset 0: Few very long vectors (simulate heavy computation)
void test_dataset_0(TextTable& table) {
    InputVectors data(3, std::vector<int8_t>(250'000'000));
    OutputVector solution(data.size());

    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Few very long vectors", results, table);
}

// Dataset 1: Mixed long and short vectors (heterogeneous load)
void test_dataset_1(TextTable& table) {
    std::array<size_t, 8192> lengths{};

    // First 1/8 of the vectors are long, the rest are very short
    auto large_data_count = lengths.size() / 8;
    auto split_it = lengths.begin() + (ptrdiff_t)large_data_count;

    data_generator::generate_vector_sizes(lengths.begin(), split_it, 500'000, 300'000); // long vectors
    data_generator::generate_vector_sizes(split_it, lengths.end(), 5, 3);               // short vectors

    InputVectors data;
    for (auto j : lengths) {
        std::vector<int8_t> vec(j);
        data.push_back(vec);
    }

    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("High variance in vector sizes", results, table);
}

// Dataset 2: Massive number of very short vectors
void test_dataset_2(TextTable& table) {
    InputVectors data(10'000'000, std::vector<int8_t>(2));
    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Many short vectors", results, table);
}

// Dataset 3: Small input, bad for parallelism
void test_dataset_3(TextTable& table) {
    InputVectors data(10, std::vector<int8_t>(10));
    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Data unsuitable for parallelism", results, table);
}

int main() {
    TextTable table{};

    // Set up the header row of the table
    table.add("");                               // Label for dataset
    table.add("Sequential");                     // Basic single-threaded version
    table.add("Vector-level parallelism");       // One thread per vector
    table.add("Shuffling");                      // Shuffling workload to balance load
    table.add("Dynamic scheduling (OpenMP)");    // Runtime load balancing
    table.add("Static scheduling (OpenMP)");     // Predefined load balancing
    table.endOfRow();

    // Run all test datasets
    test_dataset_0(table);
    test_dataset_1(table);
    test_dataset_2(table);
    test_dataset_3(table);

    // Align the first column to the right
    table.setAlignment(0, TextTable::Alignment::RIGHT);

    // Print the resulting performance comparison table
    std::cout << table;

    return 0;
}
