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
                    label.append(std::to_string(i));
                    Manager::createVar(label);
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
    void Reachability::setInitState(const std::vector<bool> &stateVector) {}
    int Reachability::stateDistance(const std::vector<bool> &stateVector) {
    return 0;
}