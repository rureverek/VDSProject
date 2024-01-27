#include "Reachability.h"

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
        };





    const std::vector<BDD_ID> &Reachability::getStates(){
        return current_states;
    };

    const std::vector<BDD_ID> &Reachability::getInputs(){
        return next_states;
    };
