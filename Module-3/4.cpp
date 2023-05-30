#include <array>
#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

constexpr std::size_t fieldSide = 4;
constexpr std::size_t fieldSize = fieldSide * fieldSide;
constexpr std::array<char, fieldSize> finishFieldState{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class FieldState {
 public:
    explicit FieldState(const std::array<char, fieldSize>& field);
    explicit FieldState(const std::array<char, fieldSize>& field, std::size_t zeroPos);

    auto calculateManhattanDist() noexcept -> void;
    auto calculateHash() noexcept -> void;

    [[nodiscard]] auto canMoveLeft() const noexcept -> bool;
    [[nodiscard]] auto canMoveRight() const noexcept -> bool;
    [[nodiscard]] auto canMoveUp() const noexcept -> bool;
    [[nodiscard]] auto canMoveDown() const noexcept -> bool;

    [[nodiscard]] auto moveLeft() const noexcept -> FieldState;
    [[nodiscard]] auto moveRight() const noexcept -> FieldState;
    [[nodiscard]] auto moveUp() const noexcept -> FieldState;
    [[nodiscard]] auto moveDown() const noexcept -> FieldState;

    [[nodiscard]] auto isFinish() const noexcept -> bool;

    [[nodiscard]] auto operator<(const FieldState& other) const noexcept -> bool;
    [[nodiscard]] auto operator==(const FieldState& other) const noexcept -> bool;
    [[nodiscard]] auto operator!=(const FieldState& other) const noexcept -> bool;

    [[nodiscard]] auto getHash() const noexcept -> std::size_t;

 private:
    [[nodiscard]] static auto findZeroPos(const std::array<char, fieldSize>& field) noexcept -> std::size_t;

 private:
    std::array<char, fieldSize> m_field;
    std::size_t m_zeroPos;
    std::size_t m_manhattanDist;
    std::size_t m_hash;
};

FieldState::FieldState(const std::array<char, fieldSize>& field) : FieldState(field, FieldState::findZeroPos(field)) {}

FieldState::FieldState(const std::array<char, fieldSize>& field, std::size_t zeroPos) : m_field(field), m_zeroPos(zeroPos) {
    assert(m_zeroPos < fieldSize && m_field[m_zeroPos] == 0);

    calculateManhattanDist();
    calculateHash();
}

auto FieldState::findZeroPos(const std::array<char, fieldSize>& field) noexcept -> std::size_t {
    std::size_t zeroPos{fieldSize};
    for (size_t i = 0; i != fieldSize; ++i) {
        if (field[i] == 0) {
            zeroPos = i;
        }
    }
    return zeroPos;
}

auto FieldState::calculateManhattanDist() noexcept -> void {
    m_manhattanDist = 0;
    for (std::size_t i = 0; i != fieldSize; ++i) {
        m_manhattanDist +=
            std::abs(static_cast<int>(i / fieldSide) - static_cast<int>((m_field[i] + fieldSize - 1) / fieldSide % fieldSide));
        m_manhattanDist += std::abs(static_cast<int>(i % fieldSide) - static_cast<int>((m_field[i] + fieldSide - 1) % fieldSide));
    }
}

auto FieldState::calculateHash() noexcept -> void {
    m_hash = 0;
    for (const auto& cell : m_field) {
        m_hash = m_hash * 10 + cell;
    }
}

auto FieldState::canMoveLeft() const noexcept -> bool {
    return m_zeroPos % fieldSide != 3;
}

auto FieldState::canMoveRight() const noexcept -> bool {
    return m_zeroPos % fieldSide != 0;
}

auto FieldState::canMoveUp() const noexcept -> bool {
    return m_zeroPos < (fieldSize - fieldSide);
}

auto FieldState::canMoveDown() const noexcept -> bool {
    return m_zeroPos >= fieldSide;
}

auto FieldState::moveLeft() const noexcept -> FieldState {
    assert(canMoveLeft());

    auto newField = m_field;
    std::swap(newField[m_zeroPos], newField[m_zeroPos + 1]);
    return FieldState{newField, m_zeroPos + 1};
}

auto FieldState::moveRight() const noexcept -> FieldState {
    assert(canMoveRight());

    auto newField = m_field;
    std::swap(newField[m_zeroPos], newField[m_zeroPos - 1]);
    return FieldState{newField, m_zeroPos - 1};
}

auto FieldState::moveUp() const noexcept -> FieldState {
    assert(canMoveUp());

    auto newField = m_field;
    std::swap(newField[m_zeroPos], newField[m_zeroPos + fieldSide]);
    return FieldState{newField, m_zeroPos + fieldSide};
}

auto FieldState::moveDown() const noexcept -> FieldState {
    assert(canMoveDown());

    auto newField = m_field;
    std::swap(newField[m_zeroPos], newField[m_zeroPos - fieldSide]);
    return FieldState{newField, m_zeroPos - fieldSide};
}

auto FieldState::isFinish() const noexcept -> bool {
    return m_field == finishFieldState;
}

auto FieldState::operator<(const FieldState& other) const noexcept -> bool {
    return m_manhattanDist <= other.m_manhattanDist;
}

auto FieldState::operator==(const FieldState& other) const noexcept -> bool {
    return m_field == other.m_field;
}

auto FieldState::operator!=(const FieldState& other) const noexcept -> bool {
    return !operator==(other);
}

auto FieldState::getHash() const noexcept -> std::size_t {
    return m_hash;
}

struct FieldStateHash {
    [[nodiscard]] auto operator()(const FieldState& fieldState) const noexcept -> std::size_t { return fieldState.getHash(); }
};

auto run(std::istream& input, std::ostream& output) -> void {
    int cell{};
    std::array<char, fieldSize> field{};
    for (auto& fieldCell : field) {
        input >> cell;
        fieldCell = cell;
    }

    int N{};
    int K{};
    for (auto it1 = field.cbegin(); it1 != field.cend(); ++it1) {
        if (*it1 == 0) {
            K = std::distance(field.cbegin(), it1) / 4 + 1;
            continue;
        }
        for (auto it2 = std::next(it1); it2 != field.cend(); ++it2) {
            if (*it2 != 0 && *it1 > *it2) {
                ++N;
            }
        }
    }

    if ((N + K) % 2 == 1) {
        output << -1;
        return;
    }

    auto startFieldState = FieldState(field);
    auto fieldState = startFieldState;
    auto nextFieldState = startFieldState;
    std::unordered_map<FieldState, char, FieldStateHash> visited{
        {startFieldState, '\0'}
    };
    std::set<FieldState> fieldQueue{{startFieldState}};

    while (!fieldState.isFinish()) {
        fieldState = *fieldQueue.begin();
        fieldQueue.erase(fieldQueue.begin());

        if (fieldState.canMoveUp()) {
            nextFieldState = fieldState.moveUp();
            if (!visited.count(nextFieldState)) {
                visited.insert({nextFieldState, 'U'});
                fieldQueue.insert(nextFieldState);
            }
        }
        if (fieldState.canMoveDown()) {
            nextFieldState = fieldState.moveDown();
            if (!visited.count(nextFieldState)) {
                visited.insert({nextFieldState, 'D'});
                fieldQueue.insert(nextFieldState);
            }
        }
        if (fieldState.canMoveLeft()) {
            nextFieldState = fieldState.moveLeft();
            if (!visited.count(nextFieldState)) {
                visited.insert({nextFieldState, 'L'});
                fieldQueue.insert(nextFieldState);
            }
        }
        if (fieldState.canMoveRight()) {
            nextFieldState = fieldState.moveRight();
            if (!visited.count(nextFieldState)) {
                visited.insert({nextFieldState, 'R'});
                fieldQueue.insert(nextFieldState);
            }
        }
    }

    std::string resultWay{};
    std::size_t lengthResultWay{};
    char nextStep{};
    while ((nextStep = visited.at(fieldState)) != 0) {
        ++lengthResultWay;
        resultWay += nextStep;
        switch (nextStep) {
            case 'U':
                fieldState = fieldState.moveDown();
                break;
            case 'D':
                fieldState = fieldState.moveUp();
                break;
            case 'L':
                fieldState = fieldState.moveRight();
                break;
            case 'R':
                fieldState = fieldState.moveLeft();
                break;
        }
    }

    output << lengthResultWay << '\n';

    for (auto it = resultWay.rbegin(); it != resultWay.rend(); ++it) {
        output << *it << ' ';
    }
}

auto test() -> void {
    {
        std::stringstream input;
        std::stringstream output;
        input << "11 9 4 8\n13 0 7 10\n3 12 15 6\n1 2 14 5";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "...\n...");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
