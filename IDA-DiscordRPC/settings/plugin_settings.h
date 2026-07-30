#pragma once
#include "../includes.h"

#define SETTING_FILE "ida-discordrpc.save"
#define DEFAULT_DISCORD_APPID "1529972907072294962"
#define DEFAULT_DISCORD_LARGEIMAGEKEY "idaicon"

#define SETTING_OPTION(type, name, value) type name = value
#define SETTING_OPTION_CHAR(name, value) SETTING_OPTION(char, name[MAXSTR], value)

enum class AddressShowing : int32
{
	Hidden		= 0,
	Cursor		= 1,
	Function	= 2
};

enum class TimestampShowing : int32
{
	Persistent		= 0,
	BetweenDatabase	= 1
};

class UserSettings
{
public:
	/* Choose to show the IDA RPC to Discord */
	SETTING_OPTION(bool, RPCEnabled, true);
	/* Choose to show the filename in the Discord RPC */
	SETTING_OPTION(bool, ShowFilename, true);
	/* Choose to show the function name in the Discord RPC */
	SETTING_OPTION(bool, ShowFunctionName, true);

	/* Choose the type of cursor to show in the Discord RPC or disable it with AddressShowing::Hidden */
	SETTING_OPTION(AddressShowing, Address, AddressShowing::Function);
	/* Choose how to calculate the timestam to show in the Discord RPC or disable it with TimestampShowing::Hidden */
	SETTING_OPTION(TimestampShowing, Timestamp, TimestampShowing::Persistent);

	/* Choose whenever to use a custom Application ID for custom RPCs */
	SETTING_OPTION(bool, UseCustomRPC, false);
	/* Discord Application ID used to connect the RPC */
	SETTING_OPTION_CHAR(DiscordAppId, DEFAULT_DISCORD_APPID);
	/* Discord Large image key used to show an image in the RPC */
	SETTING_OPTION_CHAR(LargeImageKey, DEFAULT_DISCORD_LARGEIMAGEKEY);

	/* Choose if the plugin should write logs to the output window */
	SETTING_OPTION(bool, WriteLogsToOutput, true);

public:
	void Load();
	void Save(bool notify = true);

	/* validation (DiscordAppId) */
	void Validate();
};

inline UserSettings gPluginSettings;
