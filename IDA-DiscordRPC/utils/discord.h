#pragma once
#include "discord_rpc.h"
#include "../includes.h"

#define DISCORD_NOERR 1000
#define DISCORD_ERR_INVALIDCLIENTID 4000

class DiscordRPC
{
private:
	bool Started = false;
	DiscordRichPresence RPC;

private:
	static void OnReady(const DiscordUser* user);
	static void OnDisconnection(int32 errorCode, const char* message);
	static void OnError(int32 errorCode, const char* message);

public:
	void Initialize();
	void Stop();

	void Update(const bool resetTime = false);
};
