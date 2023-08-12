#include "tier0.h"
#include "tier0/commandline.h"

IMemAlloc* g_pMemAllocSingleton;

Plat_FloatTimeType Plat_FloatTime;
ThreadInServerFrameThreadType ThreadInServerFrameThread;

typedef IMemAlloc* (*CreateGlobalMemAllocType)();
CreateGlobalMemAllocType CreateGlobalMemAlloc;

// needs to be a seperate function, since memalloc.cpp calls it
void TryCreateGlobalMemAlloc()
{
	// init memalloc stuff
	CreateGlobalMemAlloc =
		reinterpret_cast<CreateGlobalMemAllocType>(GetProcAddress(GetModuleHandleA("tier0.dll"), "CreateGlobalMemAlloc"));
	g_pMemAllocSingleton = CreateGlobalMemAlloc(); // if it already exists, this returns the preexisting IMemAlloc instance
}

ON_DLL_LOAD("tier0.dll", Tier0GameFuncs, (CModule module))
{
	// shouldn't be necessary, but do this just in case
	TryCreateGlobalMemAlloc();

	// setup tier0 funcs
	CommandLine = module.GetExport("CommandLine").RCast<CCommandLine* (*)()>();
	Plat_FloatTime = module.GetExport("Plat_FloatTime").RCast<Plat_FloatTimeType>();
	ThreadInServerFrameThread = module.GetExport("ThreadInServerFrameThread").RCast<ThreadInServerFrameThreadType>();
}