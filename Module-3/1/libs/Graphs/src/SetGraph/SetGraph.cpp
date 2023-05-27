#include "SetGraph.hpp"

#include <cassert>

namespace Graphs {

    SetGraph::SetGraph(int vertexCount) : m_adjacencySets(vertexCount), m_prevAdjacencySets(vertexCount) {}

    SetGraph::SetGraph(const IGraph& otherGraph) noexcept : SetGraph(otherGraph.VerticesCount()) {
        for (std::size_t vertexFrom = 0; vertexFrom != m_adjacencySets.size(); ++vertexFrom) {
            for (const auto& vertexTo : otherGraph.GetNextVertices(vertexFrom)) {
                m_adjacencySets[vertexFrom].insert(vertexTo);
                m_prevAdjacencySets[vertexTo].insert(vertexFrom);
            }
        }
    }

    auto SetGraph::AddEdge(int from, int to) -> void {
        assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencySets.size());
        assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencySets.size());

        m_adjacencySets[from].insert(to);
        m_prevAdjacencySets[to].insert(from);
    }

    auto SetGraph::VerticesCount() const noexcept -> int {
        return m_adjacencySets.size();
    }

    auto SetGraph::GetNextVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencySets.size());

        return {m_adjacencySets[vertex].begin(), m_adjacencySets[vertex].end()};
    }

    auto SetGraph::GetPrevVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencySets.size());

        return {m_prevAdjacencySets[vertex].begin(), m_prevAdjacencySets[vertex].end()};
    }

}  // namespace Graphs
