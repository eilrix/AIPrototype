#pragma once

#include "Delegates/Delegate.h"

template<class InducingDelegateType, class CatchingDelegateType, class... ParamTypes>
void Chain(UObject* ContextObject, InducingDelegateType& InducingDelegate, CatchingDelegateType& CatchingDelegate, ParamTypes... CatchingDelegateParams)
{
	InducingDelegate.AddWeakLambda(ContextObject, [&](auto&&...) 
	{
		CatchingDelegate.Broadcast(CatchingDelegateParams...);
	});
}