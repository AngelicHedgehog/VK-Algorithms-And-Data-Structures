#include "ArcGraph.hpp"

#include <cassert>

namespace Graphs {

    ArcGraph::ArcGraph(int vertexCount) : m_vertexCount(vertexCount) {}

    ArcGraph::ArcGraph(const IGraph& otherGraph) noexcept : ArcGraph(otherGraph.VerticesCount()) {
        for (int vertexFrom = 0; vertexFrom != m_vertexCount; ++vertexFrom) {
            for (const auto& vertexTo : otherGraph.GetNextVertices(vertexFrom)) {
                m_edgesList.insert({vertexFrom, vertexTo});
            }
        }
    }

    auto ArcGraph::AddEdge(int from, int to) -> void {
        assert(from >= 0 && from < m_vertexCount);
        assert(to >= 0 && to < m_vertexCount);

        m_edgesList.insert({from, to});
    }

    auto ArcGraph::VerticesCount() const noexcept -> int {
        return m_vertexCount;
    }

    auto ArcGraph::GetNextVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && vertex < m_vertexCount);

        std::vector<int> nextVertices{};

        for (const auto& iterEdge : m_edgesList) {
            if (iterEdge.from == vertex) {
                nextVertices.push_back(iterEdge.to);
            }
        }

        return nextVertices;
    }

    auto ArcGraph::GetPrevVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && vertex < m_vertexCount);

        std::vector<int> nextVertices{};

        for (const auto& iterEdge : m_edgesList) {
            if (iterEdge.to == vertex) {
                nextVertices.push_back(iterEdge.from);
            }
        }

        return nextVertices;
    }

}  // namespace Graphs
