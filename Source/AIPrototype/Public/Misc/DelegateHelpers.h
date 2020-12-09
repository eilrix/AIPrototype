#pragma once

#include "Delegates/Delegate.h"

/**
 * Chains multicast delegates broadcasting, duplicator waits for instigator and broadcasts in turn, optionally with required arguments.
 * Method is useful when you want to wait for some delegate but you are't interested in its params and don't want to bother with creation of a function/functor
 * with proper signature - you just need to catch some delegate notification and then broadcast your own delegate.
 */
template<class InstigatorDelegateType, class DuplicatorDelegateType, class... DuplicatorParamTypes>
void Chain(UObject* ContextObject, InstigatorDelegateType& Instigator, DuplicatorDelegateType& Duplicator, DuplicatorParamTypes... DuplicatorParams)
{
	Instigator.AddWeakLambda(ContextObject, [&](auto&&...) 
	{
		Duplicator.Broadcast(DuplicatorParams...);
	});
}