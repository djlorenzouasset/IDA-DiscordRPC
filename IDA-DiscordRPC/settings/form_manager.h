#pragma once
#include "../includes.h"

#define SEPARATOR "\n"
#define DONOTCHANGE_WARN "!! DO NOT CHANGE IF YOU DONT KNOW WHAT YOU'RE DOING !!"

enum RPCBitmask : int32
{
	RPCEnabled			= (1 << 0),
	ShowFilename		= (1 << 1),
	ShowFunctionName	= (1 << 2),
};

// i know the following enums are very useless and stupid
enum CustomRPCBitmask : int32
{
	UseCustomRPC = (1 << 0)
};

enum LogsBitmask : int32
{
	WriteLogsInOutput = (1 << 0)
};

/* enum used to identify UI fields */
enum OptionField : int32
{
	/* RPC */
	FIELD_RPCGroup			= 1,
	FIELD_ShowFilename		= 2,
	FIELD_ShowFunctionName	= 3,
	FIELD_Address			= 4,
	FIELD_Timestamp			= 5,

	/* CUSTOM RPC */
	FIELD_CustomRPCEnabled	= 6,
	FIELD_DiscordAppId		= 7,
	FIELD_LargeImageKey		= 8
};

void OpenSettingsForm();

int32 idaapi OnOptionChanged(int32 fid, form_actions_t& fa);
