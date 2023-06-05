#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

class ListGraph {
 public:
    ListGraph(int vertexCount);

    auto AddEdge(int from, int to, int weight) -> void;

    [[nodiscard]] auto VerticesCount() const noexcept -> int;

    [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<std::pair<int, int>>;

 private:
    std::vector<std::vector<std::pair<int, int>>> m_adjacencyLists;
};

ListGraph::ListGraph(int vertexCount) : m_adjacencyLists(vertexCount) {}

auto ListGraph::AddEdge(int from, int to, int weight) -> void {
    assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencyLists.size());
    assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencyLists.size());

    m_adjacencyLists[from].push_back({to, weight});
}

auto ListGraph::VerticesCount() const noexcept -> int {
    return m_adjacencyLists.size();
}

auto ListGraph::GetNextVertices(int vertex) const -> std::vector<std::pair<int, int>> {
    assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyLists.size());

    return m_adjacencyLists[vertex];
}

auto readInput(std::istream& input, int& vertexCount, int& edgeCount, int& maxEdgeLength, int& startVertex, int& stopVertex,
               ListGraph& graph) -> void {
    input >> vertexCount >> edgeCount >> maxEdgeLength >> startVertex >> stopVertex;
    --startVertex;
    --stopVertex;

    graph = ListGraph{vertexCount};
    int vertexFrom{};
    int vertexTo{};
    int weight{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo >> weight;
        graph.AddEdge(vertexFrom - 1, vertexTo - 1, weight);
    }
}

struct WayToVertexInfo {
    int vertex;
    int wayLength;
};

auto lenShortestWay(int vertexCount, int maxEdgeLength, int startVertex, int stopVertex, const ListGraph& graph) -> int {
    std::vector<int> vertexMinLength(vertexCount, -1);
    vertexMinLength[startVertex] = 0;
    std::queue<WayToVertexInfo> vertexQueue{{{startVertex, 0}}};
    for (int wayEdgeLength = 0; wayEdgeLength != maxEdgeLength; ++wayEdgeLength) {
        for (int i = vertexQueue.size(); i != 0; --i) {
            auto wayToVertex = vertexQueue.front();
            vertexQueue.pop();

            for (const auto& [vertexTo, weight] : graph.GetNextVertices(wayToVertex.vertex)) {
                WayToVertexInfo nextVertexWay{vertexTo, wayToVertex.wayLength + weight};

                if (vertexMinLength[vertexTo] == -1 || vertexMinLength[vertexTo] > nextVertexWay.wayLength) {
                    vertexMinLength[vertexTo] = nextVertexWay.wayLength;
                    vertexQueue.push(nextVertexWay);
                }
            }
        }
    }

    return vertexMinLength[stopVertex];
}

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    int maxEdgeLength{};
    int startVertex{};
    int stopVertex{};
    ListGraph graph(0);
    readInput(input, vertexCount, edgeCount, maxEdgeLength, startVertex, stopVertex, graph);

    output << lenShortestWay(vertexCount, maxEdgeLength, startVertex, stopVertex, graph);
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 7 2 4 1\n1 2 6\n5 1 1\n4 1 9\n4 5 3\n4 3 2\n2 5 7\n3 5 1";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 3 1 1 3\n1 2 4\n2 3 5\n3 1 6";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "-1");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
