#pragma once

#include <cstdint>

namespace ff_browser_decimal {

struct ProductMemoStats {
    uint32_t hits = 0;
    uint32_t misses = 0;
};

inline thread_local ProductMemoStats productMemoStats;

} // namespace ff_browser_decimal
