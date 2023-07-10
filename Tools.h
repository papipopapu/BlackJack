#ifndef TOOLS_H
#define TOOLS_H
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>


std::mt19937 GLOBAL_RNG(1);

bool compare(const float& a, const float& b) {
    return a < b;
}
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

#endif