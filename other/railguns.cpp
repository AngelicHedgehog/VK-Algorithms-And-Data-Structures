#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

enum class FireDirect {
    Horizontal,
    Vertical,
};

struct RailgunInfo {
    int timePoint;
    int coord;
    FireDirect direct;
};

struct InputKit {
    int fieldHeight;
    int fieldWidth;
    std::vector<RailgunInfo> railgunsInfo;
};

void readInput(std::istream& input, std::vector<InputKit>& inputKits) {
    int countKits{};
    input >> countKits;

    inputKits = std::vector<InputKit>(countKits);
    for (auto& inputKit : inputKits) {
        input >> inputKit.fieldHeight >> inputKit.fieldWidth;
        ++inputKit.fieldHeight;
        ++inputKit.fieldWidth;

        int countRailguns{};
        input >> countRailguns;

        inputKit.railgunsInfo = std::vector<RailgunInfo>(countRailguns);
        for (auto& railgunInfo : inputKit.railgunsInfo) {
            input >> railgunInfo.timePoint;
            --railgunInfo.timePoint;

            int railgunDirect{};
            input >> railgunDirect;
            railgunInfo.direct = (railgunDirect == 1 ? FireDirect::Horizontal : FireDirect::Vertical);

            input >> railgunInfo.coord;
        }
    }
}

using matrix1d = std::vector<bool>;
using matrix2d = std::vector<matrix1d>;
using matrix3d = std::vector<matrix2d>;

void subMain(std::istream& input, std::ostream& output) {
    std::vector<InputKit> inputKits{};
    readInput(input, inputKits);

    for (const auto& inputKit : inputKits) {
        matrix3d field(inputKit.fieldHeight, matrix2d(inputKit.fieldWidth, matrix1d(inputKit.railgunsInfo.size() + 1, true)));

        for (const auto& railgunInfo : inputKit.railgunsInfo) {
            int x{0};
            int y{0};
            int r{};
            (railgunInfo.direct == FireDirect::Horizontal ? x : y) = railgunInfo.coord;

            for (; x != inputKit.fieldHeight && y != inputKit.fieldWidth; ++(railgunInfo.direct == FireDirect::Vertical ? x : y)) {
                r = railgunInfo.timePoint + railgunInfo.coord - x - y;
                if (r < 0 || r > static_cast<int>(inputKit.railgunsInfo.size())) {
                    continue;
                }

                field[x][y][r] = false;
            }
        }

        for (std::size_t r = 0; r != inputKit.railgunsInfo.size() + 1; ++r) {
            for (int x = 0; x != inputKit.fieldHeight; ++x) {
                for (int y = 0; y != inputKit.fieldWidth; ++y) {
                    if (x == 0 && y == 0 && r == 0) {
                        field[x][y][r] = true;
                        continue;
                    }

                    if (!field[x][y][r]) {
                        continue;
                    }

                    if ((x == 0 || !field[x - 1][y][r]) && (y == 0 || !field[x][y - 1][r]) && (r == 0 || !field[x][y][r - 1])) {
                        field[x][y][r] = false;
                    }
                }
            }

            if (field[inputKit.fieldHeight - 1][inputKit.fieldWidth - 1][r]) {
                output << inputKit.fieldHeight + inputKit.fieldWidth - 2 + r << '\n';
                break;
            }

            if (r == inputKit.railgunsInfo.size()) {
                output << -1 << '\n';
            }
        }
    }
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 \
1 3\n4\n1 2 0\n2 2 1\n3 2 2\n4 1 1 \
3 3\n6\n2 1 0\n2 1 1\n2 1 2\n2 2 0\n2 2 1\n2 2 2 \
2 1\n3\n7 1 2\n2 1 1\n7 2 1 \
2 2\n5\n9 1 2\n3 2 0\n5 1 2\n4 2 2\n7 1 0 \
4 6\n7\n6 1 2\n12 1 3\n4 1 0\n17 2 3\n1 2 6\n16 2 6\n3 2 4";
        subMain(input, output);
        std::cout << ">>> " << output.str() << '\n';
        assert(output.str() == "5\n-1\n3\n4\n10\n");
    }
}

int main() {
    test();
    // subMain(std::cin, std::cout);
    return 0;
}
