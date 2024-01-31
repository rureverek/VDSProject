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
                }
                for(int i = 0; i < stateSize; i++)
                {
                    std::string label = "s";
                    label.append(std::to_string(i)); //Give label names: s0, s1, s2 ...
                    label.append("'");
                    BDD_ID id = Manager::createVar(label); //Create next state variables
                    next_states.push_back(id);
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

        try
        {
            if (transitionFunctions.size() != initial_states.size())
            {
                throw std::runtime_error("The number of transition functions does not match the number of state bits");
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
        }
        catch(std::exception const& e)
        {
            std::cout << "Exception: " << e.what() << "\n";
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

        try
        {
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
        }
        catch(std::exception const& e)
        {
            std::cout << "Exception: " << e.what() << "\n";
        }
    };

    int Reachability::stateDistance(const std::vector<bool> &stateVector) {

        std::vector<BDD_ID> current_state = current_states;
        BDD_ID c_r_it = c_s; //c_r_it = c_s
        BDD_ID c_r;
        try
        {
            if (stateVector.size() != current_state.size())
            {
                throw std::runtime_error("Size of stateVector does not match the number of state bits.");
            }

            do {
                BDD_ID temp1, temp2, img_;
                /* cR(s) := cR_it(s); */
                c_r = c_r_it;

                /* imgR(s') := ∃x∃scR(s) ⋅τ(s, x, s'); */
                img_ = Manager::and2(c_r, tau);
                temp1 = True();
                for(int i = 0; i < stateVector.size(); i++)
                {
                    img_ = Manager::or2(Manager::coFactorTrue(img_, current_state[i]), Manager::coFactorFalse(img_, current_state[i]));
                    temp1 = Manager::and2(Manager::xnor2(current_state[i], next_states[i]), temp1);
                }

                /* form imgR(s) by renaming of variables s'into s; */
                
                img_ = Manager::and2(temp1, img_);
                for(int i = 0; i < stateVector.size(); i++)
                {
                    img_ = Manager::or2(Manager::coFactorTrue(img_, next_states[i]), Manager::coFactorFalse(img_, next_states[i]));
                }

                /* cR_it(s) := cR(s) + imgR(s); */
                c_r_it = Manager::or2(c_r, img_);
            } while (c_r != c_r_it);

            std::cout << "All reachable states: "<<c_r;
            //visualizeBDD("./", c_r);
        }
        catch(std::exception const& e)
        {
            std::cout << "Exception: " << e.what() << "\n";
        }
        return 0;
    }