#include "ListGraph.hpp"

#include <cassert>

namespace Graphs {

    ListGraph::ListGraph(int vertexCount) : m_adjacencyLists(vertexCount), m_prevAdjacencyLists(vertexCount) {}

    ListGraph::ListGraph(const IGraph& otherGraph) noexcept : ListGraph(otherGraph.VerticesCount()) {
        for (std::size_t vertexFrom = 0; vertexFrom != m_adjacencyLists.size(); ++vertexFrom) {
            for (const auto& vertexTo : otherGraph.GetNextVertices(vertexFrom)) {
                m_adjacencyLists[vertexFrom].push_back(vertexTo);
                m_prevAdjacencyLists[vertexTo].push_back(vertexFrom);
            }
        }
    }

    auto ListGraph::AddEdge(int from, int to) -> void {
        assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencyLists.size());
        assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencyLists.size());

        m_adjacencyLists[from].push_back(to);
        m_prevAdjacencyLists[to].push_back(from);
    }

    auto ListGraph::VerticesCount() const noexcept -> int {
        return m_adjacencyLists.size();
    }

    auto ListGraph::GetNextVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyLists.size());

        return m_adjacencyLists[vertex];
    }

    auto ListGraph::GetPrevVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyLists.size());

        return m_prevAdjacencyLists[vertex];
    }

}  // namespace Graphs
