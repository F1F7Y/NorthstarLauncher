#pragma once
#include "tier1/convar.h"
#include "engine/client/client.h"

#include <unordered_map>
#include <string>

struct RemoteAuthData
{
	char uid[33];
	char username[64];

	// pdata
	char* pdata;
	size_t pdataSize;
};

struct PlayerAuthenticationData
{
	bool usingLocalPdata;
	size_t pdataSize;
	bool needPersistenceWriteOnLeave = true;
};

typedef int64_t (*CBaseServer__RejectConnectionType)(void* a1, unsigned int a2, void* a3, const char* a4, ...);
extern CBaseServer__RejectConnectionType CBaseServer__RejectConnection;

class ServerAuthenticationManager
{
  public:
	std::mutex m_AuthDataMutex;
	std::unordered_map<std::string, RemoteAuthData> m_RemoteAuthenticationData;
	std::unordered_map<CClient*, PlayerAuthenticationData> m_PlayerAuthenticationData;

	bool m_bAllowDuplicateAccounts = false;
	bool m_bNeedLocalAuthForNewgame = false;
	bool m_bForceResetLocalPlayerPersistence = false;
	bool m_bStartingLocalSPGame = false;

  public:
	void AddRemotePlayer(std::string token, uint64_t uid, std::string username, std::string pdata);

	void AddPlayer(CClient* pPlayer, const char* pAuthToken);
	void RemovePlayer(CClient* pPlayer);

	bool VerifyPlayerName(const char* pAuthToken, const char* pName, char pOutVerifiedName[64]);
	bool IsDuplicateAccount(CClient* pPlayer, const char* pUid);
	bool CheckAuthentication(CClient* pPlayer, uint64_t iUid, char* pAuthToken);

	void AuthenticatePlayer(CClient* pPlayer, uint64_t iUid, char* pAuthToken);
	bool RemovePlayerAuthData(CClient* pPlayer);
	void WritePersistentData(CClient* pPlayer);
};

extern ServerAuthenticationManager* g_pServerAuthentication;
