# PrimeDLL

find_package(minhook REQUIRED)
find_package(libcurl REQUIRED)
find_package(silver-bun REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(PrimeDLL SHARED
            "appframework/IAppSystem.h"
            "client/audio.cpp"
            "client/audio.h"
            "client/diskvmtfixes.cpp"
            "client/languagehooks.cpp"
            "client/latencyflex.cpp"
            "client/localchatwriter.cpp"
            "client/localchatwriter.h"
			"codecs/bink/bink.cpp"
			"codecs/bink/binkread.cpp"
			"common/callbacks.cpp"
			"common/callbacks.h"
            "common/globals_cvar.cpp"
            "common/globals_cvar.h"
			"core/init.cpp"
            "core/init.h"
            "core/hooks.cpp"
            "core/hooks.h"
            "core/macros.h"
            "core/memalloc.cpp"
            "core/memalloc.h"
            "dedicated/dedicated.cpp"
            "dedicated/dedicated.h"
            "dedicated/dedicatedlogtoclient.cpp"
            "dedicated/dedicatedlogtoclient.h"
            "dedicated/dedicatedmaterialsystem.cpp"
            "engine/client/client.cpp"
            "engine/client/client.h"
            "engine/server/server.cpp"
            "engine/server/server.h"
            "engine/baseserver.h"
            "engine/cdll_engine_int.cpp"
            "engine/cdll_engine_int.h"
            "engine/cl_splitscreen.cpp"
            "engine/cl_splitscreen.h"
            "engine/cmd.cpp"
            "engine/cmd.h"
            "engine/common.cpp"
            "engine/datamap.cpp"
            "engine/datamap.h"
            "engine/debugoverlay.cpp"
            "engine/debugoverlay.h"
            "engine/edict.cpp"
            "engine/edict.h"
            "engine/eiface.h"
            "engine/gl_matsysiface.cpp"
			"engine/globalvars_base.h"
            "engine/host.cpp"
			"engine/host.h"
            "engine/host_listmaps.cpp"
            "engine/hoststate.cpp"
            "engine/hoststate.h"
            "engine/ivdebugoverlay.h"
            "engine/sv_main.cpp"
            "engine/sys_engine.cpp"
			"engine/vengineserver_impl.cpp"
			"engine/vengineserver_impl.h"
            "engine/vphysics_interface.h"
			"filesystem/basefilesystem.cpp"
			"filesystem/basefilesystem.h"
            "game/client/cdll_client_int.h"
            "game/client/entity_client_tools.cpp"
            "game/client/hud_chat.cpp"
            "game/client/hud_chat.h"
            "game/client/vscript_client.cpp"
            "game/server/ai_helper.cpp"
            "game/server/ai_helper.h"
            "game/server/ai_navmesh.cpp"
            "game/server/ai_navmesh.h"
            "game/server/ai_networkmanager.cpp"
            "game/server/ai_networkmanager.h"
            "game/server/ai_node.h"
            "game/server/baseanimating.h"
            "game/server/baseanimatingoverlay.h"
            "game/server/basecombatcharacter.h"
            "game/server/baseentity.h"
            "game/server/enginecallback.h"
            "game/server/gameinterface.cpp"
            "game/server/gameinterface.h"
			"game/server/physics_main.cpp"
            "game/server/player.cpp"
            "game/server/player.h"
            "game/server/recipientfilter.cpp"
            "game/server/recipientfilter.h"
            "game/server/util_server.cpp"
            "game/server/util_server.h"
            "game/server/vscript_server.cpp"
            "game/shared/vscript_shared.cpp"
            "game/shared/vscript_shared.h"
            "gameui/GameConsole.cpp"
			"gameui/GameConsole.h"
            "launcher/launcher.cpp"
			"localize/localize.cpp"
            "logging/logging.h"
            "logging/loghooks.cpp"
            "logging/loghooks.h"
            "materialsystem/cshaderglue.h"
            "materialsystem/cmaterialglue.h"
			"mathlib/bitbuf.h"
            "mathlib/bits.cpp"
            "mathlib/bits.h"
            "mathlib/color.cpp"
            "mathlib/color.h"
            "mathlib/math_pfns.h"
            "mathlib/matrix.h"
            "mathlib/quaternion.h"
            "mathlib/vector.h"
            "mathlib/vplane.h"
            "mods/compiled/kb_act.cpp"
            "mods/compiled/modkeyvalues.cpp"
            "mods/compiled/modpdef.cpp"
            "mods/compiled/modscriptsrson.cpp"
            "mods/modmanager.cpp"
            "mods/modmanager.h"
            "mods/modsavefiles.cpp"
            "mods/modsavefiles.h"
            "networksystem/bansystem.cpp"
            "networksystem/bansystem.h"
            "networksystem/bcrypt.cpp"
            "networksystem/bcrypt.h"
            "networksystem/inetmsghandler.h"
            "networksystem/netchannel.h"
            "networksystem/atlas.cpp"
            "networksystem/atlas.h"
            "originsdk/origin.cpp"
            "originsdk/origin.h"
            "originsdk/overlay.cpp"
            "originsdk/overlay.h"
            "originsdk/stryder.cpp"
            "rtech/rui/rui.cpp"
            "rtech/datatable.cpp"
            "rtech/datatable.h"
			"rtech/pakapi.cpp"
			"rtech/pakapi.h"
            "shared/exploit_fixes/exploitfixes.cpp"
            "shared/exploit_fixes/exploitfixes_lzss.cpp"
            "shared/exploit_fixes/exploitfixes_utf8parser.cpp"
            "shared/exploit_fixes/ns_limits.cpp"
            "shared/exploit_fixes/ns_limits.h"
            "shared/misccommands.cpp"
            "shared/misccommands.h"
            "shared/playlist.cpp"
            "shared/playlist.h"
            "squirrel/squirrel.cpp"
            "squirrel/squirrel.h"
            "squirrel/squirrelautobind.cpp"
            "squirrel/squirrelautobind.h"
            "squirrel/squirrelclasstypes.h"
            "squirrel/squirreldatatypes.h"
            "tier0/commandline.h"
            "tier0/dbg.cpp"
            "tier0/dbg.h"
            "tier0/filestream.cpp"
            "tier0/filestream.h"
            "tier0/memstd.cpp"
            "tier0/memstd.h"
            "tier0/platform.h"
            "tier0/threadtools.h"
            "tier0/utils.cpp"
            "tier0/utils.h"
            "tier1/cmd.cpp"
            "tier1/cmd.h"
            "tier1/convar.cpp"
            "tier1/convar.h"
            "tier1/cvar.cpp"
            "tier1/cvar.h"
            "tier1/interface.cpp"
			"tier1/interface.h"
            "tier1/keyvalues.cpp"
            "tier1/keyvalues.h"
            "tier1/utlmemory.h"
            "tier2/curlutils.cpp"
            "tier2/curlutils.h"
            "toolframework/itoolentity.h"
            "vgui/vgui_baseui_interface.cpp"
			"vgui/vgui_baseui_interface.h"
            "windows/libsys.cpp"
            "windows/libsys.h"
			"windows/wconsole.cpp"
			"windows/wconsole.h"
			"windows/window.cpp"
			"Northstar.def"
)

target_link_libraries(PrimeDLL PRIVATE
                      minhook
                      libcurl
                      silver-bun
					  nlohmann_json
                      WS2_32.lib
                      Crypt32.lib
                      Cryptui.lib
                      dbghelp.lib
                      Wldap32.lib
                      Normaliz.lib
                      Bcrypt.lib
                      version.lib
)

target_precompile_headers(PrimeDLL PRIVATE core/stdafx.h)

target_compile_definitions(PrimeDLL PRIVATE
                           NORTHSTAR
                           UNICODE
                           _UNICODE
                           CURL_STATICLIB
)

set_target_properties(PrimeDLL PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${NS_BINARY_DIR}
                      OUTPUT_NAME bin/x64_retail/Prime
                      COMPILE_FLAGS "/W4"
                      LINK_FLAGS "/MANIFEST:NO /DEBUG"
)
