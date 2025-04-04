#include "BoxHack.h"
#include "helpers.h"
#include <chrono>
#include <ranges>
#include <thread>

using namespace SecureBoxHack;

std::vector<std::tuple<uint32_t, uint32_t>> BoxHack::getUnlockSequence()
{
    buildGaussMatrix();
    helpers::logMatrix(m, "Gaussian matrix builded");

    echelonGaussMatrix();
    helpers::logMatrix(m, "Echelon form builded");

    std::vector<bool> linearSystemSolution(m.size(), false);
    std::vector<std::tuple<uint32_t, uint32_t>> togglCells;

    std::size_t i = m.size() - 1;
    do
    {
        bool value = m[i].back();

        // the 'x' component is missing. leave the value as 0 and continue
        if (!m[i].test(i) && value)
            continue;

        for (std::size_t j = m[i].size() - 2; j != i; j--)
            if (m[i].test(j))
                value = value ^ linearSystemSolution[j];

        linearSystemSolution[i] = value;
        if (value)
            togglCells.push_back(helpers::toCartesianCoordinates(i, x));
    } while (i-- != 0);

    char buffer[100];
    snprintf(buffer,
             100,
             "Solution found. Reuires %lu toggles",
             togglCells.size());
    helpers::logMessage(buffer);

    return {togglCells.rbegin(), togglCells.rend()};
}

void BoxHack::buildGaussMatrix()
{
    for (std::size_t i = 0; i < y * x; i++)
        fillGaussRow(m[i], i);
    fillInitialState();
}

void BoxHack::fillGaussRow(DynamicBitset &row, std::size_t rowI)
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
    const std::size_t size = x * y;
    auto stateJoinView = state | std::views::join;
    auto cur = stateJoinView.begin();

    for (std::size_t i = 0; i < size; i++, cur++)
        m[i].set(size, *cur);
}

void BoxHack::echelonGaussMatrix()
{
    // std::atomic_uint32_t workerRow;

    for (uint32_t i = 0, j = 0; i < m.size() - 1; ++i, j = i)
    {
        // std::vector<uint32_t> addableRowIDs;
        // addableRowIDs.reserve(std::sqrt(m.size() / 2));

        for (; j < m.size() && !m[j].test(i); j++)
        { // leave empty
        }
        if (j == m.size())
            continue;
        if (j != i)
            std::swap(m[i], m[j]);
        else
            j++;

        // auto &row = m[i];

        for (; j < m.size(); j++)
            if (m[j].test(i))
                m[j] ^= m[i];
        // for (; j < m.size(); j++)
        //     if (m[j].test(i))
        //         addableRowIDs.push_back(j);

        // std::thread rowAddingWorker([&m, &addableRowIDs]() {});
    }
}
