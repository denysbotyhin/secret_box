//
//  main.cpp
//  CMakeSandbox
//
//  Created by Denys on 07.01.2025.
//

#include "SecureBox.h"
#include "BoxHack.h"
#include "helpers.h"

#include <gtest/gtest.h>
#include <random>
#include <time.h>
#include <ranges>

using namespace SecureBoxHack;

GTEST_TEST(SecureBoxTests, TestsUnder10)
{
    std::mt19937 rng(time(0));

    for (int i = 0; i < 200; i++)
    {
        uint32_t y = rng() % 10 + 1, x = rng() % 10 + 1;
        SecureBox box(y, x);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join, [](const auto val)
                                { return val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());

        if (box.isLocked() && !toggleSeq.empty())
        {
            helpers::logLevel = helpers::LogLevel::DEBUG;
            char buffer[100];
            snprintf(buffer, 100, "test failed with y = %u, x = %u, matrix:", y, x);
            helpers::logMatrix(state);
            helpers::logLevel = helpers::LogLevel::OFF;
        }
    }
}

GTEST_TEST(SecureBoxTests, SquareMatrix10_20)
{
    std::mt19937 rng(time(0));

    for (int i = 0; i < 20; i++)
    {
        uint32_t y = rng() % 10 + 10;
        SecureBox box(y, y);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join, [](const auto val)
                                { return val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());

        if (box.isLocked() && !toggleSeq.empty())
        {
            helpers::logLevel = helpers::LogLevel::DEBUG;
            char buffer[100];
            snprintf(buffer, 100, "test failed with y = %u, x = %u, matrix:", y, y);
            helpers::logMatrix(state);
            helpers::logLevel = helpers::LogLevel::OFF;
        }
    }
}

TEST(SecureBoxTests, TestsUnder30_50)
{
    std::mt19937 rng(time(0));

    for (int i = 0; i < 10; i++)
    {
        uint32_t y = rng() % 20 + 30, x = rng() % 20 + 30;
        SecureBox box(y, x);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join, [](const auto val)
                                { return val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());

        if (box.isLocked() && !toggleSeq.empty())
        {
            helpers::logLevel = helpers::LogLevel::DEBUG;
            char buffer[100];
            snprintf(buffer, 100, "test failed with y = %u, x = %u, matrix:", y, x);
            helpers::logLevel = helpers::LogLevel::OFF;
        }
    }
}

TEST(SecureBoxTests, TestsUnder50_100)
{
    std::mt19937 rng(time(0));

    for (int i = 0; i < 5; i++)
    {
        uint32_t y = rng() % 50 + 50, x = rng() % 50 + 50;
        SecureBox box(y, x);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join, [](const auto val)
                                { return val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());

        if (box.isLocked() && !toggleSeq.empty())
        {
            helpers::logLevel = helpers::LogLevel::INFO;
            char buffer[100];
            snprintf(buffer, 100, "test failed with y = %u, x = %u, matrix:", y, x);
            helpers::logLevel = helpers::LogLevel::OFF;
        }
    }
}
