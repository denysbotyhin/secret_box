#include "BoxHack.h"
#include "helpers.h"
#include <chrono>
#include <ranges>

using namespace SecureBoxHack;

std::vector<std::tuple<uint32_t, uint32_t>> BoxHack::getUnlockSequence()
{
    buildGaussMatrix();
    helpers::logMatrix(m, "Gaussian matrix builded");

    echelonGaussMatrix();
    helpers::logMatrix(m, "Echelon form builded");

    std::vector<bool> gaussMatrixSolution(m.size(), false);
    std::vector<std::tuple<uint32_t, uint32_t>> togglCells;

    uint32_t i = m.size() - 1;
    do
    {
        bool value = m[i].back();
        // the row has the form like 000.....1 which means the system has no solution
        if (!m[i].test(i) && value)
            return {};

        for (uint32_t j = m[i].size() - 2; j != i; j--)
            if (m[i].test(j))
                value = value ^ gaussMatrixSolution[j];

        gaussMatrixSolution[i] = value;
        if (value)
            togglCells.push_back(helpers::toCartesianCoordinates(i, x));
    } while (i-- != 0);

    char buffer[100];
    snprintf(buffer, 100, "Solution found. Reuires %lu toggles", togglCells.size());
    helpers::logMessage(buffer);

    return {togglCells.rbegin(), togglCells.rend()};
}

void BoxHack::buildGaussMatrix()
{
    uint32_t size = y * x;

    for (uint32_t i = 0; i < size; i++)
        fillGaussRow(m[i], i);
    fillInitialState();
}

void BoxHack::fillGaussRow(DynamicBitset &row, uint32_t rowI)
{
    // the cell coordinates
    auto [posY, posX] = helpers::toCartesianCoordinates(rowI, x);
    // fill the column
    for (uint32_t j = 0; j < y; j++)
        row.set(j * x + posX);
    // fill the row
    for (uint32_t j = 0; j < x; j++)
        row.set(posY * x + j);
}

void BoxHack::fillInitialState()
{
    const uint32_t size = x * y;
    auto stateJoinView = state | std::views::join;
    auto cur = stateJoinView.begin();

    for (uint32_t i = 0; i < size; i++, cur++)
        m[i].set(size, *cur);
}

void BoxHack::echelonGaussMatrix()
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
                //           << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count()
                //           << "ms\n";
            }
    }
}
