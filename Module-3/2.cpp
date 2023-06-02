#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

class ListGraph {
 public:
    ListGraph(int vertexCount);

    auto AddEdge(int from, int to) -> void;

    [[nodiscard]] auto VerticesCount() const noexcept -> int;

    [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<int>;

 private:
    std::vector<std::vector<int>> m_adjacencyLists;
};

ListGraph::ListGraph(int vertexCount) : m_adjacencyLists(vertexCount) {}

auto ListGraph::AddEdge(int from, int to) -> void {
    assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencyLists.size());
    assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencyLists.size());

    m_adjacencyLists[from].push_back(to);
    m_adjacencyLists[to].push_back(from);
}

auto ListGraph::VerticesCount() const noexcept -> int {
    return m_adjacencyLists.size();
}

auto ListGraph::GetNextVertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyLists.size());

    return m_adjacencyLists[vertex];
}

auto readInput(std::istream& input, int& vertexCount, int& edgeCount, ListGraph& graph, int& startVertex, int& stopVertex) -> void {
    input >> vertexCount >> edgeCount;

    graph = ListGraph{vertexCount};
    int vertexFrom{};
    int vertexTo{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo;
        graph.AddEdge(vertexFrom, vertexTo);
    }

    input >> startVertex >> stopVertex;
}

auto findCountWays(int& vertexCount, const ListGraph& graph, int startVertex, int stopVertex) -> int {
    std::vector<int> vertexWaysCount(vertexCount);
    vertexWaysCount[startVertex] = 1;
    std::vector<bool> visitedVertex(vertexCount, false);
    visitedVertex[startVertex] = true;
    std::queue<int> vertexQueue{{startVertex}};
    std::queue<int> newVertexQueue{};
    while (!vertexQueue.empty()) {
        for (; !vertexQueue.empty(); vertexQueue.pop()) {
            int vertexFrom = vertexQueue.front();

            if (vertexFrom == stopVertex) {
                return vertexWaysCount[vertexFrom];
            }

            for (const auto& vertexTo : graph.GetNextVertices(vertexFrom)) {
                if (visitedVertex[vertexTo]) {
                    continue;
                }
                if (vertexWaysCount[vertexTo] == 0) {
                    newVertexQueue.push(vertexTo);
                }
                vertexWaysCount[vertexTo] += vertexWaysCount[vertexFrom];
            }
        }

        for (int iterVertex{}; !newVertexQueue.empty(); newVertexQueue.pop()) {
            iterVertex = newVertexQueue.front();
            vertexQueue.push(iterVertex);
            visitedVertex[iterVertex] = true;
        }
    }

    return 0;
}

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    ListGraph graph(0);
    int startVertex{};
    int stopVertex{};

    readInput(input, vertexCount, edgeCount, graph, startVertex, stopVertex);

    output << findCountWays(vertexCount, graph, startVertex, stopVertex);
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n5\n0 1\n0 2\n1 2\n1 3\n2 3\n\n0 3";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "2");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
