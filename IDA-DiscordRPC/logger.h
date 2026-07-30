#pragma once
#include "settings/plugin_settings.h"

enum MessageBoxType : int32
{
	None = 0,
	Info = 1,
	Warn = 2
};

template<bool showInOutput = true, bool writeInLogs = true, MessageBoxType boxType = MessageBoxType::None>
static void PluginLog(const char* fmt, ...)
{
	// use this function so we can log in multiple ways at once

	qstring logMsg;
	va_list args;
	va_start(args, fmt);
	logMsg.vsprnt(fmt, args);
	va_end(args);

	if constexpr (showInOutput)
	{
		// even when showInOuput = true, check if the user wants 
		// the plugin to write in the output window
		if (gPluginSettings.WriteLogsToOutput)
		{
			char outputLogBuffer[4096];
			qsnprintf(outputLogBuffer, sizeof(outputLogBuffer), "[Discord-RPC] %s", logMsg.c_str());
			msg("%s\n", outputLogBuffer);
		}
	}

	if constexpr (writeInLogs)
	{
		char logPathBuffer[QMAXPATH];
		qmakepath(logPathBuffer, sizeof(logPathBuffer), get_user_idadir(), "IDADiscordRPC.log", nullptr);

		FILE* logFile = qfopen(logPathBuffer, "a");
		if (logFile)
		{
			qfprintf(logFile, "%s\n", logMsg.c_str());
			qfclose(logFile);
		}
	}

	switch (boxType)
	{
	case MessageBoxType::Info:
		info("%s", logMsg.c_str());
		break;
	case MessageBoxType::Warn:
		warning("%s", logMsg.c_str());
	default:
		break;
	}
}
