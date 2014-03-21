#pragma once

##include "util/Noncopyable.h"

namespace silk
{

	//A state models a vehicle state like - idle, armed, flying, RTL, position hold, recovery etc
	//The Transition between states are modeled through each states constructor. Armed takes as parameter an Idle or Flying state so they are the only defined transition.
	//Like this we have typechecked transition to avoid going to idle from flying
	//Each state defines it's exit points as it wishes
	//
	//The run method is supposed to process the state until a state changes occur - in which case it returns the new state
	//If null is returned the behavoir is UNDEFINED!!! Most probably Recovery will be set which might go to Idle or Land depending on the vehicle status.

class State : util::Noncopyable
{

	virtual ~State() {}

	//This executes the state.
	//The methos is supposed to return the new state but NEVER null.
	virtual std::unique_ptr<State> run() = 0;

};

}
