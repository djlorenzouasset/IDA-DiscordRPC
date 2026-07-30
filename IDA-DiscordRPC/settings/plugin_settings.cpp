#include "../logger.h"
#include "plugin_settings.h"

void UserSettings::Load()
{
	char fileLocation[QMAXPATH];
	qsnprintf(fileLocation, sizeof(fileLocation), "%s\\%s", get_user_idadir(), SETTING_FILE);

	FILE* file = qfopen(fileLocation, "rb");
	if (!file)
	{
		PluginLog("Failed to load file %s: creating a settings file.", fileLocation);
		Save();
	}
	else
	{
		qfread(file, this, sizeof(this));
		qfclose(file);
		PluginLog("Loaded settings from %s", fileLocation);
	}
}

void UserSettings::Save(bool notify)
{
	char saveLocation[QMAXPATH];
	qsnprintf(saveLocation, sizeof(saveLocation), "%s\\%s", get_user_idadir(), SETTING_FILE);

	FILE* file = qfopen(saveLocation, "wb");
	if (!file) [[unlikely]]
	{
		PluginLog("Failed to write settings file to %s", saveLocation);
	}
	else
	{
		qfwrite(file, this, sizeof(this));
		qfclose(file);
		if (notify) PluginLog("Saved settings to %s", saveLocation);
		else PluginLog<false>("Saved settings to %s", saveLocation);
	}
}

void UserSettings::Validate()
{
	if (!UseCustomRPC) return;

	if (strlen(DiscordAppId) == 0)
	{
		qsnprintf(DiscordAppId, sizeof(DiscordAppId), "%s", DEFAULT_DISCORD_APPID);
		PluginLog<true, false /* do not write this in logs */>(
			"DiscordAppId was empty! Replacing back to default"
		);

		Save(); // prevent unsavings when IDA closes for crashes or taskkill
	}
}
