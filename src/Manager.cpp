#include "Manager.h"
#include <algorithm>

/**
 * @brief ClassProject Class.
 *
 * Me gusta la fafa.
 * @return What you can expect to get back from the function.
 */

namespace ClassProject
{

    Manager::Manager()
    {

        Table.emplace(Unique_Table_Key{0, 0, 0}, Unique_Table_Entry{"False", 0});

        Table.emplace(Unique_Table_Key{1, 1, 1}, Unique_Table_Entry{"True", 1});
    }

    /**
     * @brief createVar
     *
     * Creates a new variable with the given ID.
     * @param label String of the label of the variable.
     * @return returns the ID of the created variable.
     */
  
    

    BDD_ID Manager::createVar(const std::string &label)
    {
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

    /**
     * @brief False
     *
     * Returns the id of the False node
     * @return const BDD_ID&
     */
    const BDD_ID &Manager::False()
    {
        static const BDD_ID ret = 0;
        return ret;
    }

    /**
     * @brief True
     *
     * Returns the id of the True node
     * @return const BDD_ID&
     */
    const BDD_ID &Manager::True()
    {
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
    bool Manager::isConstant(BDD_ID f)
    {
        for (auto &it : Table)
        {
            if (it.second.id == f)
            {
                if (it.first.low == it.first.high)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    };

    /**
     * @brief isVariable
     *
     * checks whether the item corresponding to a given ID is a variable or not.
     * @param x: ID of the node.
     * @return returns true if the ID represents a variable.
     */
   bool Manager::isVariable( BDD_ID x) {

       for (auto & it : Table) {
           if (it.second.id == x) {
               if (it.second.label.size() == 1 &&
                   std::all_of(it.second.label.begin(), it.second.label.end(), ::isalpha)) {
                   return true;
               } else {
                   return false;
               }
           }
       }
       return false;
   }

    /**
      * @brief topVar
      *
      * returns the top variable ID of the given node.
      * @param f: ID of the node.
      * @return returns the ID of the top variable.
      */
    BDD_ID Manager::topVar(BDD_ID f) {
        if (isVariable(f) || isConstant(f)) {
            return f;
        }

        else {
            for (auto & it : Table) {
                if(it.second.id == f)
                {
                    return it.first.TopVar;
                }
            }
        }


    }

    /**
    * @brief getTopVarName
    *
    * returns the name of the top variable of the given node.
    * @param root: ID of the node.
    * @return returns the name of the top variable.
    */
    std::string Manager::getTopVarName(const BDD_ID &root) {
        for (auto &it: Table) {
            if (it.second.id == root ) {
                return it.second.label;
            }
        }
    }

    /**
    * @brief and2
    *
    * returns the id numbers required for the ite operation
    * @param &a and &b: variable names
    * @return returns the id numbers required for the ite operation
    */
    BDD_ID Manager::and2(const std::string &a, const std::string &b) {
        BDD_ID f_op, s_op;

        for (auto& it: Table ) {
            if (it.second.label == a)
            {
                f_op = it.second.id;
            }

            if (it.second.label == b)
            {
                s_op = it.second.id;
            }
        }
        return ite(f_op, s_op, 0);
    }

}
        