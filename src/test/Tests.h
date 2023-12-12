//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

/**
 * @brief InitManager Class
 *
 */
class InitManager : public testing::Test
{
protected:
    /**
     * @brief Set the Up object
     *
     */
    void SetUp() override
    {
        manager = new ClassProject::Manager;
    }

    /**
     * @brief Deletes manager
     *
     */
    void TearDown() override
    {
        delete manager;
    }

    ClassProject::Manager *manager;
};

/**
 * @brief TestManager Class
 * Sets up main scenario of the system: nodes a,b,c,d, a+b, c*d, b*c*d
 */
class TestManager : public testing::Test
{
protected:
    /**
     * @brief Set the Up object
     *
     */
    void SetUp() override
    {
        manager = new ClassProject::Manager;
        a = manager->createVar("a");
        b = manager->createVar("b");
        c = manager->createVar("c");
        d = manager->createVar("d");
        ab = manager->or2(a,b);
        cd = manager->and2(c,d);
        f = manager->and2(ab,cd);
    }

    /**
     * @brief Deletes manager
     *
     */
    void TearDown() override
    {
        delete manager;
    }

    ClassProject::Manager *manager;
    ClassProject::BDD_ID a,b,c,d,ab,cd,f;
};

/**
 * @brief InitTableEntries Test
 *
 */
TEST_F(InitManager, InitTableEntries)
{
    ClassProject::Manager::Unique_Table_Key key = {0, 0, 0};

    EXPECT_EQ(manager->Table[key].id, 0);

    key = {1, 1, 1};
    EXPECT_EQ(manager->Table[key].id, 1);
}

/**
 * @brief InitTableSize Test
 *
 */
TEST_F(InitManager, InitTableSize)
{

    EXPECT_EQ(manager->Table.size(), 2);
}

/**
 * @brief CreateVar Test
 *
 */
TEST_F(InitManager, CreateVar)
{

    ClassProject::BDD_ID ID = manager->createVar("a");
    EXPECT_EQ(ID, 2);
    // EXPECT_EQ(manager.Table[2].id, ID);
    // EXPECT_EQ(manager.Table[2].label, "a");
}

/**
 * @brief TrueID Test
 *
 */
TEST_F(InitManager, TrueID)
{
    ClassProject::BDD_ID id = manager->True();

    EXPECT_EQ(id, 1);
}

/**
 * @brief FalseID Test
 *
 */
TEST_F(InitManager, FalseID)
{

    ClassProject::BDD_ID id = manager->False();

    EXPECT_EQ(id, 0);
}

/**
 * @brief isConstant Test
 *
 */
TEST_F(InitManager, isConstant)
{
    manager->Table[{2, 0, 1}] = {"a", 2};
    manager->Table[{3, 0, 1}] = {"ab", 3};

    EXPECT_TRUE(manager->isConstant(0)); //ID's 0 and 1 are already created
    EXPECT_TRUE(manager->isConstant(1));
    EXPECT_FALSE(manager->isConstant(2));
}

/**
 * @brief isVariable Test
 *
 */
TEST_F(InitManager, isVar)
{
    manager->Table[{2, 0, 1}] = {"a", 2};
    manager->Table[{3, 0, 1}] = {"ab", 3};

    EXPECT_FALSE(manager->isVariable(0));
    EXPECT_FALSE(manager->isVariable(1));
    EXPECT_TRUE(manager->isVariable(2));
    EXPECT_FALSE(manager->isVariable(3));
}

/**
 * @brief topVar Test
 *
 */

TEST_F(TestManager, topVar) {

    EXPECT_EQ(manager->topVar(a), a);
    EXPECT_EQ(manager->topVar(b), b);
    EXPECT_EQ(manager->topVar(c), c);
    EXPECT_EQ(manager->topVar(d), d);
    EXPECT_EQ(manager->topVar(ab), a);
    EXPECT_EQ(manager->topVar(cd), c);

}

//test ite function
TEST_F(TestManager, ite_terminal_case) {

    EXPECT_EQ(manager->ite(manager->True(), manager->True(), a) , manager->True()); //Test i==True Cases - Terminal
    EXPECT_EQ(manager->ite(manager->True(), d, manager->True()) , d); //Test i==True Cases - Not Terminal

    EXPECT_EQ(manager->ite(manager->False(), a, manager->False()) , manager->False()); //Test i==False Case - Terminal
    EXPECT_EQ(manager->ite(manager->False(), c, a) , a); //Test i==False Case - Not Terminal

    EXPECT_EQ(manager->ite(4, manager->True(), manager->False()) , c); //Test Node with 1,0 subnodes

    EXPECT_EQ(manager->ite(a, manager->True(), b) , ab); //Test Existing node (a+b), not terminal

}

TEST_F(InitManager, ite) {

    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");

    EXPECT_EQ(manager->Table.size(),6);
    EXPECT_EQ(manager->computed_table.size(), 0);

    /* a+b */
    EXPECT_EQ(manager->ite(a, b, manager->False()) , 6);
    EXPECT_EQ(manager->computed_table.size(), 1);

    /* c*d */
    EXPECT_EQ(manager->ite(c, d, manager->False()) , 7);
    EXPECT_EQ(manager->computed_table.size(), 2);
    EXPECT_EQ(manager->Table.size(), 8);

    /* b*c*d */
    EXPECT_EQ(manager->ite(6, 1, 7) , 9);
    EXPECT_EQ(manager->computed_table.size(), 4);

    /* Call again */
    EXPECT_EQ(manager->ite(6, 1, 7) , 9);
    EXPECT_EQ(manager->computed_table.size(), 4);
    EXPECT_EQ(manager->ite(6, 1, 7) , 9);
    EXPECT_EQ(manager->computed_table.size(), 4);
    EXPECT_EQ(manager->Table.size(),10);
}

/**
* @brief getTopVarName Test
*
*/

TEST_F(TestManager, getTopVarName)
{

    EXPECT_EQ(manager->getTopVarName(a), "a");
    EXPECT_EQ(manager->getTopVarName(b), "b");
    EXPECT_EQ(manager->getTopVarName(ab), "a");
    EXPECT_EQ(manager->getTopVarName(cd), "c");

}

TEST_F(InitManager, CoFactorTrue) {

    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");
    ClassProject::BDD_ID ab = manager->or2(a,b);

    EXPECT_EQ(manager->coFactorTrue(manager->False(),c), manager->False()); //Terminal Case - f = 0
    EXPECT_EQ(manager->coFactorTrue(manager->True(),b), manager->True()); //Terminal Case - f = 1
    EXPECT_EQ(manager->coFactorTrue(manager->False(),manager->False()), manager->False()); //Terminal Case - x = 0
    EXPECT_EQ(manager->coFactorTrue(d,manager->True()), d); //Terminal Case - x = 1

    EXPECT_EQ(manager->coFactorTrue(ab,a), manager->True()); //Case: f.top == x ret f.high

    //Case: if else: ret ite(f_key.TopVar, T, F);
    EXPECT_EQ(manager->coFactorTrue(ab,d), manager->ite(a, manager->coFactorTrue(manager->True(), a), manager->coFactorTrue(b,a))); //b, d

}

TEST_F(InitManager, CoFactorFalse) {

    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");
    ClassProject::BDD_ID ab = manager->or2(a,b);

    EXPECT_EQ(manager->coFactorFalse(manager->False(),d), manager->False()); //Terminal Case - f = 0
    EXPECT_EQ(manager->coFactorFalse(manager->True(),c), manager->True()); //Terminal Case - f = 1
    EXPECT_EQ(manager->coFactorFalse(manager->False(),manager->False()), manager->False()); //Terminal Case - x = 0
    EXPECT_EQ(manager->coFactorFalse(d,manager->True()), d); //Terminal Case - x = 1

    EXPECT_EQ(manager->coFactorFalse(ab,a), b); //Case: f.top == x ret f.low

    //Case: if else: ret ite(f_key.TopVar, T, F);
    EXPECT_EQ(manager->coFactorFalse(ab,d), manager->ite(a, manager->coFactorFalse(b, d), manager->coFactorFalse(manager->True(),d)));

}



TEST_F(TestManager, findNodes){

    std::set<ClassProject::BDD_ID> nodes_of_root,vars_of_root;

    std::set<ClassProject::BDD_ID> correct_answer = {0, 1, 5, 7, 8, 9};

    manager->findNodes(f, nodes_of_root);
    
    for(auto i : nodes_of_root){
        std::cout << ' ' << i;
    }
    std::cout << '\n';
    
    EXPECT_EQ(correct_answer, nodes_of_root);

    manager->findVars(f, vars_of_root);

    for (auto i : vars_of_root)
    {
        std::cout << ' ' << i;
    }
    std::cout << '\n';
    
    correct_answer = { 2, 3, 4, 5 };

    EXPECT_EQ(correct_answer, vars_of_root);
}


/**
 * @brief neg Test
 *
 */

TEST_F(TestManager, neg)
{

    EXPECT_EQ(manager->neg(a), manager->ite(a, manager->False(), manager->True()));
    EXPECT_EQ(manager->neg(ab), manager->ite(ab, manager->False(), manager->True()));
    EXPECT_EQ(manager->neg(cd), manager->ite(cd, manager->False() ,manager->True()));
}

/**
 * @brief and2 Test
 *
 */

TEST_F(TestManager, and2)
{

    EXPECT_EQ(manager->and2(a, b), manager->ite(a, b, manager->False()));
    EXPECT_EQ(manager->and2(c, d), manager->ite(c, d, manager->False()));
    EXPECT_EQ(manager->and2(a, c), manager->ite(a, c, manager->False()));
    EXPECT_EQ(manager->and2(cd, f), manager->ite(cd, f ,manager->False()));
}

/**
 * @brief or2 Test
 *
 */

TEST_F(TestManager, or2)
{

    EXPECT_EQ(manager->or2(a, b), manager->ite(a, manager->True(), b));
    EXPECT_EQ(manager->or2(c, d), manager->ite(c, manager->True(), d));
    EXPECT_EQ(manager->or2(cd, f), manager->ite(cd, manager->True() ,f));
    EXPECT_EQ(manager->or2(cd, f), manager->ite(cd, manager->True() ,f));
}

/**
 * @brief xor2 Test
 *
 */

TEST_F(TestManager, xor2) //ite(a, ~b, b)
{

    EXPECT_EQ(manager->xor2(a, b), manager->ite(a, manager->neg(b), b));
    EXPECT_EQ(manager->xor2(c, d), manager->ite(c, manager->neg(d), d));
    EXPECT_EQ(manager->xor2(cd, f), manager->ite(cd, manager->neg(f) ,f));
}


/**
 * @brief nor2 Test
 *
 */

TEST_F(TestManager, nor2) //ite(a, 0, ~b)
{

    EXPECT_EQ(manager->nor2(a, b), manager->ite(a, manager->False(), manager->neg(b)));
    EXPECT_EQ(manager->nor2(c, d), manager->ite(c,manager->False(), manager->neg(d)));
    EXPECT_EQ(manager->nor2(cd, f), manager->ite(cd,manager->False(), manager->neg(f)));
}


/**
 * @brief xnor2 Test
 *
 */

TEST_F(TestManager, xnor2) //ite(a, b, ~b)
{

    EXPECT_EQ(manager->xnor2(a, b), manager->ite(a, b, manager->neg(b)));
    EXPECT_EQ(manager->xnor2(c, d), manager->ite(c, d, manager->neg(d)));
    EXPECT_EQ(manager->xnor2(cd, f), manager->ite(cd, f, manager->neg(f)));
};

/**
 * @brief xnor2 Test
 *
 */

TEST_F(TestManager, nand2) //ite(a, ~b, 1)
{
    EXPECT_EQ(manager->nand2(manager->False(), manager->False()), manager->True());
    EXPECT_EQ(manager->nand2(manager->False(), manager->True()), manager->True());
    EXPECT_EQ(manager->nand2(manager->True(), manager->False()), manager->True());
    EXPECT_EQ(manager->nand2(manager->True(), manager->True()), manager->False());

    EXPECT_EQ(manager->nand2(a, manager->False()), manager->True());
    EXPECT_EQ(manager->nand2(a, manager->True()), manager->neg(a));
    EXPECT_EQ(manager->nand2(manager->False(), a), manager->True());
    EXPECT_EQ(manager->nand2(manager->True(), a), manager->neg(a));

    ClassProject::BDD_ID  a_nand_b = manager->nand2(a, b);

    EXPECT_EQ(manager->coFactorTrue(a_nand_b), manager->neg(b));
    EXPECT_EQ(manager->coFactorFalse(a_nand_b), manager->True());
    EXPECT_EQ(manager->topVar(a_nand_b), a);

}
#endif
