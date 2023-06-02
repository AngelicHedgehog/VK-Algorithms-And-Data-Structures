#include <cassert>
#include <iostream>
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
    m_adjacencyLists[to].push_back({from, weight});
}

auto ListGraph::VerticesCount() const noexcept -> int {
    return m_adjacencyLists.size();
}

auto ListGraph::GetNextVertices(int vertex) const -> std::vector<std::pair<int, int>> {
    assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencyLists.size());

    return m_adjacencyLists[vertex];
}

auto readInput(std::istream& input, int& vertexCount, int& edgeCount, ListGraph& graph, int& startVertex, int& stopVertex) -> void {
    input >> vertexCount >> edgeCount;

    graph = ListGraph{vertexCount};
    int vertexFrom{};
    int vertexTo{};
    int weight{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo >> weight;
        graph.AddEdge(vertexFrom, vertexTo, weight);
    }

    input >> startVertex >> stopVertex;
}

auto lenShortestWay(int vertexCount, const ListGraph& graph, int startVertex, int stopVertex) -> int {
    std::vector<int> vertexWayWeight(vertexCount, -1);
    vertexWayWeight[startVertex] = 0;
    std::vector<bool> visitedVertex(vertexCount, false);
    visitedVertex[startVertex] = true;
    std::vector<int> vertexPriorityQueue{startVertex};
    while (!vertexPriorityQueue.empty()) {
        auto minIt = vertexPriorityQueue.begin();
        for (auto it = vertexPriorityQueue.begin(); it != vertexPriorityQueue.end(); ++it) {
            if (vertexWayWeight[*it] < vertexWayWeight[*minIt]) {
                minIt = it;
            }
        }
        int vertexFrom = *minIt;
        vertexPriorityQueue.erase(minIt);

        if (vertexFrom == stopVertex) {
            return vertexWayWeight[vertexFrom];
        }

        for (const auto& edge : graph.GetNextVertices(vertexFrom)) {
            if (visitedVertex[edge.first]) {
                continue;
            }

            int& weigthVertexTo = vertexWayWeight[edge.first];
            int newWeigthVertexTo = vertexWayWeight[vertexFrom] + edge.second;

            if (weigthVertexTo == -1) {
                vertexPriorityQueue.push_back(edge.first);
            }

            if (weigthVertexTo == -1 || newWeigthVertexTo < weigthVertexTo) {
                weigthVertexTo = newWeigthVertexTo;
            }
        }

        visitedVertex[vertexFrom] = true;
    }

    return -1;
}

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    ListGraph graph(0);
    int startVertex{};
    int stopVertex{};
    readInput(input, vertexCount, edgeCount, graph, startVertex, stopVertex);

    output << lenShortestWay(vertexCount, graph, startVertex, stopVertex);
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "6\n9\n0 3 1\n0 4 2\n1 2 7\n1 3 2\n1 4 3\n1 5 3\n2 5 3\n3 4 4\n3 5 6\n0 2";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "9");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
