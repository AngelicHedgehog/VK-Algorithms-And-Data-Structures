#pragma once

#include "IGraph.hpp"

#include <cassert>
#include <iostream>

namespace tests {

    auto isVectorsEqual(const std::vector<int>& firstVector, const std::vector<int>& secondVector) -> bool;

}

namespace {

    template <typename GraphType>
    auto createGraph() -> GraphType {
        return std::move(GraphType{4});
    }

    template <typename GraphType>
    auto checkByVerticesCount(const GraphType& graph) -> void {
        assert(graph.VerticesCount() == 4);
    }

    template <typename GraphType>
    auto checkByAddEdge(GraphType& graph) -> void {
        graph.AddEdge(1, 2);
        graph.AddEdge(1, 3);
        graph.AddEdge(1, 3);
        graph.AddEdge(3, 2);
        graph.AddEdge(3, 2);
        graph.AddEdge(3, 2);
        graph.AddEdge(3, 3);
        graph.AddEdge(3, 3);
        graph.AddEdge(3, 3);
        graph.AddEdge(3, 3);
    }

    template <typename GraphType>
    auto checkByGetNextVertices(const GraphType& graph) -> void {
        assert(tests::isVectorsEqual(graph.GetNextVertices(0), {}));
        assert(tests::isVectorsEqual(graph.GetNextVertices(1), {2, 3, 3}));
        assert(tests::isVectorsEqual(graph.GetNextVertices(2), {}));
        assert(tests::isVectorsEqual(graph.GetNextVertices(3), {2, 2, 2, 3, 3, 3, 3}));
    }

    template <typename GraphType>
    auto checkByGetPrevVertices(const GraphType& graph) -> void {
        assert(tests::isVectorsEqual(graph.GetPrevVertices(0), {}));
        assert(tests::isVectorsEqual(graph.GetPrevVertices(1), {}));
        assert(tests::isVectorsEqual(graph.GetPrevVertices(2), {1, 3, 3, 3}));
        assert(tests::isVectorsEqual(graph.GetPrevVertices(3), {1, 1, 3, 3, 3, 3}));
    }

    template <typename GraphType>
    auto copyGraph(const GraphType& graph) -> GraphType {
        return std::move(GraphType{graph});
    }

}  // namespace

namespace tests {

    template <typename GraphType>
        requires std::is_base_of<Graphs::IGraph, GraphType>::value
    auto testGraph(std::ostream& output) -> void {
        output << "Creating graph... ";
        GraphType graph = createGraph<GraphType>();
        output << "Done!\n";

        output << "Check graph by VerticesCount... ";
        checkByVerticesCount(graph);
        output << "Done!\n";

        output << "Check graph by AddEdge... ";
        checkByAddEdge(graph);
        output << "Done!\n";

        output << "Check graph by GetNextVertices... ";
        checkByGetNextVertices(graph);
        output << "Done!\n";

        output << "Check graph by GetPrevVertices... ";
        checkByGetPrevVertices(graph);
        output << "Done!\n";

        output << "Copying graph... ";
        GraphType graphCopy = copyGraph(graph);
        output << "Done!\n";

        output << "Check graph copy by VerticesCount... ";
        checkByVerticesCount(graphCopy);
        output << "Done!\n";

        output << "Check graph copy by GetNextVertices... ";
        checkByGetNextVertices(graphCopy);
        output << "Done!\n";

        output << "Check graph copy by GetPrevVertices... ";
        checkByGetPrevVertices(graphCopy);
        output << "Done!\n";
    }

}  // namespace tests
