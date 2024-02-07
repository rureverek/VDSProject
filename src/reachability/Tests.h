#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"
#include <iostream>
using namespace ClassProject;

 struct ReachabilityTest : testing::Test {

     std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);

     std::vector<BDD_ID> stateVars2 = fsm2->getStates();
     std::vector<BDD_ID> transitionFunctions;
 };

 TEST_F(ReachabilityTest, HowTo_Example) { /* NOLINT */

     /* Create variables for current state and next state */
     BDD_ID s0 = stateVars2.at(0);
     BDD_ID s1 = stateVars2.at(1);

     transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
     transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)

     /* Compute BDD for transition function */
     fsm2->setTransitionFunctions(transitionFunctions);

     /* Compute BDD for characteristic function of the initial state */
     fsm2->setInitState({false,false});
    ASSERT_TRUE(fsm2->isReachable({false, false}));
    ASSERT_FALSE(fsm2->isReachable({false, true}));
    ASSERT_FALSE(fsm2->isReachable({true, false}));
    ASSERT_TRUE(fsm2->isReachable({true, true}));
 }

TEST_F(ReachabilityTest, StateDistanceExample) { /* NOLINT */

    /* Create variables for current state and next state */
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s1)); // s0' = not(s1)
    transitionFunctions.push_back(s0); // s1' = s0

    /* Compute BDD for transition function */
    fsm2->setTransitionFunctions(transitionFunctions);

    /* Compute BDD for characteristic function of the initial state */
    fsm2->setInitState({false,false});

    ASSERT_EQ(fsm2->stateDistance({false, false}), 0);
    ASSERT_EQ(fsm2->stateDistance({true, false}), 1);
    ASSERT_EQ(fsm2->stateDistance({true, true}), 2);
    ASSERT_EQ(fsm2->stateDistance({false, true}), 3);
}

TEST_F(ReachabilityTest, ExceptionCheck) { /* NOLINT */

    /* Create variables for current state and next state */
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    ASSERT_THROW(fsm2->setInitState({false}), std::runtime_error);

    fsm2->setInitState({false,false});
    transitionFunctions.push_back(fsm2->neg(s1)); // s0' = not(s1)

    ASSERT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    transitionFunctions.push_back(s0); // s1' = s0

    /* Compute BDD for transition function */
    fsm2->setTransitionFunctions(transitionFunctions);

    ASSERT_THROW(fsm2->isReachable({false,false,false}), std::runtime_error);
    ASSERT_THROW(fsm2->stateDistance({false}), std::runtime_error);
}

TEST(InitTest, InputExample) { /* NOLINT */

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2, 2);
    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    auto inputs = fsm2->getInputs();
    std::vector<BDD_ID> transitionFunctions;
    /* Create variables for current state and next state */
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);
    BDD_ID x0 = inputs.at(0);
    BDD_ID x1 = inputs.at(1);

    fsm2->setInitState({false,false});

    BDD_ID s0pluss1 = fsm2->or2(s0,s1);
    BDD_ID notx0 = fsm2->neg(x0);
    transitionFunctions.push_back(fsm2->and2(notx0, s0pluss1)); // s0' = not(x0) * (s0+s1)
    transitionFunctions.push_back(fsm2->and2(x1, fsm2->or2(x0, s0pluss1))); // s1' = x1 * (x0+s0+s1)

    /* Compute BDD for transition function */
    fsm2->setTransitionFunctions(transitionFunctions);

    ASSERT_EQ(fsm2->stateDistance({false, true}), 1);

}

TEST(InitTest, ConstructorCheck) { /* NOLINT */

     /* Call constructor with state_size = 0 */
    ASSERT_THROW(std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(0, 2), std::runtime_error);

}

#endif
