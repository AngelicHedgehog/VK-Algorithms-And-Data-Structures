#include "TestGraphs.hpp"

#include <unordered_set>

namespace tests {

    auto isVectorsEqual(const std::vector<int>& firstVector, const std::vector<int>& secondVector) -> bool {
        return std::unordered_multiset<int>{firstVector.begin(), firstVector.end()} ==
               std::unordered_multiset<int>{secondVector.begin(), secondVector.end()};
    }

}  // namespace tests
