#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

auto run(std::istream& input, std::ostream& output) -> void {
    int vertexCount{};
    int edgeCount{};
    input >> vertexCount >> edgeCount;

    std::vector<std::vector<int>> graph(vertexCount);
    int vertexFrom{};
    int vertexTo{};
    for (int i = 0; i != edgeCount; ++i) {
        input >> vertexFrom >> vertexTo;
        graph[vertexFrom].push_back(vertexTo);
        graph[vertexTo].push_back(vertexFrom);
    }

    int startVertex{};
    int stopVertex{};
    input >> startVertex >> stopVertex;

    std::vector<int> vertexWaysCount(vertexCount);
    vertexWaysCount[startVertex] = 1;
    std::vector<bool> visitedVertex(vertexCount, false);
    visitedVertex[startVertex] = true;
    std::queue<int> vertexQueue{{startVertex}};
    std::queue<int> newVertexQueue{};
    while (!vertexQueue.empty()) {
        for (; !vertexQueue.empty(); vertexQueue.pop()) {
            vertexFrom = vertexQueue.front();

            if (vertexFrom == stopVertex) {
                output << vertexWaysCount[vertexFrom];
                return;
            }

            for (const auto& vertexTo : graph[vertexFrom]) {
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

    output << 0;
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
