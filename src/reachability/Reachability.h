#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    public:
        Reachability() : ReachabilityInterface() {};
        Reachability(unsigned int stateSize) : ReachabilityInterface(stateSize,0) {};
        Reachability(unsigned int stateSize, unsigned int inputSize) : ReachabilityInterface(stateSize,inputSize) {};
    };

}
#endif
