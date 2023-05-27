#pragma once

#include "IGraph.hpp"

namespace Graphs {

    class MatrixGraph final : public IGraph {
     public:
        MatrixGraph(int vertexCount);
        MatrixGraph(const IGraph& otherGraph) noexcept;

        auto AddEdge(int from, int to) -> void override;

        [[nodiscard]] auto VerticesCount() const noexcept -> int override;

        [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<int> override;
        [[nodiscard]] auto GetPrevVertices(int vertex) const -> std::vector<int> override;

     private:
        std::vector<std::vector<std::size_t>> m_adjacencyMatrix;
    };

}  // namespace Graphs
