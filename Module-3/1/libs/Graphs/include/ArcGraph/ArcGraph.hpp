#pragma once

#include "IGraph.hpp"

#include <unordered_set>

namespace Graphs {

    class ArcGraph final : public IGraph {
     public:
        ArcGraph(int vertexCount);
        ArcGraph(const IGraph& otherGraph) noexcept;

        auto AddEdge(int from, int to) -> void override;

        [[nodiscard]] auto VerticesCount() const noexcept -> int override;

        [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<int> override;
        [[nodiscard]] auto GetPrevVertices(int vertex) const -> std::vector<int> override;

     private:
        struct Edge {
            int from{};
            int to{};
        };
        struct HashEdge {
            auto operator()(const Edge& edge) const -> std::size_t { return std::hash<int>()(edge.from) ^ std::hash<int>()(edge.to); }
        };
        struct EqualEdge {
            auto operator()(const Edge& edge1, const Edge& edge2) const -> bool { return edge1.from == edge2.from && edge1.to == edge2.to; }
        };

        const int m_vertexCount;
        std::unordered_multiset<Edge, HashEdge, EqualEdge> m_edgesList;
    };

}  // namespace Graphs
