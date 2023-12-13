#pragma once

#include "core/math/vector.h"
#include "player.h"

// use the R2 namespace for game funcs
namespace R2
{
	// server entity stuff
	class CBaseEntity;
	extern CBaseEntity* (*Server_GetEntityByIndex)(int index);

	extern CBasePlayer*(__fastcall* UTIL_PlayerByIndex)(int playerIndex);
} // namespace R2
