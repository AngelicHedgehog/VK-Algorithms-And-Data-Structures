#pragma once

#include "IGraph.hpp"

namespace Graphs {

    class ListGraph final : public IGraph {
     public:
        ListGraph(int vertexCount);
        ListGraph(const IGraph& otherGraph) noexcept;

        auto AddEdge(int from, int to) -> void override;

        [[nodiscard]] auto VerticesCount() const noexcept -> int override;

        [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<int> override;
        [[nodiscard]] auto GetPrevVertices(int vertex) const -> std::vector<int> override;

     private:
        std::vector<std::vector<int>> m_adjacencyLists;
        std::vector<std::vector<int>> m_prevAdjacencyLists;
    };

}  // namespace Graphs
