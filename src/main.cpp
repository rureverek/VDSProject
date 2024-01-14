//
// Created by Carolina P. Nogueira 2016
//

/**
 * @brief Me gusta la fafa.
 * 
 * A detailed description of why this algorithm works and what behavior to expect.
 * @param arg1 What the first argument to the function should be.
 * @return What you can expect to get back from the function.
 */


#include <iostream>
#include <string>
#include "Manager.h"


int main(int argc, char* argv[])
{
    ClassProject::Manager manager;

    ClassProject::BDD_ID a = manager.createVar("a");
    ClassProject::BDD_ID b = manager.createVar("b");
    ClassProject::BDD_ID c = manager.createVar("c");
    ClassProject::BDD_ID d = manager.createVar("d");
    ClassProject::BDD_ID ab = manager.and2(a,b);
    ClassProject::BDD_ID cd = manager.and2(c,d);
    ClassProject::BDD_ID f = manager.or2(ab,cd);

    std::cout << "START!" << std::endl;

    /* Print Table */
    for(auto [key, value] : manager.Table){
        std::cout<<value<<":"<<key.high<<","<<key.low<<","<<key.TopVar<<std::endl;
    }

    /* Print graph */
    manager.visualizeBDD("../", f);
}