#ifndef NETWORKED_CRAFTER_CLASS_H
#define NETWORKED_CRAFTER_CLASS_H

#include "craft/crafter.h"
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

class NetworkedCrafter : public Crafter {
	GDCLASS(NetworkedCrafter, Crafter);

private:
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

public:
	virtual void _ready() override;
	virtual bool open(CraftStation *craft_station) override;
	virtual bool close(CraftStation *craft_station) override;
	// === CLIENT COMMANDS TO SERVER ===
	void open_rpc(const NodePath object_path);
	void close_rpc(const NodePath object_path);
	// === SERVER RESPONSES TO CLIENT ===
	void open_response_rpc(const NodePath object_path);
	void close_response_rpc(const NodePath object_path);
};

#endif // NETWORKED_CRAFTER_CLASS_H