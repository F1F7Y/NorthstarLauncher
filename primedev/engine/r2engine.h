#pragma once

#include "tier1/cmd.h"
#include "shared/keyvalues.h"
#include "engine/edict.h"

// Cbuf
enum class ECommandTarget_t
{
	CBUF_FIRST_PLAYER = 0,
	CBUF_LAST_PLAYER = 1, // MAX_SPLITSCREEN_CLIENTS - 1, MAX_SPLITSCREEN_CLIENTS = 2
	CBUF_SERVER = CBUF_LAST_PLAYER + 1,

	CBUF_COUNT,
};

enum class cmd_source_t
{
	// Added to the console buffer by gameplay code.  Generally unrestricted.
	kCommandSrcCode,

	// Sent from code via engine->ClientCmd, which is restricted to commands visible
	// via FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS.
	kCommandSrcClientCmd,

	// Typed in at the console or via a user key-bind.  Generally unrestricted, although
	// the client will throttle commands sent to the server this way to 16 per second.
	kCommandSrcUserInput,

	// Came in over a net connection as a clc_stringcmd
	// host_client will be valid during this state.
	//
	// Restricted to FCVAR_GAMEDLL commands (but not convars) and special non-ConCommand
	// server commands hardcoded into gameplay code (e.g. "joingame")
	kCommandSrcNetClient,

	// Received from the server as the client
	//
	// Restricted to commands with FCVAR_SERVER_CAN_EXECUTE
	kCommandSrcNetServer,

	// Being played back from a demo file
	//
	// Not currently restricted by convar flag, but some commands manually ignore calls
	// from this source.  FIXME: Should be heavily restricted as demo commands can come
	// from untrusted sources.
	kCommandSrcDemoFile,

	// Invalid value used when cleared
	kCommandSrcInvalid = -1
};

typedef ECommandTarget_t (*Cbuf_GetCurrentPlayerType)();
extern Cbuf_GetCurrentPlayerType Cbuf_GetCurrentPlayer;

typedef void (*Cbuf_AddTextType)(ECommandTarget_t eTarget, const char* text, cmd_source_t source);
extern Cbuf_AddTextType Cbuf_AddText;

typedef void (*Cbuf_ExecuteType)();
extern Cbuf_ExecuteType Cbuf_Execute;

extern bool (*CCommand__Tokenize)(CCommand& self, const char* pCommandString, cmd_source_t commandSource);

// CEngine

enum EngineQuitState
{
	QUIT_NOTQUITTING = 0,
	QUIT_TODESKTOP,
	QUIT_RESTART
};

enum class EngineState_t
{
	DLL_INACTIVE = 0, // no dll
	DLL_ACTIVE, // engine is focused
	DLL_CLOSE, // closing down dll
	DLL_RESTART, // engine is shutting down but will restart right away
	DLL_PAUSED, // engine is paused, can become active from this state
};

class CEngine
{
	public:
	virtual void unknown() = 0; // unsure if this is where
	virtual bool Load(bool dedicated, const char* baseDir) = 0;
	virtual void Unload() = 0;
	virtual void SetNextState(EngineState_t iNextState) = 0;
	virtual EngineState_t GetState() = 0;
	virtual void Frame() = 0;
	virtual double GetFrameTime() = 0;
	virtual float GetCurTime() = 0;

	EngineQuitState m_nQuitting;
	EngineState_t m_nDllState;
	EngineState_t m_nNextDllState;
	double m_flCurrentTime;
	float m_flFrameTime;
	double m_flPreviousTime;
	float m_flFilteredTime;
	float m_flMinFrameTime; // Expected duration of a frame, or zero if it is unlimited.
};

extern CEngine* g_pEngine;

extern void (*CBaseClient__Disconnect)(void* self, uint32_t unknownButAlways1, const char* reason, ...);

// #56169 $DB69 PData size
// #512   $200	Trailing data
// #100	  $64	Safety buffer
const int PERSISTENCE_MAX_SIZE = 0xDDCD;

// note: NOT_READY and READY are the only entries we have here that are defined by the vanilla game
// entries after this are custom and used to determine the source of persistence, e.g. whether it is local or remote
enum class ePersistenceReady : char
{
	NOT_READY,
	READY = 3,
	READY_INSECURE = 3,
	READY_REMOTE
};

enum class eSignonState : int
{
	NONE = 0, // no state yet; about to connect
	CHALLENGE = 1, // client challenging server; all OOB packets
	CONNECTED = 2, // client is connected to server; netchans ready
	NEW = 3, // just got serverinfo and string tables
	PRESPAWN = 4, // received signon buffers
	GETTINGDATA = 5, // respawn-defined signonstate, assumedly this is for persistence
	SPAWN = 6, // ready to receive entity packets
	FIRSTSNAP = 7, // another respawn-defined one
	FULL = 8, // we are fully connected; first non-delta packet received
	CHANGELEVEL = 9, // server is changing level; please wait
};

// clang-format off
OFFSET_STRUCT(CBaseClient)
{
	STRUCT_SIZE(0x2D728)
	FIELD(0x16, char m_Name[64])
	FIELD(0x258, KeyValues* m_ConVars)
	FIELD(0x2A0, eSignonState m_Signon)
	FIELD(0x358, char m_ClanTag[16])
	FIELD(0x484, bool m_bFakePlayer)
	FIELD(0x4A0, ePersistenceReady m_iPersistenceReady)
	FIELD(0x4FA, char m_PersistenceBuffer[PERSISTENCE_MAX_SIZE])
	FIELD(0xF500, char m_UID[32])
};
// clang-format on

extern CBaseClient* g_pClientArray;

enum server_state_t
{
	ss_dead = 0, // Dead
	ss_loading, // Spawning
	ss_active, // Running
	ss_paused, // Running, but paused
};

extern server_state_t* g_pServerState;

extern char* g_pModName;


extern CGlobalVars* g_pServerGlobalVariables;
extern CGlobalVarsBase* g_pClientGlobalVariables;
