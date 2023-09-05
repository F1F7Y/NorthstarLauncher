#include "modmanager.h"
#include "tier1/convar.h"
#include "tier1/cmd.h"
#include "client/audio.h"
#include "filesystem/basefilesystem.h"
#include "rtech/pakapi.h"
#include "squirrel/squirrel.h"

#include "rapidjson/error/en.h"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

ModManager* g_pModManager;

Mod::Mod(fs::path modDir, char* jsonBuf)
{
	m_bWasReadSuccessfully = false;

	m_ModDirectory = modDir;

	rapidjson_document modJson;
	modJson.Parse<rapidjson::ParseFlag::kParseCommentsFlag | rapidjson::ParseFlag::kParseTrailingCommasFlag>(jsonBuf);

	DevMsg(eLog::MODSYS, "Loading mod file at path '%s'\n", modDir.string().c_str());

	// fail if parse error
	if (modJson.HasParseError())
	{
		Error(eLog::MODSYS, NO_ERROR, "Failed reading mod file %s: encountered parse error \"%s\" at offset %li\n", (modDir / "mod.json").string().c_str(), GetParseError_En(modJson.GetParseError()), modJson.GetErrorOffset());
		return;
	}

	// fail if it's not a json obj (could be an array, string, etc)
	if (!modJson.IsObject())
	{
		Error(eLog::MODSYS, NO_ERROR, "Failed reading mod file %s: file is not a JSON object\n", (modDir / "mod.json").string().c_str());
		return;
	}

	// basic mod info
	// name is required
	if (!modJson.HasMember("Name"))
	{
		Error(eLog::MODSYS, NO_ERROR, "Failed reading mod file %s: missing required member \"Name\"\n", (modDir / "mod.json").string().c_str());
		return;
	}

	Name = modJson["Name"].GetString();
	Warning(eLog::MODSYS, "Loading mod '%s'\n", Name.c_str());

	// Don't load blacklisted mods
	if (!strstr(GetCommandLineA(), "-nomodblacklist") && MODS_BLACKLIST.find(Name) != std::end(MODS_BLACKLIST))
	{
		Warning(eLog::MODSYS, "Skipping blacklisted mod \"%s\"!\n", Name.c_str());
		return;
	}

	if (modJson.HasMember("Description"))
		Description = modJson["Description"].GetString();
	else
		Description = "";

	if (modJson.HasMember("Version"))
		Version = modJson["Version"].GetString();
	else
	{
		Version = "0.0.0";
		Warning(eLog::MS, "Mod file %s is missing a version, consider adding a version\n", (modDir / "mod.json").string().c_str());
	}

	if (modJson.HasMember("DownloadLink"))
		DownloadLink = modJson["DownloadLink"].GetString();
	else
		DownloadLink = "";

	if (modJson.HasMember("RequiredOnClient"))
		RequiredOnClient = modJson["RequiredOnClient"].GetBool();
	else
		RequiredOnClient = false;

	if (modJson.HasMember("LoadPriority"))
		LoadPriority = modJson["LoadPriority"].GetInt();
	else
	{
		Warning(eLog::MS, "Mod file %s is missing a LoadPriority, consider adding one\n", (modDir / "mod.json").string().c_str());
		LoadPriority = 0;
	}

	// Parse all array fields
	ParseConVars(modJson);
	ParseConCommands(modJson);
	ParseScripts(modJson);
	ParseLocalization(modJson);
	ParseDependencies(modJson);
	ParseInitScript(modJson);

	m_bWasReadSuccessfully = true;
}

void Mod::ParseConVars(rapidjson_document& json)
{
	if (!json.HasMember("ConVars"))
		return;

	if (!json["ConVars"].IsArray())
	{
		Warning(eLog::MODSYS, "'ConVars' field is not an array, skipping...\n");
		return;
	}

	for (auto& convarObj : json["ConVars"].GetArray())
	{
		if (!convarObj.IsObject())
		{
			Warning(eLog::MODSYS, "ConVar is not an object, skipping...\n");
			continue;
		}
		if (!convarObj.HasMember("Name"))
		{
			Warning(eLog::MODSYS, "ConVar does not have a Name, skipping...\n");
			continue;
		}
		// from here on, the ConVar can be referenced by name in logs
		if (!convarObj.HasMember("DefaultValue"))
		{
			Warning(eLog::MODSYS, "ConVar '%s' does not have a DefaultValue, skipping...\n", convarObj["Name"].GetString());
			continue;
		}

		// have to allocate this manually, otherwise convar registration will break
		// unfortunately this causes us to leak memory on reload, unsure of a way around this rn
		ModConVar* convar = new ModConVar;
		convar->Name = convarObj["Name"].GetString();
		convar->DefaultValue = convarObj["DefaultValue"].GetString();

		if (convarObj.HasMember("HelpString"))
			convar->HelpString = convarObj["HelpString"].GetString();
		else
			convar->HelpString = "";

		convar->Flags = FCVAR_NONE;

		if (convarObj.HasMember("Flags"))
		{
			// read raw integer flags
			if (convarObj["Flags"].IsInt())
				convar->Flags = convarObj["Flags"].GetInt();
			else if (convarObj["Flags"].IsString())
			{
				// parse cvar flags from string
				// example string: ARCHIVE_PLAYERPROFILE | GAMEDLL
				convar->Flags |= ParseConVarFlagsString(convar->Name, convarObj["Flags"].GetString());
			}
		}

		ConVars.push_back(convar);

		DevMsg(eLog::MODSYS, "'%s' contains ConVar '%s'\n", Name.c_str(), convar->Name.c_str());
	}
}

void Mod::ParseConCommands(rapidjson_document& json)
{
	if (!json.HasMember("ConCommands"))
		return;

	if (!json["ConCommands"].IsArray())
	{
		Warning(eLog::MODSYS, "'ConCommands' field is not an array, skipping...\n");
		return;
	}

	for (auto& concommandObj : json["ConCommands"].GetArray())
	{
		if (!concommandObj.IsObject())
		{
			Warning(eLog::MODSYS, "ConCommand is not an object, skipping...\n");
			continue;
		}
		if (!concommandObj.HasMember("Name"))
		{
			Warning(eLog::MODSYS, "ConCommand does not have a Name, skipping...\n");
			continue;
		}
		// from here on, the ConCommand can be referenced by name in logs
		if (!concommandObj.HasMember("Function"))
		{
			Warning(eLog::MODSYS, "ConCommand '{}' does not have a Function, skipping...\n", concommandObj["Name"].GetString());
			continue;
		}
		if (!concommandObj.HasMember("Context"))
		{
			Warning(eLog::MODSYS, "ConCommand '{}' does not have a Context, skipping...\n", concommandObj["Name"].GetString());
			continue;
		}

		// have to allocate this manually, otherwise concommand registration will break
		// unfortunately this causes us to leak memory on reload, unsure of a way around this rn
		ModConCommand* concommand = new ModConCommand;
		concommand->Name = concommandObj["Name"].GetString();
		concommand->Function = concommandObj["Function"].GetString();
		concommand->Context = ScriptContextFromString(concommandObj["Context"].GetString());
		if (concommand->Context == ScriptContext::INVALID)
		{
			Warning(eLog::MODSYS, "Mod ConCommand %s has invalid context %s\n", concommand->Name.c_str(), concommandObj["Context"].GetString());
			continue;
		}

		if (concommandObj.HasMember("HelpString"))
			concommand->HelpString = concommandObj["HelpString"].GetString();
		else
			concommand->HelpString = "";

		concommand->Flags = FCVAR_NONE;

		if (concommandObj.HasMember("Flags"))
		{
			// read raw integer flags
			if (concommandObj["Flags"].IsInt())
			{
				concommand->Flags = concommandObj["Flags"].GetInt();
			}
			else if (concommandObj["Flags"].IsString())
			{
				// parse cvar flags from string
				// example string: ARCHIVE_PLAYERPROFILE | GAMEDLL
				concommand->Flags |= ParseConVarFlagsString(concommand->Name, concommandObj["Flags"].GetString());
			}
		}

		ConCommands.push_back(concommand);

		DevMsg(eLog::MODSYS, "'%s' contains ConCommand '%s'\n", Name.c_str(), concommand->Name.c_str());
	}
}

void Mod::ParseScripts(rapidjson_document& json)
{
	if (!json.HasMember("Scripts"))
		return;

	if (!json["Scripts"].IsArray())
	{
		Warning(eLog::MODSYS, "'Scripts' field is not an array, skipping...\n");
		return;
	}

	for (auto& scriptObj : json["Scripts"].GetArray())
	{
		if (!scriptObj.IsObject())
		{
			Warning(eLog::MODSYS, "Script is not an object, skipping...\n");
			continue;
		}
		if (!scriptObj.HasMember("Path"))
		{
			Warning(eLog::MODSYS, "Script does not have a Path, skipping...\n");
			continue;
		}
		// from here on, the Path for a script is used as it's name in logs
		if (!scriptObj.HasMember("RunOn"))
		{
			// "a RunOn" sounds dumb but anything else doesn't match the format of the warnings...
			// this is the best i could think of within 20 seconds
			Warning(eLog::MODSYS, "Script '%s' does not have a RunOn field, skipping...\n", scriptObj["Path"].GetString());
			continue;
		}

		ModScript script;

		script.Path = scriptObj["Path"].GetString();
		script.RunOn = scriptObj["RunOn"].GetString();

		if (scriptObj.HasMember("ServerCallback"))
		{
			if (scriptObj["ServerCallback"].IsObject())
			{
				ModScriptCallback callback;
				callback.Context = ScriptContext::SERVER;

				if (scriptObj["ServerCallback"].HasMember("Before"))
				{
					if (scriptObj["ServerCallback"]["Before"].IsString())
						callback.BeforeCallback = scriptObj["ServerCallback"]["Before"].GetString();
					else
						Warning(eLog::MODSYS, "'Before' ServerCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["ServerCallback"].HasMember("After"))
				{
					if (scriptObj["ServerCallback"]["After"].IsString())
						callback.AfterCallback = scriptObj["ServerCallback"]["After"].GetString();
					else
						Warning(eLog::MODSYS, "'After' ServerCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["ServerCallback"].HasMember("Destroy"))
				{
					if (scriptObj["ServerCallback"]["Destroy"].IsString())
						callback.DestroyCallback = scriptObj["ServerCallback"]["Destroy"].GetString();
					else
						Warning(eLog::MODSYS, "'Destroy' ServerCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				script.Callbacks.push_back(callback);
			}
			else
			{
				Warning(eLog::MODSYS, "ServerCallback for script '%s' is not an object, skipping...\n", scriptObj["Path"].GetString());
			}
		}

		if (scriptObj.HasMember("ClientCallback"))
		{
			if (scriptObj["ClientCallback"].IsObject())
			{
				ModScriptCallback callback;
				callback.Context = ScriptContext::CLIENT;

				if (scriptObj["ClientCallback"].HasMember("Before"))
				{
					if (scriptObj["ClientCallback"]["Before"].IsString())
						callback.BeforeCallback = scriptObj["ClientCallback"]["Before"].GetString();
					else
						Warning(eLog::MODSYS, "'Before' ClientCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["ClientCallback"].HasMember("After"))
				{
					if (scriptObj["ClientCallback"]["After"].IsString())
						callback.AfterCallback = scriptObj["ClientCallback"]["After"].GetString();
					else
						Warning(eLog::MODSYS, "'After' ClientCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["ClientCallback"].HasMember("Destroy"))
				{
					if (scriptObj["ClientCallback"]["Destroy"].IsString())
						callback.DestroyCallback = scriptObj["ClientCallback"]["Destroy"].GetString();
					else
						Warning(eLog::MODSYS, "'Destroy' ClientCallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				script.Callbacks.push_back(callback);
			}
			else
			{
				Warning(eLog::MODSYS, "ClientCallback for script '%s' is not an object, skipping...\n", scriptObj["Path"].GetString());
			}
		}

		if (scriptObj.HasMember("UICallback"))
		{
			if (scriptObj["UICallback"].IsObject())
			{
				ModScriptCallback callback;
				callback.Context = ScriptContext::UI;

				if (scriptObj["UICallback"].HasMember("Before"))
				{
					if (scriptObj["UICallback"]["Before"].IsString())
						callback.BeforeCallback = scriptObj["UICallback"]["Before"].GetString();
					else
						Warning(eLog::MODSYS, "'Before' UICallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["UICallback"].HasMember("After"))
				{
					if (scriptObj["UICallback"]["After"].IsString())
						callback.AfterCallback = scriptObj["UICallback"]["After"].GetString();
					else
						Warning(eLog::MODSYS, "'After' UICallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				if (scriptObj["UICallback"].HasMember("Destroy"))
				{
					if (scriptObj["UICallback"]["Destroy"].IsString())
						callback.DestroyCallback = scriptObj["UICallback"]["Destroy"].GetString();
					else
						Warning(eLog::MODSYS, "'Destroy' UICallback for script '%s' is not a string, skipping...\n", scriptObj["Path"].GetString());
				}

				script.Callbacks.push_back(callback);
			}
			else
			{
				Warning(eLog::MODSYS, "UICallback for script '%s' is not an object, skipping...\n", scriptObj["Path"].GetString());
			}
		}

		Scripts.push_back(script);

		DevMsg(eLog::MODSYS, "'%s' contains Script '%s'\n", Name.c_str(), script.Path.c_str());
	}
}

void Mod::ParseLocalization(rapidjson_document& json)
{
	if (!json.HasMember("Localisation"))
		return;

	if (!json["Localisation"].IsArray())
	{
		Warning(eLog::MODSYS, "'Localisation' field is not an array, skipping...\n");
		return;
	}

	for (auto& localisationStr : json["Localisation"].GetArray())
	{
		if (!localisationStr.IsString())
		{
			// not a string but we still GetString() to log it :trol:
			Warning(eLog::MODSYS, "Localisation '%s' is not a string, skipping...\n", localisationStr.GetString());
			continue;
		}

		LocalisationFiles.push_back(localisationStr.GetString());

		DevMsg(eLog::MODSYS, "'%s' registered Localisation '%s'\n", Name.c_str(), localisationStr.GetString());
	}
}

void Mod::ParseDependencies(rapidjson_document& json)
{
	if (!json.HasMember("Dependencies"))
		return;

	if (!json["Dependencies"].IsObject())
	{
		Warning(eLog::MODSYS, "'Dependencies' field is not an object, skipping...\n");
		return;
	}

	for (auto v = json["Dependencies"].MemberBegin(); v != json["Dependencies"].MemberEnd(); v++)
	{
		if (!v->name.IsString())
		{
			Warning(eLog::MODSYS, "Dependency constant '%s' is not a string, skipping...\n", v->name.GetString());
			continue;
		}
		if (!v->value.IsString())
		{
			Warning(eLog::MODSYS, "Dependency constant '%s' is not a string, skipping...\n", v->value.GetString());
			continue;
		}

		if (DependencyConstants.find(v->name.GetString()) != DependencyConstants.end() && v->value.GetString() != DependencyConstants[v->name.GetString()])
		{
			// this is fatal because otherwise the mod will probably try to use functions that dont exist,
			// which will cause errors further down the line that are harder to debug
			Error(eLog::MODSYS, NO_ERROR,
				  "'%s' attempted to register a dependency constant '%s' for '%s' that already exists for '%s'. "
				  "Change the constant name.\n",
				  Name.c_str(), v->name.GetString(), v->value.GetString(), DependencyConstants[v->name.GetString()].c_str());
			return;
		}

		if (DependencyConstants.find(v->name.GetString()) == DependencyConstants.end())
			DependencyConstants.emplace(v->name.GetString(), v->value.GetString());

		DevMsg(eLog::MODSYS, "'%s' registered dependency constant '%s' for mod '%s'\n", Name.c_str(), v->name.GetString(), v->value.GetString());
	}
}

void Mod::ParseInitScript(rapidjson_document& json)
{
	if (!json.HasMember("InitScript"))
		return;

	if (!json["InitScript"].IsString())
	{
		Warning(eLog::MODSYS, "'InitScript' field is not a string, skipping...\n");
		return;
	}

	initScript = json["InitScript"].GetString();
}

ModManager::ModManager()
{
	// precaculated string hashes
	// note: use backslashes for these, since we use lexically_normal for file paths which uses them
	m_hScriptsRsonHash = STR_HASH("scripts\\vscripts\\scripts.rson");
	m_hPdefHash = STR_HASH("cfg\\server\\persistent_player_data_version_231.pdef" // this can have multiple versions, but we use 231 so that's what we hash
	);
	m_hKBActHash = STR_HASH("scripts\\kb_act.lst");

	LoadMods();
}

struct Test
{
	std::string funcName;
	ScriptContext context;
};

template <ScriptContext context>
auto ModConCommandCallback_Internal(std::string name, const CCommand& command)
{
	if (g_pSquirrel<context>->m_pSQVM && g_pSquirrel<context>->m_pSQVM)
	{
		if (command.ArgC() == 1)
		{
			g_pSquirrel<context>->AsyncCall(name);
		}
		else
		{
			std::vector<std::string> args;
			args.reserve(command.ArgC());
			for (int i = 1; i < command.ArgC(); i++)
				args.push_back(command.Arg(i));
			g_pSquirrel<context>->AsyncCall(name, args);
		}
	}
	else
	{
		Warning(eLog::MODSYS, "ConCommand `%s` was called while the associated Squirrel VM `%s` was unloaded\n", name.c_str(), GetContextName(context));
	}
}

auto ModConCommandCallback(const CCommand& command)
{
	ModConCommand* found = nullptr;
	auto commandString = std::string(command.GetCommandString());

	// Finding the first space to remove the command's name
	auto firstSpace = commandString.find(' ');
	if (firstSpace)
	{
		commandString = commandString.substr(0, firstSpace);
	}

	// Find the mod this command belongs to
	for (auto& mod : g_pModManager->m_LoadedMods)
	{
		auto res = std::find_if(mod.ConCommands.begin(), mod.ConCommands.end(), [&commandString](const ModConCommand* other) { return other->Name == commandString; });
		if (res != mod.ConCommands.end())
		{
			found = *res;
			break;
		}
	}
	if (!found)
		return;

	switch (found->Context)
	{
	case ScriptContext::CLIENT:
		ModConCommandCallback_Internal<ScriptContext::CLIENT>(found->Function, command);
		break;
	case ScriptContext::SERVER:
		ModConCommandCallback_Internal<ScriptContext::SERVER>(found->Function, command);
		break;
	case ScriptContext::UI:
		ModConCommandCallback_Internal<ScriptContext::UI>(found->Function, command);
		break;
	};
}

void ModManager::LoadMods()
{
	if (m_bHasLoadedMods)
		UnloadMods();

	std::vector<fs::path> modDirs;

	// ensure dirs exist
	fs::remove_all(GetCompiledAssetsPath());
	fs::create_directories(GetModFolderPath());
	fs::create_directories(GetThunderstoreModFolderPath());
	fs::create_directories(GetRemoteModFolderPath());

	m_DependencyConstants.clear();

	// read enabled mods cfg
	std::ifstream enabledModsStream(g_svProfileDir + "/enabledmods.json");
	std::stringstream enabledModsStringStream;

	if (!enabledModsStream.fail())
	{
		while (enabledModsStream.peek() != EOF)
			enabledModsStringStream << (char)enabledModsStream.get();

		enabledModsStream.close();
		m_EnabledModsCfg.Parse<rapidjson::ParseFlag::kParseCommentsFlag | rapidjson::ParseFlag::kParseTrailingCommasFlag>(enabledModsStringStream.str().c_str());

		m_bHasEnabledModsCfg = m_EnabledModsCfg.IsObject();
	}

	// get mod directories
	fs::directory_iterator classicModsDir = fs::directory_iterator(GetModFolderPath());
	fs::directory_iterator remoteModsDir = fs::directory_iterator(GetRemoteModFolderPath());

	for (fs::directory_iterator modIterator : {classicModsDir, remoteModsDir})
		for (fs::directory_entry dir : modIterator)
			if (FileExists(dir.path() / "mod.json"))
				modDirs.push_back(dir.path());

	// Special case for Thunderstore mods dir
	fs::directory_iterator thunderstoreModsDir = fs::directory_iterator(GetThunderstoreModFolderPath());
	// Set up regex for `AUTHOR-MOD-VERSION` pattern
	std::regex pattern(R"(.*\\([a-zA-Z0-9_]+)-([a-zA-Z0-9_]+)-(\d+\.\d+\.\d+))");
	for (fs::directory_entry dir : thunderstoreModsDir)
	{
		fs::path modsDir = dir.path() / "mods"; // Check for mods folder in the Thunderstore mod
		// Use regex to match `AUTHOR-MOD-VERSION` pattern
		if (!std::regex_match(dir.path().string(), pattern))
		{
			Warning(eLog::MODSYS, "The following directory did not match 'AUTHOR-MOD-VERSION': %s\n", dir.path().string().c_str());
			continue; // skip loading mod that doesn't match
		}
		if (FileExists(modsDir) && fs::is_directory(modsDir))
		{
			for (fs::directory_entry subDir : fs::directory_iterator(modsDir))
			{
				if (FileExists(subDir.path() / "mod.json"))
				{
					modDirs.push_back(subDir.path());
				}
			}
		}
	}

	for (fs::path modDir : modDirs)
	{
		// read mod json file
		std::ifstream jsonStream(modDir / "mod.json");
		std::stringstream jsonStringStream;

		// fail if no mod json
		if (jsonStream.fail())
		{
			Warning(eLog::MODSYS, "Mod file at '%s' does not exist or could not be read, is it installed correctly?\n", (modDir / "mod.json").string().c_str());
			continue;
		}

		while (jsonStream.peek() != EOF)
			jsonStringStream << (char)jsonStream.get();

		jsonStream.close();

		Mod mod(modDir, (char*)jsonStringStream.str().c_str());

		for (auto& pair : mod.DependencyConstants)
		{
			if (m_DependencyConstants.find(pair.first) != m_DependencyConstants.end() && m_DependencyConstants[pair.first] != pair.second)
			{
				Error(eLog::MODSYS, NO_ERROR,
					  "'%s' attempted to register a dependency constant '%s' for '%s' that already exists for '%s'. "
					  "Change the constant name.\n",
					  mod.Name.c_str(), pair.first.c_str(), pair.second.c_str(), m_DependencyConstants[pair.first].c_str());
				mod.m_bWasReadSuccessfully = false;
				break;
			}
			if (m_DependencyConstants.find(pair.first) == m_DependencyConstants.end())
				m_DependencyConstants.emplace(pair);
		}

		if (m_bHasEnabledModsCfg && m_EnabledModsCfg.HasMember(mod.Name.c_str()))
			mod.m_bEnabled = m_EnabledModsCfg[mod.Name.c_str()].IsTrue();
		else
			mod.m_bEnabled = true;

		if (mod.m_bWasReadSuccessfully)
		{
			if (mod.m_bEnabled)
				DevMsg(eLog::MODSYS, "'%s' loaded successfully\n", mod.Name.c_str());
			else
				DevMsg(eLog::MODSYS, "'%s' loaded successfully (DISABLED)\n", mod.Name.c_str());

			m_LoadedMods.push_back(mod);
		}
		else
			Warning(eLog::MODSYS, "Mod file at '%s' failed to load\n", (modDir / "mod.json").string().c_str());
	}

	// sort by load prio, lowest-highest
	std::sort(m_LoadedMods.begin(), m_LoadedMods.end(), [](Mod& a, Mod& b) { return a.LoadPriority < b.LoadPriority; });

	for (Mod& mod : m_LoadedMods)
	{
		if (!mod.m_bEnabled)
			continue;

		// register convars
		// for reloads, this is sorta barebones, when we have a good findconvar method, we could probably reset flags and stuff on
		// preexisting convars note: we don't delete convars if they already exist because they're used for script stuff, unfortunately this
		// causes us to leak memory on reload, but not much, potentially find a way to not do this at some point
		for (ModConVar* convar : mod.ConVars)
		{
			// make sure convar isn't registered yet, unsure if necessary but idk what
			// behaviour is for defining same convar multiple times
			if (!g_pCVar->FindVar(convar->Name.c_str()))
			{
				ConVar::StaticCreate(convar->Name.c_str(), convar->DefaultValue.c_str(), convar->Flags, convar->HelpString.c_str());
			}
		}

		for (ModConCommand* command : mod.ConCommands)
		{
			// make sure command isnt't registered multiple times.
			if (!g_pCVar->FindCommand(command->Name.c_str()))
			{
				ConCommand::StaticCreate(command->Name.c_str(), command->HelpString.c_str(), command->Flags, ModConCommandCallback, nullptr);
			}
		}

		// read vpk paths
		if (FileExists(mod.m_ModDirectory / "vpk"))
		{
			// read vpk cfg
			std::ifstream vpkJsonStream(mod.m_ModDirectory / "vpk/vpk.json");
			std::stringstream vpkJsonStringStream;

			bool bUseVPKJson = false;
			rapidjson::Document dVpkJson;

			if (!vpkJsonStream.fail())
			{
				while (vpkJsonStream.peek() != EOF)
					vpkJsonStringStream << (char)vpkJsonStream.get();

				vpkJsonStream.close();
				dVpkJson.Parse<rapidjson::ParseFlag::kParseCommentsFlag | rapidjson::ParseFlag::kParseTrailingCommasFlag>(vpkJsonStringStream.str().c_str());

				bUseVPKJson = !dVpkJson.HasParseError() && dVpkJson.IsObject();
			}

			for (fs::directory_entry file : fs::directory_iterator(mod.m_ModDirectory / "vpk"))
			{
				// a bunch of checks to make sure we're only adding dir vpks and their paths are good
				// note: the game will literally only load vpks with the english prefix
				if (fs::is_regular_file(file) && file.path().extension() == ".vpk" && file.path().string().find("english") != std::string::npos && file.path().string().find(".bsp.pak000_dir") != std::string::npos)
				{
					std::string formattedPath = file.path().filename().string();

					// this really fucking sucks but it'll work
					std::string vpkName = formattedPath.substr(strlen("english"), formattedPath.find(".bsp") - 3);

					ModVPKEntry& modVpk = mod.Vpks.emplace_back();
					modVpk.m_bAutoLoad = !bUseVPKJson || (dVpkJson.HasMember("Preload") && dVpkJson["Preload"].IsObject() && dVpkJson["Preload"].HasMember(vpkName) && dVpkJson["Preload"][vpkName].IsTrue());
					modVpk.m_sVpkPath = (file.path().parent_path() / vpkName).string();

					if (m_bHasLoadedMods && modVpk.m_bAutoLoad)
						g_pFilesystem->m_vtable->MountVPK(g_pFilesystem, vpkName.c_str());
				}
			}
		}

		// read rpak paths
		if (FileExists(mod.m_ModDirectory / "paks"))
		{
			// read rpak cfg
			std::ifstream rpakJsonStream(mod.m_ModDirectory / "paks/rpak.json");
			std::stringstream rpakJsonStringStream;

			bool bUseRpakJson = false;
			rapidjson::Document dRpakJson;

			if (!rpakJsonStream.fail())
			{
				while (rpakJsonStream.peek() != EOF)
					rpakJsonStringStream << (char)rpakJsonStream.get();

				rpakJsonStream.close();
				dRpakJson.Parse<rapidjson::ParseFlag::kParseCommentsFlag | rapidjson::ParseFlag::kParseTrailingCommasFlag>(rpakJsonStringStream.str().c_str());

				bUseRpakJson = !dRpakJson.HasParseError() && dRpakJson.IsObject();
			}

			// read pak aliases
			if (bUseRpakJson && dRpakJson.HasMember("Aliases") && dRpakJson["Aliases"].IsObject())
			{
				for (rapidjson::Value::ConstMemberIterator iterator = dRpakJson["Aliases"].MemberBegin(); iterator != dRpakJson["Aliases"].MemberEnd(); iterator++)
				{
					if (!iterator->name.IsString() || !iterator->value.IsString())
						continue;

					mod.RpakAliases.insert(std::make_pair(iterator->name.GetString(), iterator->value.GetString()));
				}
			}

			for (fs::directory_entry file : fs::directory_iterator(mod.m_ModDirectory / "paks"))
			{
				// ensure we're only loading rpaks
				if (fs::is_regular_file(file) && file.path().extension() == ".rpak")
				{
					std::string pakName(file.path().filename().string());

					ModRpakEntry& modPak = mod.Rpaks.emplace_back();
					modPak.m_bAutoLoad = !bUseRpakJson || (dRpakJson.HasMember("Preload") && dRpakJson["Preload"].IsObject() && dRpakJson["Preload"].HasMember(pakName) && dRpakJson["Preload"][pakName].IsTrue());

					// postload things
					if (!bUseRpakJson || (dRpakJson.HasMember("Postload") && dRpakJson["Postload"].IsObject() && dRpakJson["Postload"].HasMember(pakName)))
						modPak.m_sLoadAfterPak = dRpakJson["Postload"][pakName].GetString();

					modPak.m_sPakName = pakName;

					// read header of file and get the starpak paths
					// this is done here as opposed to on starpak load because multiple rpaks can load a starpak
					// and there is seemingly no good way to tell which rpak is causing the load of a starpak :/

					std::ifstream rpakStream(file.path(), std::ios::binary);

					// seek to the point in the header where the starpak reference size is
					rpakStream.seekg(0x38, std::ios::beg);
					int starpaksSize = 0;
					rpakStream.read((char*)&starpaksSize, 2);

					// seek to just after the header
					rpakStream.seekg(0x58, std::ios::beg);
					// read the starpak reference(s)
					std::vector<char> buf(starpaksSize);
					rpakStream.read(buf.data(), starpaksSize);

					rpakStream.close();

					// split the starpak reference(s) into strings to hash
					std::string str = "";
					for (int i = 0; i < starpaksSize; i++)
					{
						// if the current char is null, that signals the end of the current starpak path
						if (buf[i] != 0x00)
						{
							str += buf[i];
						}
						else
						{
							// only add the string we are making if it isnt empty
							if (!str.empty())
							{
								mod.StarpakPaths.push_back(STR_HASH(str));
								DevMsg(eLog::MODSYS, "Mod %s registered starpak '%s'\n", mod.Name.c_str(), str.c_str());
								str = "";
							}
						}
					}

					// not using atm because we need to resolve path to rpak
					// if (m_hasLoadedMods && modPak.m_bAutoLoad)
					//	g_pPakLoadManager->LoadPakAsync(pakName.c_str());
				}
			}
		}

		// read keyvalues paths
		if (FileExists(mod.m_ModDirectory / "keyvalues"))
		{
			for (fs::directory_entry file : fs::recursive_directory_iterator(mod.m_ModDirectory / "keyvalues"))
			{
				if (fs::is_regular_file(file))
				{
					std::string kvStr = g_pModManager->NormaliseModFilePath(file.path().lexically_relative(mod.m_ModDirectory / "keyvalues"));
					mod.KeyValues.emplace(STR_HASH(kvStr), kvStr);
				}
			}
		}

		// read pdiff
		if (FileExists(mod.m_ModDirectory / "mod.pdiff"))
		{
			std::ifstream pdiffStream(mod.m_ModDirectory / "mod.pdiff");

			if (!pdiffStream.fail())
			{
				std::stringstream pdiffStringStream;
				while (pdiffStream.peek() != EOF)
					pdiffStringStream << (char)pdiffStream.get();

				pdiffStream.close();

				mod.Pdiff = pdiffStringStream.str();
			}
		}

		// read bink video paths
		if (FileExists(mod.m_ModDirectory / "media"))
		{
			for (fs::directory_entry file : fs::recursive_directory_iterator(mod.m_ModDirectory / "media"))
				if (fs::is_regular_file(file) && file.path().extension() == ".bik")
					mod.BinkVideos.push_back(file.path().filename().string());
		}

		// try to load audio
		if (FileExists(mod.m_ModDirectory / "audio"))
		{
			for (fs::directory_entry file : fs::directory_iterator(mod.m_ModDirectory / "audio"))
			{
				if (fs::is_regular_file(file) && file.path().extension().string() == ".json")
				{
					if (!g_CustomAudioManager.TryLoadAudioOverride(file.path()))
					{
						Warning(eLog::MODSYS, "Mod %s has an invalid audio def %s\n", mod.Name.c_str(), file.path().filename().string().c_str());
						continue;
					}
				}
			}
		}
	}

	// in a seperate loop because we register mod files in reverse order, since mods loaded later should have their files prioritised
	for (int64_t i = m_LoadedMods.size() - 1; i > -1; i--)
	{
		if (!m_LoadedMods[i].m_bEnabled)
			continue;

		if (FileExists(m_LoadedMods[i].m_ModDirectory / MOD_OVERRIDE_DIR))
		{
			for (fs::directory_entry file : fs::recursive_directory_iterator(m_LoadedMods[i].m_ModDirectory / MOD_OVERRIDE_DIR))
			{
				std::string path = g_pModManager->NormaliseModFilePath(file.path().lexically_relative(m_LoadedMods[i].m_ModDirectory / MOD_OVERRIDE_DIR));
				if (file.is_regular_file() && m_ModFiles.find(path) == m_ModFiles.end())
				{
					ModOverrideFile modFile;
					modFile.m_pOwningMod = &m_LoadedMods[i];
					modFile.m_Path = path;
					m_ModFiles.insert(std::make_pair(path, modFile));
				}
			}
		}
	}

	// build modinfo obj for masterserver
	rapidjson_document modinfoDoc;
	auto& alloc = modinfoDoc.GetAllocator();
	modinfoDoc.SetObject();
	modinfoDoc.AddMember("Mods", rapidjson::kArrayType, alloc);

	int currentModIndex = 0;
	for (Mod& mod : m_LoadedMods)
	{
		if (!mod.m_bEnabled || (!mod.RequiredOnClient && !mod.Pdiff.size()))
			continue;

		modinfoDoc["Mods"].PushBack(rapidjson::kObjectType, modinfoDoc.GetAllocator());
		modinfoDoc["Mods"][currentModIndex].AddMember("Name", rapidjson::StringRef(&mod.Name[0]), modinfoDoc.GetAllocator());
		modinfoDoc["Mods"][currentModIndex].AddMember("Version", rapidjson::StringRef(&mod.Version[0]), modinfoDoc.GetAllocator());
		modinfoDoc["Mods"][currentModIndex].AddMember("RequiredOnClient", mod.RequiredOnClient, modinfoDoc.GetAllocator());
		modinfoDoc["Mods"][currentModIndex].AddMember("Pdiff", rapidjson::StringRef(&mod.Pdiff[0]), modinfoDoc.GetAllocator());

		currentModIndex++;
	}

	rapidjson::StringBuffer buffer;
	buffer.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	modinfoDoc.Accept(writer);

	m_bHasLoadedMods = true;

	ReloadMapsList();
}

void ModManager::UnloadMods()
{
	// clean up stuff from mods before we unload
	m_vMapList.clear();
	m_ModFiles.clear();
	fs::remove_all(GetCompiledAssetsPath());

	g_CustomAudioManager.ClearAudioOverrides();

	if (!m_bHasEnabledModsCfg)
		m_EnabledModsCfg.SetObject();

	for (Mod& mod : m_LoadedMods)
	{
		// remove all built kvs
		for (std::pair<size_t, std::string> kvPaths : mod.KeyValues)
			fs::remove(GetCompiledAssetsPath() / fs::path(kvPaths.second).lexically_relative(mod.m_ModDirectory));

		mod.KeyValues.clear();

		// write to m_enabledModsCfg
		// should we be doing this here or should scripts be doing this manually?
		// main issue with doing this here is when we reload mods for connecting to a server, we write enabled mods, which isn't necessarily
		// what we wanna do
		if (!m_EnabledModsCfg.HasMember(mod.Name.c_str()))
			m_EnabledModsCfg.AddMember(rapidjson_document::StringRefType(mod.Name.c_str()), false, m_EnabledModsCfg.GetAllocator());

		m_EnabledModsCfg[mod.Name.c_str()].SetBool(mod.m_bEnabled);
	}

	std::ofstream writeStream(g_svProfileDir + "/enabledmods.json");
	rapidjson::OStreamWrapper writeStreamWrapper(writeStream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(writeStreamWrapper);
	m_EnabledModsCfg.Accept(writer);

	// do we need to dealloc individual entries in m_loadedMods? idk, rework
	m_LoadedMods.clear();
}

std::string ModManager::NormaliseModFilePath(const fs::path path)
{
	std::string str = path.lexically_normal().string();

	// force to lowercase
	for (char& c : str)
		if (c <= 'Z' && c >= 'A')
			c = c - ('Z' - 'z');

	return str;
}

void ModManager::CompileAssetsForFile(const char* filename)
{
	size_t fileHash = STR_HASH(NormaliseModFilePath(fs::path(filename)));

	if (fileHash == m_hScriptsRsonHash)
		BuildScriptsRson();
	else if (fileHash == m_hPdefHash)
		BuildPdef();
	else if (fileHash == m_hKBActHash)
		BuildKBActionsList();
	else
	{
		// check if we should build keyvalues, depending on whether any of our mods have patch kvs for this file
		for (Mod& mod : m_LoadedMods)
		{
			if (!mod.m_bEnabled)
				continue;

			if (mod.KeyValues.find(fileHash) != mod.KeyValues.end())
			{
				TryBuildKeyValues(filename);
				return;
			}
		}
	}
}

void ModManager::ReloadMapsList()
{
	for (auto& modFilePair : m_ModFiles)
	{
		ModOverrideFile file = modFilePair.second;
		if (file.m_Path.extension() == ".bsp" && file.m_Path.parent_path().string() == "maps") // only allow mod maps actually in /maps atm
		{
			MapVPKInfo_t& map = m_vMapList.emplace_back();
			map.svName = file.m_Path.stem().string();
			map.svParent = file.m_pOwningMod->Name;
			map.eSource = ModManager::MOD;
		}
	}

	// get maps in vpk
	{
		const int iNumRetailNonMapVpks = 1;
		static const char* const ppRetailNonMapVpks[] = {"englishclient_frontend.bsp.pak000_dir.vpk"}; // don't include mp_common here as it contains mp_lobby

		// matches directory vpks, and captures their map name in the first group
		static const std::regex rVpkMapRegex("englishclient_([a-zA-Z0-9_]+)\\.bsp\\.pak000_dir\\.vpk", std::regex::icase);

		for (fs::directory_entry file : fs::directory_iterator("./vpk"))
		{
			std::string pathString = file.path().filename().string();

			bool bIsValidMapVpk = true;
			for (int i = 0; i < iNumRetailNonMapVpks; i++)
			{
				if (!pathString.compare(ppRetailNonMapVpks[i]))
				{
					bIsValidMapVpk = false;
					break;
				}
			}

			if (!bIsValidMapVpk)
				continue;

			// run our map vpk regex on the filename
			std::smatch match;
			std::regex_match(pathString, match, rVpkMapRegex);

			if (match.length() < 2)
				continue;

			std::string mapName = match[1].str();
			// special case: englishclient_mp_common contains mp_lobby, so hardcode the name here
			if (mapName == "mp_common")
				mapName = "mp_lobby";

			MapVPKInfo_t& map = m_vMapList.emplace_back();
			map.svName = mapName;
			map.svParent = pathString;
			map.eSource = ModManager::VPK;
		}
	}

	// get maps in game dir
	for (fs::directory_entry file : fs::directory_iterator(fmt::format("{}/maps", "r2"))) // assume mod dir
	{
		if (file.path().extension() == ".bsp")
		{
			MapVPKInfo_t& map = m_vMapList.emplace_back();
			map.svName = file.path().stem().string();
			map.svParent = "R2";
			map.eSource = ModManager::GAMEDIR;
		}
	}
}

fs::path GetModFolderPath()
{
	return fs::path(g_svProfileDir + MOD_FOLDER_SUFFIX);
}
fs::path GetThunderstoreModFolderPath()
{
	return fs::path(g_svProfileDir + THUNDERSTORE_MOD_FOLDER_SUFFIX);
}
fs::path GetRemoteModFolderPath()
{
	return fs::path(g_svProfileDir + REMOTE_MOD_FOLDER_SUFFIX);
}
fs::path GetCompiledAssetsPath()
{
	return fs::path(g_svProfileDir + COMPILED_ASSETS_SUFFIX);
}

ON_DLL_LOAD_RELIESON("engine.dll", ModManager, ConVar, (CModule module))
{
	g_pModManager = new ModManager;
}
