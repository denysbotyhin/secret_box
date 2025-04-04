#ifndef BoxHack_h
#define BoxHack_h

#include "types.h"
#include "DynamicBitset.h"

namespace SecureBoxHack
{
    /// @brief Helper class unlocking the SecureBox
    class BoxHack
    {
    public:
        /// @brief BoxHack constructor
        /// @param initialState The initial state of the box
        BoxHack(const BoolMatrix &initialState) : state(initialState),
                                                  y(initialState.size()), x(initialState[0].size()),
                                                  m(initialState.size() * initialState[0].size(),
                                                    DynamicBitset(initialState.size() * initialState[0].size() + 1)) {}

        /// @brief Hacks the SecureBox and returns the vector of tupples
        /// of the toggles that should be applied in order to unlock it
        /// @return vector of tupples representing (y, x) coordinates for toggle
        std::vector<std::tuple<uint32_t, uint32_t>> getUnlockSequence();

    private:
        // SeureBox initial lock state
        const BoolMatrix &state;
        // SecureBox dimentions
        const uint32_t y, x;
        // container for the generated Gaussian matrix of linear equations
        GaussMatrix m;

        /// @brief Generates the Gaussian Elimination Matrix
        /// Each row of this matrix represents the toggle effect of a single cell,
        /// i.e. it shows what cells will be flipped when we trigger a cell.
        /// The last column shows the initial lock value of the cell
        void buildGaussMatrix();

        /// @brief Fills the row of the Gauss matrix
        /// @param row The row to be filled
        /// @param rowI Zero based index of the row in the matrix
        void fillGaussRow(DynamicBitset &row, uint32_t rowI);

        /// @brief Adds the initial lock state into the last column of the Gauss matrix
        void fillInitialState();

        /// @brief Converts the Gauss matrix into the echelon form
        /// for solvind the liniar equations set
        void echelonGaussMatrix();
    };
}

#endif
