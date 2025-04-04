#ifndef helpers_h
#define helpers_h

#include "DynamicBitset.h"
#include <bitset>
#include <concepts>
#include <vector>

namespace SecureBoxHack
{
namespace helpers
{
enum class LogLevel
{
    FATAL,
    INFO,
    DEBUG
};
inline LogLevel logLevel = LogLevel::FATAL;
inline auto lastMessageTimestamp = std::chrono::steady_clock::now();

/// @brief Logs the time in milliseconds elapsed since the last call to the
/// logTimestamp() function or since the launch if no calls were made
void logTimestamp(LogLevel level = LogLevel::INFO);

/// @brief Prints the log message with the specified LogLevel
/// @param message the message to be displayed
/// @param level custom log level. Defaults to INFO
void logMessage(std::string message = "", LogLevel level = LogLevel::INFO);

/// @brief Prints the std::vector in the output stream
/// @tparam V - the value_type of the vector
/// @param os output stream
/// @param v the vector to be printed in the stream
/// @return the reference to the @os param
template <typename V>
std::ostream &operator<<(std::ostream &os, const std::vector<V> &v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<V>(os, " "));
    return os;
}

/// @brief Prints the DynamicBitset in the output stream
/// @param os output stream
/// @param bs the DynamicBitset to be printed in the stream
/// @return the reference to the @os param
inline std::ostream &operator<<(std::ostream &os, const DynamicBitset &bs)
{
    for (const uint64_t &field : bs.bitset)
        os << std::bitset<64>(field);
    return os;
}

/// @brief Prints the matrix in the output stream.
/// The log level required for the matrix output is DEBUG.
/// The log level required for the message output is INFO.
/// @tparam M - the 2D container type
/// @param m the matrix to be printed in the stream
/// @param message custom message to be displayed above the matrix
template <typename M>
void logMatrix(const M &m, std::string message = "")
{
    if (logLevel < LogLevel::INFO)
        return;

    logTimestamp();
    std::cout << "\t" << message << "\n";

    if (logLevel < LogLevel::DEBUG)
        return;

    for (const auto &row : m)
        std::cout << row << "\n";
    std::cout << "\n";
}

/// @brief Converts the flat index into {y, x} coordinates in x-sized column matrix
/// @param i The flat index
/// @param x the number of the columns in the matrix
/// @return tuple with the {y, x} matrix coordinates
std::tuple<uint32_t, uint32_t> toCartesianCoordinates(std::size_t i,
                                                      std::size_t x);
} // namespace helpers
} // namespace SecureBoxHack

#endif
