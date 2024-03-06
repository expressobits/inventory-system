#ifndef NETWORKED_CRAFT_STATION_CLASS_H
#define NETWORKED_CRAFT_STATION_CLASS_H

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include "craft/craft_station.h"

class NetworkedCraftStation : public CraftStation {
	GDCLASS(NetworkedCraftStation, CraftStation);

private:
    Array craftings_data_sync;
    _FORCE_INLINE_ void _create_rpc_config(const String method_name, MultiplayerAPI::RPCMode rpc_mode = MultiplayerAPI::RPC_MODE_AUTHORITY, const int channel = 0, const bool call_local = false, MultiplayerPeer::TransferMode transfer_mode = MultiplayerPeer::TRANSFER_MODE_RELIABLE) {
		Dictionary method_rpc_config;
		method_rpc_config["rpc_mode"] = rpc_mode;
		method_rpc_config["channel"] = channel;
		method_rpc_config["call_local"] = call_local;
		method_rpc_config["transfer_mode"] = transfer_mode;
		rpc_config(method_name, method_rpc_config);
	}
protected:
	static void _bind_methods();
    void _on_opened();
    void _on_closed();
    void _on_crafting_added(const int crafting_index);
    void _on_crafting_removed(const int crafting_index);
    // === MULTIPLAYER EVENTS ===
    void _on_connected(const int peer_id);

public:
	virtual void _ready() override;
    void set_craftings_data_sync(const Array new_craftings_data_sync);
    Array get_craftings_data_sync();
    // === OVERRIDE MAIN COMMANDS ===
    // Override all main commands for the client to send to the server through rpc
    virtual void craft(int recipe_index) override;
    virtual void cancel_craft(int crafting_index) override;
    virtual void finish_crafting(int crafting_index) override;
    // === CLIENT COMMANDS TO SERVER ===
    void craft_rpc(const int recipe_index);
    void cancel_craft_rpc(const int crafting_index);
    // === SERVER RESPONSES ===
    void _update_craftings_rpc(const Array craftings_data);
    void _crafting_added_rpc(const int recipe_index);
    void _crafting_removed_rpc(const int crafting_index);
    void _opened_rpc();
    void _closed_rpc();

};

#endif // NETWORKED_CRAFT_STATION_CLASS_H