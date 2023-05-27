#include "MatrixGraph.hpp"

#include <cassert>

namespace Graphs {

    MatrixGraph::MatrixGraph(int vertexCount)
        : m_adjacencyMatrix(vertexCount, std::vector<std::size_t>(static_cast<std::size_t>(vertexCount), 0)) {}

    MatrixGraph::MatrixGraph(const IGraph& otherGraph) noexcept : MatrixGraph(otherGraph.VerticesCount()) {
        for (std::size_t iterVertex = 0; iterVertex != m_adjacencyMatrix.size(); ++iterVertex) {
            for (const auto& vertexTo : otherGraph.GetNextVertices(iterVertex)) {
                ++m_adjacencyMatrix[iterVertex][vertexTo];
            }
        }
    }

    auto MatrixGraph::AddEdge(int from, int to) -> void {
        assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencyMatrix.size());
        assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencyMatrix.size());

        ++m_adjacencyMatrix[from][to];
    }

    auto MatrixGraph::VerticesCount() const noexcept -> int {
        return m_adjacencyMatrix.size();
    }

    auto MatrixGraph::GetNextVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyMatrix.size());

        std::vector<int> nextVertices{};

        for (std::size_t vertexTo = 0; vertexTo != m_adjacencyMatrix.size(); ++vertexTo) {
            for (std::size_t edgeNum = 0; edgeNum != m_adjacencyMatrix[vertex][vertexTo]; ++edgeNum) {
                nextVertices.push_back(vertexTo);
            }
        }

        return nextVertices;
    }

    auto MatrixGraph::GetPrevVertices(int vertex) const -> std::vector<int> {
        assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyMatrix.size());

        std::vector<int> prevVertices{};

        for (std::size_t vertexFrom = 0; vertexFrom != m_adjacencyMatrix.size(); ++vertexFrom) {
            for (std::size_t edgeNum = 0; edgeNum != m_adjacencyMatrix[vertexFrom][vertex]; ++edgeNum) {
                prevVertices.push_back(vertexFrom);
            }
        }

        return prevVertices;
    }

}  // namespace Graphs
