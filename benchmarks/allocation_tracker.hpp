#pragma once

#include <cstddef>

namespace benchmark_support {

struct AllocationTotals {
    std::size_t count;
    std::size_t bytes;
};

void start_allocation_tracking() noexcept;
AllocationTotals stop_allocation_tracking() noexcept;

} // namespace benchmark_support
