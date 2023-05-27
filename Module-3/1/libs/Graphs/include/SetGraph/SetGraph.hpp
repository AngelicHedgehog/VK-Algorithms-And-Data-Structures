#pragma once

#include "IGraph.hpp"

#include "unordered_set"

namespace Graphs {

    class SetGraph final : public IGraph {
     public:
        SetGraph(int vertexCount);
        SetGraph(const IGraph& otherGraph) noexcept;

        auto AddEdge(int from, int to) -> void override;

        [[nodiscard]] auto VerticesCount() const noexcept -> int override;

        [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<int> override;
        [[nodiscard]] auto GetPrevVertices(int vertex) const -> std::vector<int> override;

     private:
        std::vector<std::unordered_multiset<int>> m_adjacencySets;
        std::vector<std::unordered_multiset<int>> m_prevAdjacencySets;
    };

}  // namespace Graphs
