#include "helpers.h"
#include <chrono>

namespace SecureBoxHack
{
namespace helpers
{
void logTimestamp(LogLevel level)
{
    using namespace std::chrono;
    if (logLevel <= level)
        return;

    std::cout << duration_cast<milliseconds>(steady_clock::now() -
                                             lastMessageTimestamp);
    lastMessageTimestamp = steady_clock::now();
}

void logMessage(std::string message, LogLevel level)
{
    using namespace std::chrono;

    if (logLevel < level)
        return;

    logTimestamp();
    std::cout << "\t" << message << "\n";
}

std::tuple<uint32_t, uint32_t> toCartesianCoordinates(std::size_t i,
                                                      std::size_t x)
{
    return {i / x, i % x};
}
} // namespace helpers
} // namespace SecureBoxHack
