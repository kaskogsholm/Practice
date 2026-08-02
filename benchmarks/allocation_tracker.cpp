#include "allocation_tracker.hpp"

#include <cstdlib>
#include <new>

namespace {

bool tracking_enabled = false;
std::size_t allocation_count = 0;
std::size_t allocated_bytes = 0;

void record_allocation(std::size_t size) noexcept {
    if (tracking_enabled) {
        ++allocation_count;
        allocated_bytes += size;
    }
}

} // namespace

void* operator new(std::size_t size) {
    if (size == 0) {
        size = 1;
    }
    record_allocation(size);
    if (void* memory = std::malloc(size)) {
        return memory;
    }
    throw std::bad_alloc{};
}

void* operator new[](std::size_t size) {
    return ::operator new(size);
}

void operator delete(void* memory) noexcept {
    std::free(memory);
}

void operator delete[](void* memory) noexcept {
    std::free(memory);
}

void operator delete(void* memory, std::size_t) noexcept {
    std::free(memory);
}

void operator delete[](void* memory, std::size_t) noexcept {
    std::free(memory);
}

namespace benchmark_support {

void start_allocation_tracking() noexcept {
    allocation_count = 0;
    allocated_bytes = 0;
    tracking_enabled = true;
}

AllocationTotals stop_allocation_tracking() noexcept {
    tracking_enabled = false;
    return {
        .count = allocation_count,
        .bytes = allocated_bytes,
    };
}

} // namespace benchmark_support
