#pragma once

#include <vector>

namespace Graphs {

    struct IGraph {
        virtual ~IGraph() = default;

        virtual auto AddEdge(int from, int to) -> void = 0;

        [[nodiscard]] virtual auto VerticesCount() const noexcept -> int = 0;

        [[nodiscard]] virtual auto GetNextVertices(int vertex) const -> std::vector<int> = 0;
        [[nodiscard]] virtual auto GetPrevVertices(int vertex) const -> std::vector<int> = 0;
    };

}  // namespace Graphs
