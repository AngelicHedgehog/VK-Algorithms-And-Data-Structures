#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

class SetGraph {
 public:
    SetGraph(int vertexCount);

    auto AddEdge(int from, int to, int weight) -> void;

    [[nodiscard]] auto VerticesCount() const noexcept -> int;

    [[nodiscard]] auto GetNextVertices(int vertex) const -> std::vector<std::pair<int, int>>;

 private:
    std::vector<std::unordered_multimap<int, int>> m_adjacencySets;
};

SetGraph::SetGraph(int vertexCount) : m_adjacencySets(vertexCount) {}

auto SetGraph::AddEdge(int from, int to, int weight) -> void {
    assert(from >= 0 && static_cast<std::size_t>(from) < m_adjacencySets.size());
    assert(to >= 0 && static_cast<std::size_t>(to) < m_adjacencySets.size());

    m_adjacencySets[from].insert({to, weight});
    m_adjacencySets[to].insert({from, weight});
}

auto SetGraph::VerticesCount() const noexcept -> int {
    return m_adjacencySets.size();
}

auto SetGraph::GetNextVertices(int vertex) const -> std::vector<std::pair<int, int>> {
    assert(vertex >= 0 && static_cast<std::size_t>(vertex) < m_adjacencySets.size());

    return {m_adjacencySets[vertex].begin(), m_adjacencySets[vertex].end()};
}

auto readInput(std::istream& input, int& vertexCount, int& edgeCount, SetGraph& graph) -> void {
    input >> vertexCount >> edgeCount;

    graph = SetGraph{vertexCount};
    int vertexFrom{};
    int vertexTo{};
    int weight{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo >> weight;
        graph.AddEdge(vertexFrom - 1, vertexTo - 1, weight);
    }
}

auto getWeightMinSpanTree(int vertexCount, const SetGraph& graph) -> int {
    int weightMinSpanTree{0};
    std::vector<int> vertexWayWeight(vertexCount, -1);
    vertexWayWeight[0] = 0;
    std::vector<bool> visitedVertex(vertexCount, false);
    visitedVertex[0] = true;
    std::vector<int> vertexPriorityQueue{0};
    while (!vertexPriorityQueue.empty()) {
        auto minIt = vertexPriorityQueue.begin();
        for (auto it = vertexPriorityQueue.begin(); it != vertexPriorityQueue.end(); ++it) {
            if (vertexWayWeight[*it] < vertexWayWeight[*minIt]) {
                minIt = it;
            }
        }

        int vertexFrom = *minIt;
        vertexPriorityQueue.erase(minIt);
        weightMinSpanTree += vertexWayWeight[vertexFrom];

        for (const auto& edge : graph.GetNextVertices(vertexFrom)) {
            if (visitedVertex[edge.first]) {
                continue;
            }

            int& weigthVertexTo = vertexWayWeight[edge.first];
            int newWeigthVertexTo = edge.second;

            if (weigthVertexTo == -1) {
                vertexPriorityQueue.push_back(edge.first);
            }

            if (weigthVertexTo == -1 || newWeigthVertexTo < weigthVertexTo) {
                weigthVertexTo = newWeigthVertexTo;
            }
        }

        visitedVertex[vertexFrom] = true;
    }

    return weightMinSpanTree;
}

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    SetGraph graph(0);
    readInput(input, vertexCount, edgeCount, graph);

    output << getWeightMinSpanTree(vertexCount, graph);
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 4\n1 2 1\n2 3 2\n3 4 5\n4 1 4";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "7");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10\n4 3 3046\n4 5 90110\n5 1 57786\n3 2 28280\n4 3 18010\n4 5 61367\n4 1 18811\n4 2 69898\n3 5 72518\n3 1 85838";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "107923");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
