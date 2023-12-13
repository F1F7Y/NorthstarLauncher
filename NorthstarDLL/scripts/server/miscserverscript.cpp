#include "squirrel/squirrel.h"
#include "masterserver/masterserver.h"
#include "server/auth/serverauthentication.h"
#include "dedicated/dedicated.h"
#include "client/r2client.h"
#include "server/r2server.h"

#include <filesystem>

ADD_SQFUNC("void", NSEarlyWritePlayerPersistenceForLeave, "entity player", "", ScriptContext::SERVER)
{
	const R2::CPlayer* pPlayer = g_pSquirrel<context>->template getentity<R2::CPlayer>(sqvm, 1);
	if (!pPlayer)
	{
		spdlog::warn("NSEarlyWritePlayerPersistenceForLeave got null player");

		g_pSquirrel<context>->pushbool(sqvm, false);
		return SQRESULT_NOTNULL;
	}

	R2::CClient* pClient = &R2::g_pClientArray[pPlayer->m_Network.m_edict - 1];
	if (g_pServerAuthentication->m_PlayerAuthenticationData.find(pClient) == g_pServerAuthentication->m_PlayerAuthenticationData.end())
	{
		g_pSquirrel<context>->pushbool(sqvm, false);
		return SQRESULT_NOTNULL;
	}

	g_pServerAuthentication->m_PlayerAuthenticationData[pClient].needPersistenceWriteOnLeave = false;
	g_pServerAuthentication->WritePersistentData(pClient);
	return SQRESULT_NULL;
}

ADD_SQFUNC("bool", NSIsWritingPlayerPersistence, "", "", ScriptContext::SERVER)
{
	g_pSquirrel<context>->pushbool(sqvm, g_pMasterServerManager->m_bSavingPersistentData);
	return SQRESULT_NOTNULL;
}

ADD_SQFUNC("bool", NSIsPlayerLocalPlayer, "entity player", "", ScriptContext::SERVER)
{
	const R2::CPlayer* pPlayer = g_pSquirrel<ScriptContext::SERVER>->template getentity<R2::CPlayer>(sqvm, 1);
	if (!pPlayer)
	{
		spdlog::warn("NSIsPlayerLocalPlayer got null player");

		g_pSquirrel<context>->pushbool(sqvm, false);
		return SQRESULT_NOTNULL;
	}

	R2::CClient* pClient = &R2::g_pClientArray[pPlayer->m_Network.m_edict - 1];
	g_pSquirrel<context>->pushbool(sqvm, !strcmp(R2::g_pLocalPlayerUserID, pClient->m_UID));
	return SQRESULT_NOTNULL;
}

ADD_SQFUNC("bool", NSIsDedicated, "", "", ScriptContext::SERVER)
{
	g_pSquirrel<context>->pushbool(sqvm, IsDedicatedServer());
	return SQRESULT_NOTNULL;
}

ADD_SQFUNC(
	"bool",
	NSDisconnectPlayer,
	"entity player, string reason",
	"Disconnects the player from the server with the given reason",
	ScriptContext::SERVER)
{
	const R2::CPlayer* pPlayer = g_pSquirrel<context>->template getentity<R2::CPlayer>(sqvm, 1);
	const char* reason = g_pSquirrel<context>->getstring(sqvm, 2);

	if (!pPlayer)
	{
		spdlog::warn("Attempted to call NSDisconnectPlayer() with null player.");

		g_pSquirrel<context>->pushbool(sqvm, false);
		return SQRESULT_NOTNULL;
	}

	// Shouldn't happen but I like sanity checks.
	R2::CClient* pClient = &R2::g_pClientArray[pPlayer->m_Network.m_edict - 1];
	if (!pClient)
	{
		spdlog::warn("NSDisconnectPlayer(): player entity has null CClient!");

		g_pSquirrel<context>->pushbool(sqvm, false);
		return SQRESULT_NOTNULL;
	}

	if (reason)
	{
		R2::CClient__Disconnect(pClient, 1, reason);
	}
	else
	{
		R2::CClient__Disconnect(pClient, 1, "Disconnected by the server.");
	}

	g_pSquirrel<context>->pushbool(sqvm, true);
	return SQRESULT_NOTNULL;
}
