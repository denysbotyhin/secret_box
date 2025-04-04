#ifndef DynamicBitset_h
#define DynamicBitset_h

#include <algorithm>
#include <iostream>
#include <vector>

namespace SecureBoxHack
{
class DynamicBitset;
namespace helpers
{
inline std::ostream &operator<<(std::ostream &os, const DynamicBitset &bs);
}

/// @brief Helper class for the dynamic size bitset
class DynamicBitset
{
public:
    using ContainerType = uint64_t;

    /// @brief DynamicBitset constructor
    /// @param size The number of bits to be stored
    DynamicBitset(std::size_t size)
        : bitset(
              std::ceil(size / static_cast<float>(sizeof(ContainerType) * 8)),
              0),
          _s(size)
    {
    }

    /// @brief Set the bit value
    /// @param pos Zero-based bit index
    /// @param val New value. Default = true
    inline void set(ContainerType pos, bool val = true)
    {
        auto [field, shift] = getFieldShift(pos);
        bitset[field] =
            bitset[field] & ~(static_cast<ContainerType>(1) << shift);
        bitset[field] =
            bitset[field] | (static_cast<ContainerType>(val) << shift);
        return;
    }

    /// @brief Get the value of the bit in the position
    /// @param pos Zero-based bit index to be tested
    /// @return the bit value
    inline bool test(ContainerType pos) const
    {
        auto [field, shift] = getFieldShift(pos);
        return bitset[field] & (static_cast<ContainerType>(1) << shift);
    }

    /// @brief Returns the lastst's bit value
    /// @return The value of the last bit of the set
    inline bool back() const
    {
        auto [field, shift] = getFieldShift(_s - 1);
        return bitset[field] & (static_cast<ContainerType>(1) << shift);
    }

    /// @brief Return the size of the bitset
    /// @return the size of the bitset
    inline ContainerType size() const
    {
        return _s;
    }

    /// @brief Performs logic XOR operation on the pai of the bitsets.
    /// Throws the exaption if the size of the bitsets isn't equal
    /// @param other DinamicBitset instance of the same size
    /// @return the left-hand-side operand
    inline DynamicBitset &operator^=(const DynamicBitset &other)
    {
        std::transform(bitset.begin(),
                       bitset.end(),
                       other.bitset.begin(),
                       bitset.begin(),
                       std::bit_xor());
        return *this;
    }

    friend inline std::ostream &helpers::operator<<(std::ostream &os,
                                                    const DynamicBitset &bs);

private:
    // Container large enough to store the _s bits
    std::vector<ContainerType> bitset;
    // Bitset size
    std::size_t _s;

    /// @brief Returns the bit position as the container ID and the shift inside the container
    /// @param i Zero based bit position
    /// @return the tuple of elements {ontainer ID, bit offset}
    inline std::tuple<std::size_t, short> getFieldShift(std::size_t i) const
    {
        return {i / 64, i % 64};
    }
};
} // namespace SecureBoxHack

#endif
