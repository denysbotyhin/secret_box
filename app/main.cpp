#include "BoxHack.h"
#include "SecureBox.h"
#include "helpers.h"
#include <cstring>
#include <iostream>

using namespace SecureBoxHack;

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
    SecureBox box(y, x);
    auto state = box.getState();

    helpers::logMatrix(state, "Initial SecureBox state: ");

    auto hack = BoxHack(state);
    auto toggleSeq = hack.getUnlockSequence();

    for (auto [posY, posX] : toggleSeq)
    {
        char buffer[100];
        snprintf(buffer, 100, "Toggle cell in position (%u, %u)", posY, posX);
        helpers::logMessage(buffer, helpers::LogLevel::DEBUG);
        box.toggle(posY, posX);
    }

    return box.isLocked();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Bad usage! The program requires two unsigned integers to "
                     "build the box"
                  << std::endl;
        return 1;
    }

    uint32_t y = static_cast<uint32_t>(std::atol(argv[1]));
    uint32_t x = static_cast<uint32_t>(std::atol(argv[2]));

    if (x == 0 || y == 0)
        return 1;

    if (argc > 3)
    {
        if (std::strcmp(argv[3], "info") == 0)
        {
            helpers::logLevel = helpers::LogLevel::INFO;
        }
        else if (std::strcmp(argv[3], "debug") == 0)
        {
            helpers::logLevel = helpers::LogLevel::DEBUG;
        }
    }

    bool state = openBox(y, x);

    if (state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return state;
}
