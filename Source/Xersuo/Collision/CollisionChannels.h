#pragma once

#include "Engine/EngineTypes.h"

// A global mapping for custom collision channels so that the names are more expressive and match with DefaultEngine.ini 
namespace XersuoCollision
{
	inline constexpr ECollisionChannel Targeting = ECC_GameTraceChannel1;
}