#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    input >> vertexCount >> edgeCount;

    std::vector<std::vector<std::pair<int, int>>> graph(vertexCount);
    int vertexFrom{};
    int vertexTo{};
    int weight{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo >> weight;
        graph[vertexFrom].push_back({vertexTo, weight});
        graph[vertexTo].push_back({vertexFrom, weight});
    }

    int startVertex{};
    int stopVertex{};
    input >> startVertex >> stopVertex;

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
        vertexFrom = *minIt;
        vertexPriorityQueue.erase(minIt);

        if (vertexFrom == stopVertex) {
            output << vertexWayWeight[vertexFrom];
            return;
        }

        for (const auto& edge : graph[vertexFrom]) {
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

    output << -1;
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
