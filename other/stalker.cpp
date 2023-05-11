#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

auto readInput(std::istream &input, int &countVertexs, int &countGraphs, std::vector<std::unordered_multimap<int, int>> &graphs) -> void {
  input >> countVertexs >> countGraphs;

  std::unordered_multimap<int, int> graph{};
  int countEdges{};
  int firstVectex{};
  int secondVectex{};
  for (int i = 0; i < countGraphs; ++i) {
    graph.clear();

    input >> countEdges;
    for (int j = 0; j < countEdges; ++j) {
      input >> firstVectex >> secondVectex;

      graph.insert({firstVectex, secondVectex});
      graph.insert({secondVectex, firstVectex});
    }

    graphs.push_back(graph);
  }
}

auto getConnectivityComponents(const std::unordered_multimap<int, int> &graph) -> std::vector<std::unordered_set<int>> {
  std::vector<std::unordered_set<int>> connectivityComponents{};

  std::unordered_set<int> connectivityComponent{};
  std::unordered_set<int> marked{};
  std::queue<int> vertexQueue{};
  int nextVertex{};
  auto valuesByKey{graph.equal_range(0)};

  for (auto it = graph.begin(); it != graph.end(); ++it) {
    connectivityComponent.clear();
    vertexQueue = std::queue<int>{{it->first}};

    while (!vertexQueue.empty()) {
      nextVertex = vertexQueue.front();
      vertexQueue.pop();

      if (marked.count(nextVertex)) {
        continue;
      }

      marked.insert(nextVertex);
      connectivityComponent.insert(nextVertex);

      valuesByKey = graph.equal_range(nextVertex);
      for (auto it = valuesByKey.first; it != valuesByKey.second; ++it) {
        vertexQueue.push(it->second);
      }
    }

    if (!connectivityComponent.empty()) {
      connectivityComponents.push_back(connectivityComponent);
    }
  }

  return connectivityComponents;
}

auto run(std::istream &input, std::ostream &output) -> void {
  int countVertexs{};
  int countGraphs{};
  std::vector<std::unordered_multimap<int, int>> graphs{};

  readInput(input, countVertexs, countGraphs, graphs);

  std::vector<std::vector<std::unordered_set<int>>> graphsConnectivityComponents{};

  for (const auto &graph : graphs) {
    graphsConnectivityComponents.push_back(getConnectivityComponents(graph));
  }

  // for (const auto &a : graphsConnectivityComponents) {
  //   for (const auto &b : a) {
  //     for (const auto &c : b) {
  //       std::cout << c << ' ';
  //     }
  //     std::cout << "\n";
  //   }
  //   std::cout << "\n";
  // }

  int moneyCost{0};
  std::unordered_map<int, std::pair<int, int>> vertexXcostXmap{{1, {0, -1}}};
  std::vector<std::unordered_set<int>> *graphConnectivityComponentsPtr{};
  std::queue<int> vertexQueue{{1}};
  std::queue<int> nextVertexQueue{};
  int nextVertex{};
  int vertexMapIndex{};

  while (!vertexQueue.empty()) {
    ++moneyCost;

    nextVertexQueue = {};
    while (!vertexQueue.empty()) {
      nextVertex = vertexQueue.front();
      vertexQueue.pop();

      vertexMapIndex = vertexXcostXmap.at(nextVertex).second;

      for (int graphIndex = 0; graphIndex < countGraphs; ++graphIndex) {
        if (graphIndex == vertexMapIndex) {
          continue;
        }

        graphConnectivityComponentsPtr = &graphsConnectivityComponents.at(graphIndex);

        for (auto connectivityComponentIt = std::prev(graphConnectivityComponentsPtr->end());
             std::next(connectivityComponentIt) != graphConnectivityComponentsPtr->begin(); --connectivityComponentIt) {
          if (!connectivityComponentIt->count(nextVertex)) {
            continue;
          }

          for (const auto &componentVertex : *connectivityComponentIt) {
            if (vertexXcostXmap.count(componentVertex)) {
              continue;
            }

            if (componentVertex == countVertexs) {
              output << moneyCost;
              return;
            }

            vertexXcostXmap.insert({componentVertex, {moneyCost, graphIndex}});
            nextVertexQueue.push(componentVertex);
          }

          graphConnectivityComponentsPtr->erase(connectivityComponentIt);
        }
      }
    }

    std::swap(vertexQueue, nextVertexQueue);
  }

  //   for (const auto &a : vertexXcostXmap) {
  //     std::cout << a.first << " -> cost:" << a.second.first
  //               << "; map:" << a.second.second << '\n';
  //   }

  output << -1;
}

auto test() -> void {
  {
    std::stringstream input;
    std::stringstream output;
    input << "12 4\n"
             "4\n1 6\n2 4\n7 9\n10 12\n"
             "3\n1 4\n7 11\n3 6\n"
             "3\n2 5\n4 11\n8 9\n"
             "5\n3 10\n10 7\n7 2\n12 3\n5 12";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "3");
  }
}

auto main() -> int {
  run(std::cin, std::cout);
  // test();
  return 0;
}