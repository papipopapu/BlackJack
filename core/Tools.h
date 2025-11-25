/**
 * @file Tools.h
 * @brief Utility functions and global random number generator
 * 
 * Provides common utilities used throughout the simulation,
 * including sorting helpers and the global RNG instance.
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * @brief Global random number generator used throughout the simulation
 * 
 * Seeded with 0 by default for reproducibility. Can be re-seeded
 * for different random sequences.
 */
inline std::mt19937 GLOBAL_RNG(0);

/**
 * @brief Comparison function for descending sort
 * @param a First value to compare
 * @param b Second value to compare
 * @return true if a > b (for descending order)
 */
inline bool compare(const float& a, const float& b) {
    return a > b;
}

/**
 * @brief Generates a sorted permutation of indices based on a comparison function
 * 
 * Creates a vector of indices [0, 1, 2, ...] and sorts them based on the
 * values in vec, according to the provided comparison function.
 * 
 * @tparam T Type of elements in the vector
 * @tparam Compare Comparison function type
 * @param vec Vector of values to sort
 * @param compare Comparison function
 * @return Vector of indices representing the sorted permutation
 */
template <typename T, typename Compare>
std::vector<int> sort_permutation(
    const std::vector<T>& vec,
    Compare& compare)
{
    std::vector<int> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](int i, int j){ return compare(vec[i], vec[j]); });
    return p;
}

#endif // TOOLS_H
