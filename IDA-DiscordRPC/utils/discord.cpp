#include "ida.h"
#include "discord.h"
#include "../logger.h"

void DiscordRPC::OnReady(const DiscordUser* user)
{
	PluginLog("Discord RPC started for %s (%s)", user->username, user->userId);
}

void DiscordRPC::OnDisconnection(int32 errorCode, const char* message)
{
	switch (errorCode)
	{
	case DISCORD_NOERR:
		PluginLog("Discord RPC stopped");
		break;
	case DISCORD_ERR_INVALIDCLIENTID:
		PluginLog<true, true, MessageBoxType::Warn>("Discord RPC disconnected with error INVALID_CLIENTID (%d): %s", errorCode, message);
		break;
	default:
		PluginLog<true, true, MessageBoxType::Warn>("Discord RPC disconnected with error %d: %s", errorCode, message);
		break;
	}
}

void DiscordRPC::OnError(int32 errorCode, const char* message)
{
	PluginLog<true, true, MessageBoxType::Warn>("Discord RPC error %d: %s", errorCode, message);
}

void DiscordRPC::Initialize()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));

	handlers.ready = OnReady;
	handlers.disconnected = OnDisconnection;
	handlers.errored = OnError;

	Started = true;
	Discord_Initialize(gPluginSettings.UseCustomRPC ? gPluginSettings.DiscordAppId : DEFAULT_DISCORD_APPID, &handlers, 1, NULL);
	PluginLog("Discord RPC started: AppID = %s", gPluginSettings.DiscordAppId);

	memset(&RPC, 0, sizeof(RPC)); // init DiscordRichPresence
	Update(); // call update so we can show the default RPC
}

void DiscordRPC::Stop()
{
	Discord_ClearPresence();

	memset(&RPC, 0, sizeof(RPC));
	Discord_Shutdown();

	PluginLog("Successfully shutdown Discord RPC");
}

void DiscordRPC::Update(const bool resetTime)
{
	if (!gPluginSettings.RPCEnabled)
	{
		Discord_ClearPresence();
		return;
	}

	RPC.largeImageText = "IDA Pro";
	RPC.largeImageKey = gPluginSettings.UseCustomRPC ? gPluginSettings.LargeImageKey : DEFAULT_DISCORD_LARGEIMAGEKEY;

	// sets a new timestamp when the app starts or a new DB is loaded (based on the user preferences)
	if ((gPluginSettings.Timestamp == TimestampShowing::BetweenDatabase && resetTime) ||
		(gPluginSettings.Timestamp == TimestampShowing::Persistent && RPC.startTimestamp == 0))
	{
		RPC.startTimestamp = time(0);
	}

	char detailsBuffer[MAXSTR];
	const char* filename = Ida::GetDBFilename();
	if (!gPluginSettings.ShowFilename || !filename /*|| strcmp(filename, "(null)") == 0*/)
	{
		qsnprintf(detailsBuffer, sizeof(detailsBuffer), "");
	}
	else
	{
		qsnprintf(detailsBuffer, sizeof(detailsBuffer), "%s", filename);
	}
	RPC.details = detailsBuffer;

	char stateBuffer[MAXSTR];
	const char* functionName = Ida::GetFunctionName();
	if (!gPluginSettings.ShowFunctionName || !functionName || strcmp(functionName, "(null)") == 0)
	{
		qsnprintf(stateBuffer, sizeof(stateBuffer), "Idle");
	}
	else
	{
		switch (gPluginSettings.Address)
		{
		case AddressShowing::Hidden:
			qsnprintf(stateBuffer, sizeof(stateBuffer), "Reversing %s", functionName);
			break;
		case AddressShowing::Cursor:
			qsnprintf(stateBuffer, sizeof(stateBuffer), "Reversing %s (0x%a)", functionName, Ida::GetCursorAddr());
			break;
		case AddressShowing::Function:
			qsnprintf(stateBuffer, sizeof(stateBuffer), "Reversing %s (0x%a)", functionName, Ida::GetFunctionAddr());
			break;
		}
	}
	RPC.state = stateBuffer;

	Discord_UpdatePresence(&RPC);
}
