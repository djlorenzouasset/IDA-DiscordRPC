#include "form_manager.h"
#include "plugin_settings.h"
#include "../logger.h"

void OpenSettingsForm()
{
	ushort rpcFlags = 0x0;
	if (gPluginSettings.RPCEnabled) rpcFlags |= RPCEnabled;
	if (gPluginSettings.ShowFilename) rpcFlags |= ShowFilename;
	if (gPluginSettings.ShowFunctionName) rpcFlags |= ShowFunctionName;

	ushort customRPCFlags = 0x0;
	if (gPluginSettings.UseCustomRPC) customRPCFlags |= UseCustomRPC;

	ushort logsFlag = 0x0;
	if (gPluginSettings.WriteLogsToOutput) logsFlag |= WriteLogsInOutput;

	qstrvec_t addressTypes;
	addressTypes.push_back(" Hidden   ");
	addressTypes.push_back(" Cursor Address   ");
	addressTypes.push_back(" Function Address   ");

	qstrvec_t timestampTypes;
	timestampTypes.push_back(" Persistent  ");
	timestampTypes.push_back(" Between Database  ");

	qstring discordAppID = gPluginSettings.DiscordAppId;
	qstring largeImageKey = gPluginSettings.LargeImageKey;

	const char* form =
		"BUTTON YES* Save\n"
		"BUTTON NO Discard\n"
		"BUTTON CANCEL Close\n"
		"AUTOSYNC\n"

		PLUGIN_NAME SEPARATOR

		"%/\n" // set a callback for form changes

		"Discord Activity Settings\n"
		"<#Show IDA in your Discord activity#RPC Enabled:C>\n"
		"<#Show the current filename in the RPC#Show filename:C2>\n"
		"<#Show the viewing function in the RPC#Show function:C3>1>\n"

		"<#Choose how to calculate the timestamp shown in the activity#Timer mode     :b5:0:20::>\n"
		"<#Choose what address to show within the function name#Address to show:b4:0:20::>\n"

		SEPARATOR

		"Custom Activity Settings\n"
		"<#Use a custom RPC. " DONOTCHANGE_WARN "#Use custom RPC:C6>>\n"
		"<#Discord Application ID. " DONOTCHANGE_WARN "#Application ID :q7:0:20::>\n"
		"<#Activity Large Image Key " DONOTCHANGE_WARN "#Large Image Key:q8:0:20::>\n"

		SEPARATOR

		"Logging\n"
		"<Write logs to output:C>>\n"

		SEPARATOR

		"Plugin version: v" PLUGIN_VER " by @djlorenzouasset\n"
		"Github: " PLUGIN_URL

		SEPARATOR
	;

	int32 result = ask_form(
		form,
		OnOptionChanged,
		&rpcFlags,
		&timestampTypes,
		&gPluginSettings.Timestamp,
		&addressTypes, 
		&gPluginSettings.Address,
		&customRPCFlags,
		&discordAppID,
		&largeImageKey,
		&logsFlag
	);

	if (result == ASKBTN_CANCEL || result == ASKBTN_NO)
	{
		PluginLog<true, false>("Options not saved.");
	}
	else if (result == ASKBTN_YES)
	{
		/* RPC */
		gPluginSettings.RPCEnabled = ((rpcFlags & RPCEnabled) != 0);
		gPluginSettings.ShowFilename = ((rpcFlags & ShowFilename) != 0);
		gPluginSettings.ShowFunctionName = ((rpcFlags & ShowFunctionName) != 0);

		/* custom RPC */
		gPluginSettings.UseCustomRPC = ((customRPCFlags & UseCustomRPC) != 0);
		qstrncpy(gPluginSettings.DiscordAppId, discordAppID.c_str(), sizeof(gPluginSettings.DiscordAppId));
		qstrncpy(gPluginSettings.LargeImageKey, largeImageKey.c_str(), sizeof(gPluginSettings.LargeImageKey));

		/* other */
		gPluginSettings.WriteLogsToOutput = ((logsFlag & WriteLogsInOutput) != 0);

		gPluginSettings.Validate();
		gPluginSettings.Save(false);
	}
}

int32 idaapi OnOptionChanged(int32 fid, form_actions_t& fa)
{
	ushort rpcFlags = 0x0;
	ushort customRPCFlag = 0x0;
	fa.get_cbgroup_value(FIELD_RPCGroup, &rpcFlags);
	fa.get_checkbox_value(FIELD_CustomRPCEnabled, &customRPCFlag);

	bool rpcEnabled = ((rpcFlags & RPCEnabled) != 0);
	bool functionNameEnabled = ((rpcFlags & ShowFunctionName) != 0);
	bool customRPCEnabled = ((customRPCFlag & UseCustomRPC) != 0);

	fa.enable_field(FIELD_ShowFilename, rpcEnabled);
	fa.enable_field(FIELD_ShowFunctionName, rpcEnabled);
	fa.enable_field(FIELD_Address, rpcEnabled && functionNameEnabled);
	fa.enable_field(FIELD_Timestamp, rpcEnabled);

	fa.enable_field(FIELD_CustomRPCEnabled, rpcEnabled);
	fa.enable_field(FIELD_DiscordAppId, rpcEnabled && customRPCEnabled);
	fa.enable_field(FIELD_LargeImageKey, rpcEnabled && customRPCEnabled);

	return 1;
}
