#include "logger.h"
#include "utils/discord.h"
#include "settings/form_manager.h"
#include "settings/plugin_settings.h"

struct IDADiscordRPC : public plugmod_t
{
	struct IDBEventListener : public event_listener_t
	{
		IDADiscordRPC* plugin;
		IDBEventListener(IDADiscordRPC* plugin) : plugin(plugin) {}

		ssize_t idaapi on_event(ssize_t code, va_list args) override
		{
			switch (code)
			{
			case idb_event::closebase:
				plugin->DiscordRPC.Update(true);
				break;

			case idb_event::renamed:
			case idb_event::allsegs_moved:
			case idb_event::func_added:
			case idb_event::func_updated:
			case idb_event::deleting_func:
			case idb_event::set_func_start:
				plugin->DiscordRPC.Update();
				break;
			}

			return 0;
		}
	};

	struct IDPEventListener : public event_listener_t
	{
		IDADiscordRPC* plugin;
		IDPEventListener(IDADiscordRPC* plugin) : plugin(plugin) {}

		ssize_t idaapi on_event(ssize_t code, va_list args) override
		{
			switch (code)
			{
			case processor_t::ev_newfile:
			case processor_t::ev_oldfile:
				plugin->DiscordRPC.Update(true);
				break;
			}

			return 0;
		};
	};

	struct ViewEventListener : public event_listener_t
	{
		IDADiscordRPC* plugin;
		ViewEventListener(IDADiscordRPC* plugin) : plugin(plugin) {}

		ssize_t idaapi on_event(ssize_t code, va_list args) override
		{
			if (code == view_curpos)
			{
				plugin->DiscordRPC.Update();
			}

			return 0;
		};
	};

	/* listeners */
	IDBEventListener idbEventListener;
	IDPEventListener idpEventListener;
	ViewEventListener viewEventListener;

	/* other utils */
	DiscordRPC DiscordRPC;

	IDADiscordRPC() : DiscordRPC(), viewEventListener(this), idbEventListener(this), idpEventListener(this)
	{
		gPluginSettings.Load();
		gPluginSettings.Validate();

		DiscordRPC.Initialize();

		hook_event_listener(HT_IDP, &idpEventListener, HKCB_GLOBAL);
		hook_event_listener(HT_IDB, &idbEventListener, HKCB_GLOBAL);
		hook_event_listener(HT_VIEW, &viewEventListener, HKCB_GLOBAL);

		PluginLog("Successfully hooked events HT_IDP, HT_IDB, HT_VIEW");
		PluginLog("Loaded %s version v%s", PLUGIN_NAME, PLUGIN_VER);
	}

	~IDADiscordRPC()
	{
		PluginLog("Unloading plugin %s", PLUGIN_NAME);

		unhook_event_listener(HT_IDP, &idbEventListener);
		unhook_event_listener(HT_IDB, &idpEventListener);
		unhook_event_listener(HT_VIEW, &viewEventListener);
		PluginLog("Successfully unhooked all events");

		PluginLog("Stopping Discord RPC");
		DiscordRPC.Stop();
		gPluginSettings.Save();
	}

	bool idaapi run(size_t) override
	{
		// very stupid logic implementation
		bool oldUseCustomRPC = gPluginSettings.UseCustomRPC;

		OpenSettingsForm();
		if (oldUseCustomRPC != gPluginSettings.UseCustomRPC)
		{
			DiscordRPC.Stop();
			DiscordRPC.Initialize();
		}

		DiscordRPC.Update();
		return true;
	}
};

static plugmod_t* idaapi init()
{
	return new IDADiscordRPC;
}

plugin_t PLUGIN = {
	IDP_INTERFACE_VERSION,
	PLUGIN_MULTI | PLUGIN_FIX,
	init,
	nullptr,
	nullptr,
	PLUGIN_DESC,
	PLUGIN_HELP,
	PLUGIN_NAME,
	PLUGIN_KEYS
};
