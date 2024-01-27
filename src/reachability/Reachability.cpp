#include "Reachability.h"
#include <stdexcept>
#include <iostream>

using namespace ClassProject;

    Reachability::Reachability(unsigned int stateSize, unsigned int inputSize) : ReachabilityInterface(stateSize,inputSize) {

            try
            {
                if(stateSize == 0)
                {
                    throw std::runtime_error("ReachabilityInterface(): stateSize = 0");
                }
                for(int i = 0; i < stateSize; i++)
                {
                    std::string label = "s";
                    label.append(std::to_string(i)); //Give label names: s0, s1, s2 ...
                    BDD_ID id = Manager::createVar(label); //Create state variables
                    current_states.push_back(id);
                    initial_states.push_back(false); //Set initial state to false
                    if(inputSize == 0) //Set next state to be current state if no input variables
                    {
                        next_states.push_back(id);
                    }
                    tran_func.push_back(Manager::True()); //Identity function by default
                }
            }
            catch(std::exception const& e)
            {
                std::cout << "Exception: " << e.what() << "\n";
            }
        }
    const std::vector<BDD_ID> &Reachability::getStates() const{
        return current_states;
    }
    const std::vector<BDD_ID> &Reachability::getInputs() const{
        return next_states;
    }
    bool Reachability::isReachable(const std::vector<bool> &stateVector) {
        return false;
    }
    void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) {};

/**
    * Provides an initial state for the system as a vector of boolean values.
    * If the entry is true, the state bit is high. Otherwise, the bit is low.
    * E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
    *
    * @param stateVector provides the assignment for each state bit
    * @throws std::runtime_error if size does not match with number of state bits
    */
    void Reachability::setInitState(const std::vector<bool> &stateVector) {

        if (stateVector.size() != 2)
        {
            throw std::runtime_error("Size of stateVector does not match the number of state bits.");
        }

        for (size_t i = 0; i < stateVector.size(); ++i)
        {
              initial_states[i] = stateVector[i]  ? 1 : 0 ;
        }

    };

    int Reachability::stateDistance(const std::vector<bool> &stateVector) {
    return 0;
}