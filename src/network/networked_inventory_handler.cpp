#include "networked_inventory_handler.h"
#include <godot_cpp/variant/utility_functions.hpp>

void NetworkedInventoryHandler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_drop_item_spawner_path", "drop_item_spawner_path"), &NetworkedInventoryHandler::set_drop_item_spawner_path);
	ClassDB::bind_method(D_METHOD("get_drop_item_spawner_path"), &NetworkedInventoryHandler::get_drop_item_spawner_path);
	// ClassDB::bind_method(D_METHOD("drop_rpc", "item_id", "amount", "properties"), &NetworkedInventoryHandler::drop_rpc);
	ClassDB::bind_method(D_METHOD("add_to_inventory_rpc", "object_path", "item_id", "amount", "drop_excess"), &NetworkedInventoryHandler::add_to_inventory_rpc);
	ClassDB::bind_method(D_METHOD("pick_to_inventory_rpc", "pick_object_path", "object_path"), &NetworkedInventoryHandler::pick_to_inventory_rpc);
	ClassDB::bind_method(D_METHOD("move_between_inventories_at_rpc", "from_path", "slot_index", "amount", "to_path"), &NetworkedInventoryHandler::move_between_inventories_at_rpc);
	ClassDB::bind_method(D_METHOD("open_rpc"), &NetworkedInventoryHandler::open_rpc);
	ClassDB::bind_method(D_METHOD("close_rpc"), &NetworkedInventoryHandler::close_rpc);
	ClassDB::bind_method(D_METHOD("to_transaction_rpc", "slot_index", "object_path", "amount"), &NetworkedInventoryHandler::to_transaction_rpc);
	ClassDB::bind_method(D_METHOD("transaction_to_at_rpc", "slot_index", "object_path", "amount_to_move"), &NetworkedInventoryHandler::transaction_to_at_rpc);
	ClassDB::bind_method(D_METHOD("transaction_to_rpc", "object_path"), &NetworkedInventoryHandler::transaction_to_rpc);
	ClassDB::bind_method(D_METHOD("close_all_inventories_rpc"), &NetworkedInventoryHandler::close_all_inventories_rpc);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "drop_item_spawner_path"), "set_drop_item_spawner_path", "get_drop_item_spawner_path");
}

void NetworkedInventoryHandler::_ready() {
	
	create_rpc_config("drop_rpc");
	create_rpc_config("add_to_inventory_rpc");
	create_rpc_config("pick_to_inventory_rpc");
	create_rpc_config("move_between_inventories_at_rpc");
	create_rpc_config("drop_transaction_rpc");
	create_rpc_config("open_rpc");
	create_rpc_config("close_rpc");
	create_rpc_config("to_transaction_rpc");
	create_rpc_config("transaction_to_at_rpc");
	create_rpc_config("transaction_to_rpc");
	create_rpc_config("close_all_inventories_rpc");
	InventoryHandler::_ready();
}

void NetworkedInventoryHandler::set_drop_item_spawner_path(const NodePath new_drop_item_spawner_path) {
	drop_item_spawner_path = new_drop_item_spawner_path;
}

NodePath NetworkedInventoryHandler::get_drop_item_spawner_path() const {
	return drop_item_spawner_path;
}

// bool NetworkedInventoryHandler::drop(const Ref<Item> &item, const int &amount) {
// 	int item_id = item->get_definition()->get_id();
// 	if (item_id <= ItemDefinition::NONE) {
// 		return false;
// 	}
// 	if (!get_multiplayer()->is_server())
// 		rpc_id(1, "drop_rpc", item_id, amount, item->get_properties());
// 	else
// 		drop_rpc(item_id, amount, item->get_properties());
// 	return true;
// }

bool NetworkedInventoryHandler::pick_to_inventory(Node *dropped_item, Inventory *inventory) {
	if (inventory == nullptr) {
		inventory = get_inventory(0);
	}
	if (!get_multiplayer()->is_server())
		rpc_id(1, "pick_to_inventory_rpc", dropped_item->get_path(), inventory->get_path());
	else
		pick_to_inventory_rpc(dropped_item->get_path(), inventory->get_path());
	return true;
}

// TODO Make Networked Function
//func move_between_inventories(from : Inventory, slot_index : int, amount : int, to : Inventory) -> int:
// #	if not multiplayer.is_server():
// #		move_between_inventories_rpc.rpc_id(1, from.get_path(), slot_index, amount, to.get_path())
// #	else:
// #		move_between_inventories_rpc(from.get_path(), slot_index, amount, to.get_path())

void NetworkedInventoryHandler::move_between_inventories_at(Inventory *from, const int slot_index, const int amount, Inventory *to, const int to_slot_index) {
	if (!get_multiplayer()->is_server())
		rpc_id(1, "move_between_inventories_at_rpc", from->get_path(), slot_index, amount, to->get_path(), to_slot_index);
	else
		move_between_inventories_at_rpc(from->get_path(), slot_index, amount, to->get_path(), to_slot_index);
}

bool NetworkedInventoryHandler::open(Inventory *inventory) {
	if (!get_multiplayer()->is_server()) {
		rpc_id(1, "open_rpc", inventory->get_path());
	} else
		open_rpc(inventory->get_path());
	return true;
}

bool NetworkedInventoryHandler::close(Inventory *inventory) {
	if (!get_multiplayer()->is_server())
		rpc_id(1, "close_rpc", inventory->get_path());
	else
		close_rpc(inventory->get_path());
	return true;
}

void NetworkedInventoryHandler::close_all_inventories() {
	if (!get_multiplayer()->is_server())
		rpc_id(1, "close_all_inventories_rpc");
	else
		close_all_inventories_rpc();
}

void NetworkedInventoryHandler::to_transaction(const int &slot_index, Inventory *inventory, const int &amount) {
	ERR_FAIL_NULL_MSG(inventory, "Inventory is null in to_transaction method on NetworkedInventoryHandler!");
	if (!get_multiplayer()->is_server())
		rpc_id(1, "to_transaction_rpc", slot_index, inventory->get_path(), amount);
	else
		to_transaction_rpc(slot_index, inventory->get_path(), amount);
}

void NetworkedInventoryHandler::transaction_to_at(const int &slot_index, Inventory *inventory, const int &amount_to_move) {
	ERR_FAIL_NULL_MSG(inventory, "Inventory is null in transaction_to_at method on NetworkedInventoryHandler!");
	if (!get_multiplayer()->is_server())
		rpc_id(1, "transaction_to_at_rpc", slot_index, inventory->get_path(), amount_to_move);
	else
		transaction_to_at_rpc(slot_index, inventory->get_path(), amount_to_move);
}

void NetworkedInventoryHandler::transaction_to(Inventory *inventory) {
	ERR_FAIL_NULL_MSG(inventory, "Inventory is null in transaction_to method on NetworkedInventoryHandler!");
	if (!get_multiplayer()->is_server())
		rpc_id(1, "transaction_to_rpc", inventory->get_path());
	else
		transaction_to_rpc(inventory->get_path());
}

// void NetworkedInventoryHandler::drop_rpc(const int item_id, const int amount, const Dictionary properties) {
// 	if (!get_multiplayer()->is_server())
// 		return;
// 	Ref<ItemDefinition> definition = get_item_from_id(item_id);
// 	if (definition == nullptr)
// 		return;
// 	Ref<Item> item = memnew(Item());
// 	item->set_definition(definition);
// 	item->set_properties(properties);
// 	InventoryHandler::drop(item, amount);
// }

void NetworkedInventoryHandler::add_to_inventory_rpc(const NodePath object_path, const int item_id, const int amount, const bool drop_excess) {
	if (!get_multiplayer()->is_server())
		return;
	Ref<ItemDefinition> definition = get_item_from_id(item_id);
	if (definition == nullptr)
		return;
	Ref<Item> item = memnew(Item());
	item->set_definition(definition);
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::add_to_inventory(inventory, item, amount, drop_excess);
}

void NetworkedInventoryHandler::pick_to_inventory_rpc(const NodePath pick_object_path, const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	Node *pick_object = get_node<Node>(pick_object_path);
	if (pick_object == nullptr) {
		UtilityFunctions::print("pick_object == nullptr");
		return;
	}
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr) {
		UtilityFunctions::print("inventory == nullptr");
		return;
	}
	InventoryHandler::pick_to_inventory(pick_object, inventory);
}

void NetworkedInventoryHandler::move_between_inventories_at_rpc(const NodePath from_path, const int slot_index, const int amount, const NodePath to_path, const int to_slot_index) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *from = get_node<Inventory>(from_path);
	if (from == nullptr)
		return;
	Inventory *to = get_node<Inventory>(to_path);
	if (to == nullptr)
		return;
	InventoryHandler::move_between_inventories_at(from, slot_index, amount, to, to_slot_index);
}

void NetworkedInventoryHandler::open_rpc(const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::open(inventory);
}

void NetworkedInventoryHandler::close_rpc(const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::close(inventory);
}

void NetworkedInventoryHandler::to_transaction_rpc(const int slot_index, const NodePath object_path, const int amount) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::to_transaction(slot_index, inventory, amount);
}

void NetworkedInventoryHandler::transaction_to_at_rpc(const int slot_index, const NodePath object_path, const int amount_to_move) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::transaction_to_at(slot_index, inventory, amount_to_move);
}

void NetworkedInventoryHandler::transaction_to_rpc(const NodePath object_path) {
	if (!get_multiplayer()->is_server())
		return;
	Inventory *inventory = get_node<Inventory>(object_path);
	if (inventory == nullptr)
		return;
	InventoryHandler::transaction_to(inventory);
}

void NetworkedInventoryHandler::close_all_inventories_rpc() {
	InventoryHandler::close_all_inventories();
}

void NetworkedInventoryHandler::_instantiate_dropped_item(Ref<PackedScene> &dropped_item, const Ref<Item> &item) {
	// TODO move to property "../../../DroppedItemSpawner"
	Node *spawner = get_node<Node>(drop_item_spawner_path);
	ERR_FAIL_NULL_MSG(spawner, "Spawner is null in _instantiate_dropped_item method on NetworkedInventoryHandler!");
	if (spawner == nullptr) {
		return;
	}
	Node *position_reference = get_node<Node>(get_drop_parent_position_path());
	Vector3 position = position_reference->get("position");
	Vector3 rotation = position_reference->get("rotation");
	Array data = Array();
	data.append(position);
	data.append(rotation);
	data.append(dropped_item->get_path());
	data.append(item->get_properties());
	Variant obj_var = spawner->call("spawn", data);
	emit_signal("dropped", obj_var);
}
