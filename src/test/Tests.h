//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

TEST(HelloTest, BasicAssertions) {
    // Expect equality.
    EXPECT_EQ(5*7, 42);
}

#endif
