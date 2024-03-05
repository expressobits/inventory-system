#include "networked_hotbar.h"
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

void NetworkedHotbar::change_selection_rpc(const int new_index) {
	if (!get_multiplayer()->is_server())
		return;
	rpc("change_selection_response_rpc", new_index);
}

void NetworkedHotbar::change_selection_response_rpc(const int new_index) {
    Hotbar::change_selection(new_index);
}

void NetworkedHotbar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("change_selection_rpc"), &NetworkedHotbar::change_selection_rpc);
	ClassDB::bind_method(D_METHOD("change_selection_response_rpc"), &NetworkedHotbar::change_selection_response_rpc);
}

void NetworkedHotbar::_ready() {
	Dictionary change_selection_rpc_config;
	change_selection_rpc_config["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
	change_selection_rpc_config["channel"] = 0;
	change_selection_rpc_config["call_local"] = false;
	change_selection_rpc_config["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
	rpc_config("change_selection_rpc", change_selection_rpc_config);

	Dictionary change_selection_response_rpc_config;
	change_selection_response_rpc_config["rpc_mode"] = MultiplayerAPI::RPC_MODE_AUTHORITY;
	change_selection_response_rpc_config["channel"] = 0;
	change_selection_response_rpc_config["call_local"] = true;
	change_selection_response_rpc_config["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
	rpc_config("change_selection_response_rpc", change_selection_response_rpc_config);
}

void NetworkedHotbar::change_selection(const int &new_index) {
	if (!get_multiplayer()->is_server()) {
		rpc_id(1, "change_selection_rpc", new_index);
	} else {
		change_selection_rpc(new_index);
	}
}
