#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"
#include "TestGraphs.hpp"

#include <iostream>

auto subMain(std::istream&, std::ostream& output) -> int {
    output << "======================Testing ListGraph...======================\n";
    tests::testGraph<Graphs::ListGraph>(output);
    output << "======================Tests passed!======================\n\n";

    output << "======================Testing MatrixGraph...======================\n";
    tests::testGraph<Graphs::MatrixGraph>(output);
    output << "======================Tests passed!======================\n\n";

    output << "======================Testing SetGraph...======================\n";
    tests::testGraph<Graphs::SetGraph>(output);
    output << "======================Tests passed!======================\n\n";

    output << "======================Testing ArcGraph...======================\n";
    tests::testGraph<Graphs::ArcGraph>(output);
    output << "======================Tests passed!======================\n\n";

    return 0;
}

auto main() -> int {
    return subMain(std::cin, std::cout);
}