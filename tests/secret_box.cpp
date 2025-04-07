//
//  main.cpp
//  CMakeSandbox
//
//  Created by Denys on 07.01.2025.
//

#include "BoxHack.h"
#include "SecureBox.h"
#include "helpers.h"

#include <gtest/gtest.h>
#include <random>
#include <ranges>
#include <time.h>

using namespace SecureBoxHack;

std::mt19937 rng(static_cast<uint32_t>(time(0)));

GTEST_TEST(SecureBoxTests, TestsUnder10)
{
    for (int i = 0; i < 200; i++)
    {
        SecureBox box(rng() % 10 + 1, rng() % 10 + 1);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join,
                                [](const auto val) { return !val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());
    }
}

GTEST_TEST(SecureBoxTests, SquareMatrix10_20)
{
    for (int i = 0; i < 200; i++)
    {
        uint32_t y = rng() % 10 + 10;
        SecureBox box(y, y);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join,
                                [](const auto val) { return !val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());
    }
}

#ifdef BUILD_TYPE_RELEASE

TEST(SecureBoxTests, TestsUnder30_50)
{
    for (int i = 0; i < 10; i++)
    {
        SecureBox box(rng() % 20 + 30, rng() % 20 + 30);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join,
                                [](const auto val) { return !val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());
    }
}

TEST(SecureBoxTests, TestsUnder50_100)
{
    for (int i = 0; i < 10; i++)
    {
        SecureBox box(rng() % 50 + 50, rng() % 50 + 50);
        auto state = box.getState();

        if (std::ranges::all_of(state | std::views::join,
                                [](const auto val) { return !val; }))
            continue;

        BoxHack hack(state);
        auto toggleSeq = hack.getUnlockSequence();

        for (auto [posY, posX] : toggleSeq)
            box.toggle(posY, posX);

        if (!toggleSeq.empty())
            EXPECT_FALSE(box.isLocked());
        else
            EXPECT_TRUE(box.isLocked());
    }
}

#endif
