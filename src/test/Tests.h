//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

TEST(InitManagerInterface, ManagerInterface){

    ClassProject::ManagerInterface manager;
    

    EXPECT_EQ(manager.Table[0].label, "False");
    EXPECT_EQ(manager.Table[1].label, "True");
}


#endif
