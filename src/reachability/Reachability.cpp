#include "Reachability.h"
#include <stdexcept>
#include <iostream>

using namespace ClassProject;

    Reachability::Reachability(unsigned int stateSize, unsigned int inputSize) : ReachabilityInterface(stateSize,inputSize) {

                if(stateSize == 0)
                {
                    throw std::runtime_error("ReachabilityInterface(): stateSize = 0");
                }
                for(int i = 0; i < stateSize; i++)
                {
                    std::string label = "s";
                    label.append(std::to_string(i)); //Give label names: s0, s1, s2 ...
                    BDD_ID s = Manager::createVar(label); //Create state variables
                    label.append("'");
                    BDD_ID next_s = Manager::createVar(label); //Create next state variables
                    current_states.push_back(s);
                    initial_states.push_back(false); //Set initial state to false
                    next_states.push_back(next_s);
                }
                for(size_t i = 0; i < inputSize; i++ ){
                    std::string label = "input";
                    label.append(std::to_string(i)); //Give inputs names: input0, input1, input2 ...
                    label.append("'");
                    BDD_ID id = Manager::createVar(label); //Create inputs
                    inputs.push_back(id); 
                }
                setInitState(initial_states);
                setTransitionFunctions(current_states);
        }
    const std::vector<BDD_ID> &Reachability::getStates() const{
        return current_states;
    }
    const std::vector<BDD_ID> &Reachability::getInputs() const{
        return inputs;
    }
    bool Reachability::isReachable(const std::vector<bool> &stateVector) {
        return (stateDistance(stateVector) != -1);
    }


/**
     * Each state variable has a transition function.
     * The transition function specifies the value of the state after the transition.
     * The transition functions can be composed of state variables and inputs if present.
     * Example: s0' = s0 XOR s1
     * Example: s1' = (s1 AND s0) OR i0
     * The next state for s0 is defined as XOR of the current values of the state bit s0 and s1
     * The next state for s1 is defined as the AND of the current values of s1 and s0 ORed with input i0
     * An exception is thrown, if
     *  - The number of given transition functions does not match the number of state bits
     *  - An unknown ID is provided
     *
     * @param transitionFunctions provide a transition function exactly for each state bit
     * @throws std::runtime_error
     */
    void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) {

            if (transitionFunctions.size() != initial_states.size())
            {
                throw std::runtime_error("The number of transition functions does not match the number of state bits");
            }
            for(BDD_ID transitionFunction : transitionFunctions)
            {
                if(uniqueTableSize() <= transitionFunction)
                {
                    throw std::runtime_error("An unknown ID is provided");
                }
            }

            //Set transition function
            delta = transitionFunctions;

            //Calculate charateristic transition relation
            //Tau = (current_state[0] xnor next_state[0]) and (current_state [1] xnor next_state[1]) and ... [i]
            tau = Manager::xnor2(delta[0], next_states[0]);
            for (size_t i = 1; i < transitionFunctions.size(); ++i)
            {
                tau = Manager::and2(tau, Manager::xnor2(delta[i], next_states[i]));
            }
    };



/**
    * Provides an initial state for the system as a vector of boolean values.
    * If the entry is true, the state bit is high. Otherwise, the bit is low.
    * E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
    *
    * @param stateVector provides the assignment for each state bit
    * @throws std::runtime_error if size does not match with number of state bits
    */
    void Reachability::setInitState(const std::vector<bool> &stateVector) {

            if (stateVector.size() != initial_states.size())
            {
                throw std::runtime_error("Size of stateVector does not match the number of state bits.");
            }

            for (size_t i = 0; i < stateVector.size(); ++i)
            {
                initial_states[i] = stateVector[i] ? True() : False();
            }
            //Calculate characteristic function of the initial_state
            //c_s = (current_state[0] xnor initial_state[0]) and (current_state [1] xnor initial_state[1]) and ... [i]
            c_s = Manager::xnor2(current_states[0], initial_states[0]);
            for (size_t i = 1; i < stateVector.size(); ++i)
            {
                c_s = Manager::and2(c_s, Manager::xnor2(current_states[i], initial_states[i]));
            }
    };

    BDD_ID Reachability::compute_img(BDD_ID img, const std::vector<BDD_ID> &states )
    {
        // step 7

        for(auto & state : states)
        {
            img = or2(coFactorTrue(img,state),coFactorFalse(img,state));
        }

        return img;
    }

    int Reachability::stateDistance(const std::vector<bool> &stateVector) {
        BDD_ID CR_it = c_s; //c_r_it = c_s
        BDD_ID CR, img, reachable;

        if(stateVector == initial_states)return 0; //Trivial case
        int cnt = 1;
            if (stateVector.size() != current_states.size())
            {
                throw std::runtime_error("Size of stateVector does not match the number of state bits.");
            }
            do
            {
                // step 6
                CR = CR_it;

                            // step 7
            img = and2(CR, tau);

            // step 7-8
            img = compute_img(compute_img(img, current_states),inputs);

            // step 8
            for(int i = 0; i < current_states.size(); i++)
            {
                img = and2(img, xnor2(current_states[i],next_states[i]));
            }
            
            img = compute_img(compute_img(img, next_states),inputs);

                // step 9
                CR_it = or2(CR, img);

                reachable = img;
                for(size_t i = 0; i < current_states.size(); i++){
                    stateVector[i] ? reachable = coFactorTrue(reachable,current_states[i]) : reachable = coFactorFalse(reachable, current_states[i]);
                }
                if (reachable)return cnt;
                cnt++;
            }
            while (CR_it != CR ); //step 10

            return -1;
    }