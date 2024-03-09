#ifndef NETWORKED_INVENTORY_CLASS_H
#define NETWORKED_INVENTORY_CLASS_H

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include "core/inventory.h"

// Networked version of inventory where server sends rpcs to client for 
// slot update, add and remove signals

// Why not use [MultiplayerSyncronizer]?
// The idea of using rpc signals only when changed saves a lot of bandwidth, 
// but at the cost of being sure which signals will be called, ie calling 
// slot[i] = new Dictionary is not replicated across the network.
// Also keep in mind that signals need to be handled if switching to a use of
// MultiplayerSyncronizer

// Note: Slot categories are not synced

class NetworkedInventory : public Inventory {
    GDCLASS(NetworkedInventory, Inventory);

private:
    bool sync_item_added_signal = true;
    bool sync_item_removed_signal = true;
    Array slots_sync;
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

};

#endif // NETWORKED_INVENTORY_CLASS_H