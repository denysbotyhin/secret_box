#include <iostream>
#include <limits.h>
#include <random>
#include <ranges>
#include <time.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <math.h>
#include <bitset>

/*
You are given a locked container represented as a two-dimensional grid of
boolean values (true = locked, false = unlocked). Your task is to write an
algorithm that fully unlocks the box, i.e., transforms the entire matrix into
all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
    - Use the SecureBox public API (toggle, isLocked, getState).
    - Strategically toggle cells to reach a state where all elements are false.
    - Return true if the box remains locked, false if successfully unlocked.
You are not allowed to path or modify the SecureBox class.

Evaluation Criteria:
    - Functional correctness
    - Computational efficiency
    - Code quality, structure, and comments
    - Algorithmic insight and clarity
*/

class DynamicBitset;

using BoolMatrix = std::vector<std::vector<bool>>;
using GaussMatrix = std::vector<DynamicBitset>;

bool VERBOSE = false;

template <typename M>
void logMatrix(const M &m);
GaussMatrix getGaussMatrix(const BoolMatrix &state, uint32_t y, uint32_t x);
void fillGaussRow(DynamicBitset &row, uint32_t rowI, uint32_t y, uint32_t x);
void fillInitialState(GaussMatrix &gaussMatrix, const BoolMatrix &state);
inline void operator^(DynamicBitset &hs, const DynamicBitset &rhs);
void echelonGaussMatrix(GaussMatrix &m);
std::vector<bool> getToggleSequence(const GaussMatrix &gaussMatrix);
inline std::tuple<uint32_t, uint32_t> toCoordinates(uint32_t i, uint32_t y, uint32_t x);
template <typename V>
std::ostream &operator<<(std::ostream &os, const std::vector<V> &v);

class SecureBox
{
private:
    std::vector<std::vector<bool>> box;

public:
    //================================================================================
    // Constructor: SecureBox
    // Description: Initializes the secure box with a given size and
    //              shuffles its state using a pseudo-random number generator
    //              seeded with current time.
    //================================================================================
    SecureBox(uint32_t y, uint32_t x) : ySize(y), xSize(x)
    {
        // rng.seed(time(0));
        rng.seed(1);
        box.resize(y);
        for (auto &it : box)
            it.resize(x);
        shuffle();
    }

    //================================================================================
    // Method: toggle
    // Description: Toggles the state at position (x, y) and also all cells in
    // the
    //              same row above and the same column to the left of it.
    //================================================================================
    void toggle(uint32_t y, uint32_t x)
    {
        box[y][x] = !box[y][x];
        for (uint32_t i = 0; i < xSize; i++)
            box[y][i] = !box[y][i];
        for (uint32_t i = 0; i < ySize; i++)
            box[i][x] = !box[i][x];
    }

    //================================================================================
    // Method: isLocked
    // Description: Returns true if any cell
    //              in the box is true (locked); false otherwise.
    //================================================================================
    bool isLocked()
    {
        for (uint32_t x = 0; x < xSize; x++)
            for (uint32_t y = 0; y < ySize; y++)
                if (box[y][x])
                    return true;

        return false;
    }

    //================================================================================
    // Method: getState
    // Description: Returns a copy of the current state of the box.
    //================================================================================
    std::vector<std::vector<bool>> getState() { return box; }

private:
    std::mt19937_64 rng;
    uint32_t ySize, xSize;

    //================================================================================
    // Method: shuffle
    // Description: Randomly toggles cells in the box to
    // create an initial locked state.
    //================================================================================
    void shuffle()
    {
        for (uint32_t t = rng() % 1000; t > 0; t--)
            toggle(rng() % ySize, rng() % xSize);
    }
};

class DynamicBitset
{
public:
    using ContainerType = uint64_t;

    DynamicBitset(std::size_t size) : bitset(std::ceil(size / 64.0), 0), _s(size) {}

    inline void set(ContainerType pos, bool val = true)
    {
        auto [field, shift] = getFieldShift(pos);
        bitset[field] = bitset[field] & ~(static_cast<ContainerType>(1) << shift);
        bitset[field] = bitset[field] | (static_cast<ContainerType>(val) << shift);
        return;
    }

    inline bool test(ContainerType pos) const
    {
        auto [field, shift] = getFieldShift(pos);
        return bitset[field] & (static_cast<ContainerType>(1) << shift);
    }

    inline bool back() const
    {
        auto [field, shift] = getFieldShift(_s - 1);
        return bitset[field] & (static_cast<ContainerType>(1) << shift);
    }

    inline ContainerType size() const
    {
        return _s;
    }

    inline DynamicBitset &operator^=(const DynamicBitset &other)
    {
        std::transform(bitset.begin(), bitset.end(), other.bitset.begin(), bitset.begin(), std::bit_xor());
        return *this;
    }

    friend inline std::ostream &operator<<(std::ostream &o, const DynamicBitset &);

private:
    std::vector<ContainerType> bitset;
    std::size_t _s;

    std::tuple<std::size_t, short> getFieldShift(std::size_t i) const
    {
        std::size_t fieldId = i / 64;
        short bitShift = i % 64;
        return {fieldId, bitShift};
    }
};

template <typename M>
void logMatrix(const M &m)
{
    if (!VERBOSE)
        return;
    for (const auto &row : m)
    {
        std::cout << row << "\n";
    }
    std::cout << "\n";
}

template <typename V>
std::ostream &operator<<(std::ostream &os, const std::vector<V> &v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<V>(os, " "));
    return os;
}

std::ostream &operator<<(std::ostream &os, const DynamicBitset &bs)
{
    for (const uint64_t &field : bs.bitset)
        os << std::bitset<64>(field);
    return os;
}

GaussMatrix getGaussMatrix(const BoolMatrix &state, uint32_t y, uint32_t x)
{
    uint32_t size = y * x;
    GaussMatrix m(size, DynamicBitset(size + 1));

    // fill the gausian matrix
    // each row is the join view of the SecureBox state
    // as if the cell was triggered on the opened box
    // the last column is the initial lock state
    for (int i = 0; i < size; i++)
    {
        fillGaussRow(m[i], i, y, x);
        // fill the (size - i) row as the mirrored projection of the current row
        fillInitialState(m, state);
    }

    return m;
}

void fillGaussRow(DynamicBitset &row, uint32_t rowI, uint32_t y, uint32_t x)
{
    // the cell coordinates
    auto [posY, posX] = toCoordinates(rowI, y, x);
    // fill the column
    for (int j = 0; j < y; j++)
        row.set(j * x + posX);
    // fill the row
    for (int j = 0; j < x; j++)
        row.set(posY * x + j);
}

void fillInitialState(GaussMatrix &gaussMatrix, const BoolMatrix &state)
{
    const uint32_t size = state.size() * state[0].size();
    for (uint32_t i = 0; i < size; i++)
        gaussMatrix[i].set(size, state[i / state[0].size()][i % state[0].size()]);
}

void echelonGaussMatrix(GaussMatrix &m)
{
    for (uint32_t i = 0; i < m.size() - 1; i++)
    {
        uint32_t j = i;
        for (; j < m.size() && !m[j].test(i); j++)
        {
        }
        if (j == m.size())
            break;
        if (j != i)
            std::swap(m[i], m[j]);
        else
            j++;

        for (; j < m.size(); j++)
            if (m[j].test(i))
            {
                // const auto start = std::chrono::steady_clock::now();
                m[j] ^= m[i];
                // std::cout << "row xor: "
                // << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count()
                // << "ms\n";
            }
    }
}

std::vector<bool> getToggleSequence(const GaussMatrix &gaussMatrix)
{
    std::vector<bool> cellToggles(gaussMatrix.size(), 0);
    uint32_t i = gaussMatrix.size() - 1;
    do
    {
        const auto &row = gaussMatrix[i];
        bool value = gaussMatrix[i].back();
        // the row has the form like 000.....1 which means the system has no solution
        if (!gaussMatrix[i].test(i) && value)
            return {};

        for (uint32_t j = gaussMatrix[i].size() - 2; j != i; j--)
            if (gaussMatrix[i].test(j))
                value = value ^ cellToggles[j];

        cellToggles[i] = value;
    } while (i-- != 0);

    return cellToggles;
}

inline std::tuple<uint32_t, uint32_t> toCoordinates(uint32_t i, uint32_t y, uint32_t x)
{
    return std::make_tuple(i / x, i % x);
}

//================================================================================
// Function: openBox
// Description: Your task is to implement this function to unlock the
// SecureBox.
//              Use only the public methods of SecureBox (toggle, getState,
//              isLocked). You must determine the correct sequence of toggle
//              operations to make all values in the box 'false'. The function
//              should return false if the box is successfully unlocked, or
//              true if any cell remains locked.
//================================================================================
bool openBox(uint32_t y, uint32_t x)
{
    // using std::chrono::steady_clock;

    SecureBox box(y, x);

    // const auto start{steady_clock::now()};
    auto state = box.getState();
    logMatrix(state);
    // std::cout << "Box initialized in " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";
    auto m = getGaussMatrix(state, y, x);
    // std::cout << "Gauss matrix creation: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";
    logMatrix(m);
    // std::cout << "Log box state: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";
    echelonGaussMatrix(m);
    // std::cout << "Echelon Gauss matrix: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";
    logMatrix(m);
    // std::cout << "Log Gauss matrix: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";
    auto toggleSeq = getToggleSequence(m);
    // std::cout << "Find toggle sequence: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "ms\n";

    for (uint32_t i = 0; i < toggleSeq.size(); i++)
        if (toggleSeq[i])
        {
            auto [posY, posX] = toCoordinates(i, y, x);
            box.toggle(posY, posX);
        }

    // std::cout << "Apply toggle sequence: " << std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() << "seconds\n";

    return box.isLocked();
}

int main(int argc, char *argv[])
{
    uint32_t y = std::atol(argv[1]);
    uint32_t x = std::atol(argv[2]);

    if (argc > 2)
    {
        VERBOSE = std::strcmp(argv[3], "true") == 0;
    }

    bool state = openBox(y, x);

    if (state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return state;
}
