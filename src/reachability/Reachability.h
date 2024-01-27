#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    public:
        // Inherit all constructors from ReachabilityInterface
        //using ReachabilityInterface::ReachabilityInterface;
    Reachability (unsigned int stateSize, unsigned int inputSize);
    const std::vector<BDD_ID> &getStates() const override;
    const std::vector<BDD_ID> &getInputs() const override;
    bool isReachable(const std::vector<bool> &stateVector) override;
    void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;
    void setInitState(const std::vector<bool> &stateVector) override;

    private:

    std::vector<BDD_ID> current_states;
    std::vector<BDD_ID> next_states;
    std::vector<BDD_ID> tran_func;
    std::vector<bool> initial_states;


    };


}
#endif
