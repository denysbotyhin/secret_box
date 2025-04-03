#include "helpers.h"
#include <chrono>

namespace SecureBoxHack
{
    namespace helpers
    {
        void logTimestamp()
        {
            using namespace std::chrono;
            if (logLevel < LogLevel::INFO)
                return;

            std::cout << duration_cast<milliseconds>(steady_clock::now() - lastMessageTimestamp);
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

        std::tuple<uint32_t, uint32_t> toCartesianCoordinates(uint32_t i, uint32_t x)
        {
            return {i / x, i % x};
        }
    }
}
