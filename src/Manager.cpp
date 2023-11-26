#include "Manager.h"
#include <algorithm>

/**
 * @brief ClassProject.
 * 
 * Me gusta la fafa.
 * @param arg1 What the first argument to the function should be.
 * @return What you can expect to get back from the function.
 */

namespace ClassProject {

    Manager::Manager() {
    
        Table.emplace(Unique_Table_Key{0,0,0}, Unique_Table_Entry{"False", 0});

        Table.emplace(Unique_Table_Key{1,1,1}, Unique_Table_Entry{"True",1});
    }

    /**
     * @brief createVar
     * 
     * Creates a new variable with the given ID.
     * @param label String of the label of the variable.
     * @return returns the ID of the created variable.
     */

    BDD_ID Manager::createVar(const std::string &label) {

        Unique_Table_Key key_Var;
        BDD_ID ID = Table.size();
        key_Var.high = 1;
        key_Var.low = 0;
        key_Var.TopVar = ID;

        Unique_Table_Entry entry_Var;
        
        entry_Var.label = label;
        entry_Var.id = ID;

        Table.emplace(key_Var, entry_Var);
        return ID;
    };

    const BDD_ID &Manager::False() {
        static const BDD_ID ret = 0;
        return ret;
    }

    const BDD_ID &Manager::True() {
        static const BDD_ID ret = 1;
        return ret;
    }

    /**
     * @brief isConstant
     * 
     * Returns true if the node is a leaf node.
     * @param f ID of the node.
     * @return returns true if the node is a leaf node.
     */
    bool Manager::isConstant(BDD_ID f) {
        for (auto & it : Table) {
           if (it.second.id == f) {
               if (it.first.low == it.first.high) {
                   return true;
               } else {
                   return false;
               }
           }
       }
       return false;
    };

//     bool Manager::isVariable( BDD_ID x) {
//         if (Table[x].label.size() == 1 && std::all_of(Table[x].label.begin(), Table[x].label.end(), ::isalpha)) {
//             return true;
//         }
//         else {
//             return false;
//         }
//     }

}