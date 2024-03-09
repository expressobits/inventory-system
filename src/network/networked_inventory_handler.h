#ifndef NETWORKED_INVENTORY_HANDLER_CLASS_H
#define NETWORKED_INVENTORY_HANDLER_CLASS_H

#include "core/inventory_handler.h"
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

// Network version of the InventoryHandler, the main inventory handler calls are
// intercepted by this script to propagate rpc calls to the server and later if
// necessary make response rpc calls to the client.

class NetworkedInventoryHandler : public InventoryHandler {
	GDCLASS(NetworkedInventoryHandler, InventoryHandler);

private:
	NodePath drop_item_spawner_path = NodePath("../../../DroppedItemSpawner");
	_FORCE_INLINE_ void create_rpc_config(const String method_name, MultiplayerAPI::RPCMode rpc_mode = MultiplayerAPI::RPC_MODE_ANY_PEER, const int channel = 0, const bool call_local = false, MultiplayerPeer::TransferMode transfer_mode = MultiplayerPeer::TRANSFER_MODE_RELIABLE) {
		Dictionary method_rpc_config;
		method_rpc_config["rpc_mode"] = rpc_mode;
		method_rpc_config["channel"] = channel;
		method_rpc_config["call_local"] = call_local;
		method_rpc_config["transfer_mode"] = transfer_mode;
		rpc_config(method_name, method_rpc_config);
	}

protected:
	static void _bind_methods();
	virtual void _instantiate_dropped_item(Ref<PackedScene> &dropped_item, const Ref<Item> &item) override;

public:
	virtual void _ready() override;
	void set_drop_item_spawner_path(const NodePath new_drop_item_spawner_path);
	NodePath get_drop_item_spawner_path() const;
	bool drop(const Ref<Item> &item, const int &amount = 1) override;
	bool pick_to_inventory(Node *dropped_item, Inventory *inventory = nullptr) override;
	void move_between_inventories_at(Inventory *from, const int slot_index, const int amount, Inventory *to, const int to_slot_index) override;
	bool open(Inventory *inventory) override;
	bool close(Inventory *inventory) override;
	void close_all_inventories() override;
	void to_transaction(const int &slot_index, Inventory *inventory, const int &amount) override;
	void transaction_to_at(const int &slot_index, Inventory *inventory, const int &amount_to_move = -1) override;
	void transaction_to(Inventory *inventory) override;
	// === CLIENT COMMANDS TO SERVER ===
	void drop_rpc(const int item_id, const int amount, const Dictionary properties);
	void add_to_inventory_rpc(const NodePath object_path, const int item_id, const int amount = 1, const bool drop_excess = false);
	void pick_to_inventory_rpc(const NodePath pick_object_path, const NodePath object_path);
	void move_between_inventories_at_rpc(const NodePath from_path, const int slot_index, const int amount, const NodePath to_path, const int to_slot_index);
	void open_rpc(const NodePath object_path);
	void close_rpc(const NodePath object_path);
	void to_transaction_rpc(const int slot_index, const NodePath object_path, const int amount);
	void transaction_to_at_rpc(const int slot_index, const NodePath object_path, const int amount_to_move);
	void transaction_to_rpc(const NodePath object_path);
	void close_all_inventories_rpc();
};

#endif // NETWORKED_INVENTORY_HANDLER_CLASS_H