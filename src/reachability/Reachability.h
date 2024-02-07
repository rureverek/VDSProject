#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    public:

    Reachability(unsigned int stateSize) : Reachability(stateSize, 0) {};
    Reachability(unsigned int stateSize, unsigned int inputSize);

    const std::vector<BDD_ID> &getStates() const override;
    const std::vector<BDD_ID> &getInputs() const override;
    bool isReachable(const std::vector<bool> &stateVector) override;
    void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;
    void setInitState(const std::vector<bool> &stateVector) override;
    int stateDistance(const std::vector<bool> &stateVector) override;

    private:

    std::vector<BDD_ID> current_states;
    std::vector<BDD_ID> next_states;
    std::vector<BDD_ID> delta;
    BDD_ID tau;     //characteristic function of transition relation - tau
    std::vector<BDD_ID> inputs;     //fsm inputs
    std::vector<bool> initial_states;
    BDD_ID c_s;     //characteristic function of the initial state - c_s

    BDD_ID compute_img(BDD_ID img, const std::vector<BDD_ID> &states );
    };


}
#endif
