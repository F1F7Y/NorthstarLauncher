#pragma once
#include "shared/keyvalues.h"

// use the R2 namespace for game funcs
namespace R2
{
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

	extern bool (*CCommand__Tokenize)(CCommand& self, const char* pCommandString, R2::cmd_source_t commandSource);

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

#pragma once
	typedef enum
	{
		NA_NULL = 0,
		NA_LOOPBACK,
		NA_IP,
	} netadrtype_t;

#pragma pack(push, 1)
	typedef struct netadr_s
	{
		netadrtype_t type;
		unsigned char ip[16]; // IPv6
		// IPv4's 127.0.0.1 is [::ffff:127.0.0.1], that is:
		// 00 00 00 00 00 00 00 00    00 00 FF FF 7F 00 00 01
		unsigned short port;
	} netadr_t;
#pragma pack(pop)

#pragma pack(push, 1)
	typedef struct netpacket_s
	{
		netadr_t adr; // sender address
		// int				source;		// received source
		char unk[10];
		double received_time;
		unsigned char* data; // pointer to raw packet data
		void* message; // easy bitbuf data access // 'inpacket.message' etc etc (pointer)
		char unk2[16];
		int size;

		// bf_read			message;	// easy bitbuf data access // 'inpacket.message' etc etc (pointer)
		// int				size;		// size in bytes
		// int				wiresize;   // size in bytes before decompression
		// bool			stream;		// was send as stream
		// struct netpacket_s* pNext;	// for internal use, should be NULL in public
	} netpacket_t;
#pragma pack(pop)

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
	class CClient
	{
		void* vftable;
		void* vftable2;

	public:
		uint32_t m_nUserID;
		uint16_t m_nHandle;
		char m_szServerName[64];
		int64_t m_nReputation;
		char pad_0014[182];
		char m_szClientName[64];
		char pad_0015[252];
		KeyValues* m_ConVars;
		char pad_0368[8];
		void* m_pServer;
		char pad_0378[32];
		void* m_NetChannel;
		char pad_03A8[8];
		eSignonState m_nSignonState;
		int32_t m_nDeltaTick;
		uint64_t m_nOriginID;
		int32_t m_nStringTableAckTick;
		int32_t m_nSignonTick;
		char pad_03C0[160];
		char m_szClanTag[16];
		char pad2[284];
		bool m_bFakePlayer;
		bool m_bReceivedPacket;
		bool m_bLowViolence;
		bool m_bFullyAuthenticated;
		char pad_05A4[24];
		ePersistenceReady m_iPersistenceReady;
		char pad_05C0[89];
		char m_PersistenceBuffer[PERSISTENCE_MAX_SIZE];
		char pad[4665];
		char m_UID[32];
		char pad0[0x1E208];
	};
	static_assert(sizeof(CClient) == 0x2D728);
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

	// clang-format off
	class CGlobalVarsBase
	{
	public:
		double m_flRealTime;
		int m_nFrameCount;
		float m_flAbsoluteFrameTime;
		float m_flCurTime;
		float m_fUnk0;
		float m_fUnk1;
		float m_fUnk2;
		float m_fUnk3;
		float m_fUnk4;
		float m_fUnk5;
		float m_fUnk6;
		float m_flFrameTime;
		int m_nMaxClients;
		GameMode_t m_nGameMode;
		float m_Unk7;
		float m_flTickInterval;
		float m_Unk8;
		float m_Unk9;
		float m_Unk10;
		float m_Unk11;
		float m_Unk12;
		float m_Unk13;
	};
	static_assert(sizeof(CGlobalVarsBase) == 0x60);

	class CGlobalVars : public CGlobalVarsBase
	{
	public:
		const char* m_pMapName;
		int m_nMapVersion;
		const char* m_pTest;
		MapLoadType_t m_MapLoadType;
		__int64 gap0;
		void* m_pUnk0;
		__int64 gap;
		void* m_pUnk1;
		__int64 m_Unk2;
	};
	static_assert(sizeof(CGlobalVars) == 0xA8);

	extern CGlobalVars* g_pServerGlobalVariables;
} // namespace R2
