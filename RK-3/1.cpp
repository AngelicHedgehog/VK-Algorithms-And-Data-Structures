#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
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

auto readInput(std::istream& input, int& vertexCount, int& edgeCount, ListGraph& graph) -> void {
    input >> vertexCount >> edgeCount;

    graph = ListGraph{vertexCount};
    int vertexFrom{};
    int vertexTo{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo;
        graph.AddEdge(vertexFrom, vertexTo);
    }
}

auto isGraphTree(int vertexCount, const ListGraph& graph) -> bool {
    int countVisitedVertex{0};
    std::vector<bool> visitedVertex(vertexCount, false);
    std::vector<int> lastVertex(vertexCount, -1);
    std::queue<int> vertexQueue{{0}};
    while (!vertexQueue.empty()) {
        int vertexFrom = vertexQueue.front();
        vertexQueue.pop();

        if (visitedVertex[vertexFrom]) {
            return false;
        }

        ++countVisitedVertex;
        visitedVertex[vertexFrom] = true;

        for (const auto& vertexTo : graph.GetNextVertices(vertexFrom)) {
            if (lastVertex[vertexFrom] == vertexTo) {
                continue;
            }
            vertexQueue.push(vertexTo);
            lastVertex[vertexTo] = vertexFrom;
        }
    }

    return countVisitedVertex == vertexCount ? true : false;
}

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    ListGraph graph(0);
    readInput(input, vertexCount, edgeCount, graph);

    output << (isGraphTree(vertexCount, graph) ? 1 : 0);
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2\n0 1\n0 2";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n2\n0 1\n0 2";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
