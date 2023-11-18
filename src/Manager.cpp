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
        BDD_ID_Entry entry = {"False", 0, 0, 0, 0};
        Table.push_back(entry);
        entry = {"True", 1, 1, 1, 1};
        Table.push_back(entry);
    }

    BDD_ID Manager::createVar(const std::string &label) {

        BDD_ID ID = Table.size();
        BDD_ID_Entry entry_Var;
        entry_Var.label = label;
        entry_Var.id = ID;
        entry_Var.high = 1;
        entry_Var.low = 0;
        entry_Var.TopVar = ID;

        Table.push_back(entry_Var);
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

    bool isConstant(BDD_ID f) {

    };

    bool Manager::isVariable( BDD_ID x) {
        if (Table[x].label.size() == 1 && std::all_of(Table[x].label.begin(), Table[x].label.end(), ::isalpha)) {
            return true;
        }
        else {
            return false;
        }
    }

}