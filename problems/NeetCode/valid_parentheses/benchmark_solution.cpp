#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER "solution.hpp"
#endif

#include SOLUTION_HEADER

#include "allocation_tracker.hpp"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

enum class Workload {
    uniform,
    opening_only,
    nested_valid,
    shallow_valid,
};

constexpr std::array<char, 3> openings{'(', '{', '['};
constexpr std::array<char, 6> brackets{'(', '{', '[', ')', '}', ']'};

char closing_for(char opening) {
    switch (opening) {
    case '(':
        return ')';
    case '{':
        return '}';
    default:
        return ']';
    }
}

std::string make_input(Workload workload, std::size_t length,
                       std::mt19937_64& generator) {
    std::uniform_int_distribution<std::size_t> opening_index{
        0, openings.size() - 1};
    std::uniform_int_distribution<std::size_t> bracket_index{
        0, brackets.size() - 1};
    std::string input;
    input.reserve(length);

    if (workload == Workload::uniform) {
        while (input.size() < length) {
            input.push_back(brackets[bracket_index(generator)]);
        }
        return input;
    }

    if (workload == Workload::opening_only) {
        while (input.size() < length) {
            input.push_back(openings[opening_index(generator)]);
        }
        return input;
    }

    if (workload == Workload::shallow_valid) {
        while (input.size() < length) {
            const char opening = openings[opening_index(generator)];
            input.push_back(opening);
            input.push_back(closing_for(opening));
        }
        return input;
    }

    std::string nested_openings;
    nested_openings.reserve(length / 2);
    while (nested_openings.size() < length / 2) {
        nested_openings.push_back(openings[opening_index(generator)]);
    }
    input = nested_openings;
    for (auto iterator = nested_openings.rbegin();
         iterator != nested_openings.rend(); ++iterator) {
        input.push_back(closing_for(*iterator));
    }
    return input;
}

std::vector<std::string> make_dataset(Workload workload, std::size_t length) {
    const std::size_t input_count =
        std::max<std::size_t>(256, 65'536 / length);
    const auto workload_seed = static_cast<std::uint64_t>(workload) << 32U;
    std::mt19937_64 generator{0x5eed1234ULL + workload_seed + length};
    std::vector<std::string> inputs;
    inputs.reserve(input_count);
    for (std::size_t index = 0; index < input_count; ++index) {
        inputs.push_back(make_input(workload, length, generator));
    }
    return inputs;
}

const std::vector<std::string>& dataset_for(Workload workload,
                                            std::size_t length) {
    using DatasetKey = std::pair<Workload, std::size_t>;
    static std::map<DatasetKey, std::vector<std::string>> datasets;
    const DatasetKey key{workload, length};
    auto iterator = datasets.find(key);
    if (iterator == datasets.end()) {
        iterator = datasets.emplace(key, make_dataset(workload, length)).first;
    }
    return iterator->second;
}

std::string_view workload_name(Workload workload) {
    switch (workload) {
    case Workload::uniform:
        return "uniform";
    case Workload::opening_only:
        return "opening_only";
    case Workload::nested_valid:
        return "nested_valid";
    case Workload::shallow_valid:
        return "shallow_valid";
    }
    return "unknown";
}

void benchmark_solution(benchmark::State& state, Workload workload,
                        std::size_t length) {
    const std::vector<std::string>& inputs = dataset_for(workload, length);
    Solution solution;
    std::size_t input_index = 0;

    for (auto iteration : state) {
        static_cast<void>(iteration);
        bool valid = solution.isValid(inputs[input_index]);
        benchmark::DoNotOptimize(valid);
        input_index = (input_index + 1) % inputs.size();
    }

    std::size_t valid_count = 0;
    benchmark_support::start_allocation_tracking();
    for (const std::string& input : inputs) {
        valid_count += solution.isValid(input);
    }
    const benchmark_support::AllocationTotals allocations =
        benchmark_support::stop_allocation_tracking();

    const double input_count = static_cast<double>(inputs.size());
    state.counters["allocs_per_input"] =
        static_cast<double>(allocations.count) / input_count;
    state.counters["allocated_bytes_per_input"] =
        static_cast<double>(allocations.bytes) / input_count;
    state.counters["valid_fraction"] =
        static_cast<double>(valid_count) / input_count;
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() *
                            static_cast<std::int64_t>(length));
}

[[maybe_unused]] const bool benchmarks_registered = [] {
    constexpr std::array<Workload, 4> workloads{
        Workload::uniform,
        Workload::opening_only,
        Workload::nested_valid,
        Workload::shallow_valid,
    };
    constexpr std::array<std::size_t, 3> lengths{16, 128, 1000};

    for (Workload workload : workloads) {
        for (std::size_t length : lengths) {
            const std::string name = std::string{"isValid/"} +
                                     std::string{workload_name(workload)} + "/" +
                                     std::to_string(length);
            benchmark::RegisterBenchmark(name.c_str(), benchmark_solution,
                                         workload, length)
                ->Unit(benchmark::kNanosecond);
        }
    }
    return true;
}();

} // namespace

BENCHMARK_MAIN();
