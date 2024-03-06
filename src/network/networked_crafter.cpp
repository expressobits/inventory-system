#include "networked_crafter.h"

void NetworkedCrafter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("open_rpc", "object_path"), &NetworkedCrafter::open_rpc);
    ClassDB::bind_method(D_METHOD("close_rpc", "object_path"), &NetworkedCrafter::close_rpc);
    ClassDB::bind_method(D_METHOD("open_response_rpc", "object_path"), &NetworkedCrafter::open_response_rpc);
    ClassDB::bind_method(D_METHOD("close_response_rpc", "object_path"), &NetworkedCrafter::close_response_rpc);
}

void NetworkedCrafter::_ready() {
	_create_rpc_config("open_rpc", MultiplayerAPI::RPC_MODE_ANY_PEER);
	_create_rpc_config("close_rpc", MultiplayerAPI::RPC_MODE_ANY_PEER);
	_create_rpc_config("open_response_rpc");
	_create_rpc_config("close_response_rpc");
	Crafter::_ready();
}

bool NetworkedCrafter::open(CraftStation *craft_station) {
	if (!get_multiplayer()->is_server()) {
		rpc_id(1, "open_rpc", craft_station->get_path());
	} else {
		open_rpc(craft_station->get_path());
	}
	return true;
}

bool NetworkedCrafter::close(CraftStation *craft_station) {
	if (!get_multiplayer()->is_server()) {
		rpc_id(1, "close_rpc", craft_station->get_path());
	} else {
		close_rpc(craft_station->get_path());
	}
	return true;
}

void NetworkedCrafter::open_rpc(const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	CraftStation *craft_station = get_craft_station(object_path);
	if (Crafter::open(craft_station))
		rpc("open_response_rpc", object_path);
}

void NetworkedCrafter::close_rpc(const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	CraftStation *craft_station = get_craft_station(object_path);
	if (Crafter::close(craft_station))
		rpc("close_response_rpc", object_path);
}

void NetworkedCrafter::open_response_rpc(const NodePath object_path) {
	if (get_multiplayer()->is_server())
		return;
	CraftStation *craft_station = get_craft_station(object_path);
	opened_stations.append(object_path);
	emit_signal("opened", craft_station);
}

void NetworkedCrafter::close_response_rpc(const NodePath object_path) {
	if (get_multiplayer()->is_server())
		return;
	CraftStation *craft_station = get_craft_station(object_path);
	int index = opened_stations.find(object_path);
	if (index == -1)
		return;
	opened_stations.remove_at(index);
	emit_signal("closed", craft_station);
}
