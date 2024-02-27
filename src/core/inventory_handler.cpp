#include "inventory_handler.h"
#include "core/categorized_slot.h"
#include "inventory.h"
#include <godot_cpp/classes/resource_loader.hpp>

void InventoryHandler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_drop_parent_path", "drop_parent_path"), &InventoryHandler::set_drop_parent_path);
	ClassDB::bind_method(D_METHOD("get_drop_parent_path"), &InventoryHandler::get_drop_parent_path);
	ClassDB::bind_method(D_METHOD("set_drop_parent_position_path", "drop_parent_position_path"), &InventoryHandler::set_drop_parent_position_path);
	ClassDB::bind_method(D_METHOD("get_drop_parent_position_path"), &InventoryHandler::get_drop_parent_position_path);
	ClassDB::bind_method(D_METHOD("set_inventories_path", "inventories_path"), &InventoryHandler::set_inventories_path);
	ClassDB::bind_method(D_METHOD("get_inventories_path"), &InventoryHandler::get_inventories_path);
	// ClassDB::bind_method(D_METHOD("set_transaction_slot", "transaction_slot"), &InventoryHandler::set_transaction_slot);
	ClassDB::bind_method(D_METHOD("get_transaction_slot"), &InventoryHandler::get_transaction_slot);
	ClassDB::bind_method(D_METHOD("drop", "item", "amount"), &InventoryHandler::drop, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("add_to_inventory", "inventory", "item", "amount", "drop_excess"), &InventoryHandler::add_to_inventory, DEFVAL(1), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("drop_from_inventory", "slot_index", "amount", "inventory"), &InventoryHandler::drop_from_inventory, DEFVAL(1), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("pick_to_inventory", "dropped_item", "inventory"), &InventoryHandler::pick_to_inventory, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("move_between_inventories", "from", "slot_index", "amount", "to"), &InventoryHandler::move_between_inventories);
	ClassDB::bind_method(D_METHOD("move_between_inventories_at", "from", "slot_index", "amount", "to", "to_slot_index"), &InventoryHandler::move_between_inventories_at);
	ClassDB::bind_method(D_METHOD("swap_between_inventories", "inventory", "slot_index", "other_inventory", "other_slot_index", "amount"), &InventoryHandler::swap_between_inventories, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("open", "inventory"), &InventoryHandler::open);
	ClassDB::bind_method(D_METHOD("close", "inventory"), &InventoryHandler::close);
	ClassDB::bind_method(D_METHOD("is_open_main_inventory"), &InventoryHandler::is_open_main_inventory);
	ClassDB::bind_method(D_METHOD("is_open_any_inventory"), &InventoryHandler::is_open_any_inventory);
	ClassDB::bind_method(D_METHOD("is_open", "inventory"), &InventoryHandler::is_open);
	ClassDB::bind_method(D_METHOD("open_main_inventory"), &InventoryHandler::open_main_inventory);
	ClassDB::bind_method(D_METHOD("close_main_inventory"), &InventoryHandler::close_main_inventory);
	ClassDB::bind_method(D_METHOD("close_all_inventories"), &InventoryHandler::close_all_inventories);
	ClassDB::bind_method(D_METHOD("to_transaction", "slot_index", "inventory", "amount"), &InventoryHandler::to_transaction);
	ClassDB::bind_method(D_METHOD("transaction_to_at", "slot_index", "inventory", "amount_to_move"), &InventoryHandler::transaction_to_at, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("transaction_to", "inventory"), &InventoryHandler::transaction_to);
	ClassDB::bind_method(D_METHOD("is_transaction_active"), &InventoryHandler::is_transaction_active);
	ClassDB::bind_method(D_METHOD("drop_transaction"), &InventoryHandler::drop_transaction);
	ClassDB::bind_method(D_METHOD("get_inventory", "index"), &InventoryHandler::get_inventory);
	ADD_SIGNAL(MethodInfo("dropped", PropertyInfo(Variant::OBJECT, "dropped_item")));
	ADD_SIGNAL(MethodInfo("picked", PropertyInfo(Variant::OBJECT, "dropped_item")));
	ADD_SIGNAL(MethodInfo("added", PropertyInfo(Variant::OBJECT, "item_definition", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("opened", PropertyInfo(Variant::OBJECT, "inventory")));
	ADD_SIGNAL(MethodInfo("closed", PropertyInfo(Variant::OBJECT, "inventory")));
	ADD_SIGNAL(MethodInfo("updated_transaction_slot"));
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "drop_parent_path"), "set_drop_parent_path", "get_drop_parent_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "drop_parent_position_path"), "set_drop_parent_position_path", "get_drop_parent_position_path");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "inventories_path", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory")), "set_inventories_path", "get_inventories_path");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "transaction_slot", PROPERTY_HINT_RESOURCE_TYPE, "Slot"), "", "get_transaction_slot");
}

void InventoryHandler::_instantiate_dropped_item(Ref<PackedScene> &dropped_item, const Ref<Item> &item) {
	Node *node = dropped_item->instantiate();
	ERR_FAIL_NULL_MSG(drop_parent, "Dropped parent path is null reference.");
	ERR_FAIL_NULL_MSG(drop_parent_position, "Dropped parent position path is null reference.");
	drop_parent->add_child(node);
	node->set("item", item);
	node->set("position", drop_parent_position->get("position"));
	node->set("rotation", drop_parent_position->get("position"));
	emit_signal("dropped", node);
}

void InventoryHandler::_set_transaction_slot(const Ref<Item> &item, const int &amount) {
	transaction_slot->set_amount(amount);
	if (amount > 0) {
		transaction_slot->set_item(item);
	} else {
		transaction_slot->set_item(nullptr);
	}
	emit_signal("updated_transaction_slot");
}

InventoryHandler::InventoryHandler() {
}

InventoryHandler::~InventoryHandler() {
}

void InventoryHandler::_ready() {
	drop_parent = get_node_or_null(drop_parent_path);
	drop_parent_position = get_node_or_null(drop_parent_position_path);
	set_transaction_slot(memnew(Slot()));
	NodeInventories::_ready();
}

void InventoryHandler::set_drop_parent_path(const NodePath new_drop_parent_path) {
	drop_parent_path = new_drop_parent_path;
}

NodePath InventoryHandler::get_drop_parent_path() const {
	return drop_parent_path;
}

void InventoryHandler::set_drop_parent_position_path(const NodePath new_drop_parent_position_path) {
	drop_parent_position_path = new_drop_parent_position_path;
}

NodePath InventoryHandler::get_drop_parent_position_path() const {
	return drop_parent_position_path;
}

void InventoryHandler::set_inventories_path(const TypedArray<NodePath> new_inventories_path) {
	inventories_path = new_inventories_path;
}

TypedArray<NodePath> InventoryHandler::get_inventories_path() const {
	return inventories_path;
}

void InventoryHandler::set_transaction_slot(const Ref<Slot> new_transaction_slot) {
	transaction_slot = new_transaction_slot;
}

Ref<Slot> InventoryHandler::get_transaction_slot() const {
	return transaction_slot;
}

bool InventoryHandler::drop(const Ref<Item> &item, const int &amount) {
	if (item->get_definition()->get_properties().has("dropped_item")) {
		String path = item->get_definition()->get_properties()["dropped_item"];
		ResourceLoader *resource_loader = ResourceLoader::get_singleton();
		Ref<PackedScene> dropped_item = resource_loader->load(path);
		for (size_t i = 0; i < amount; i++) {
			_instantiate_dropped_item(dropped_item, item);
		}
		return true;
	}
	return false;
}

int InventoryHandler::add_to_inventory(Inventory *inventory, const Ref<Item> item, const int &amount, bool drop_excess) {
	int value_no_added = inventory->add(item, amount);
	emit_signal("added", item, amount - value_no_added);
	if (drop_excess) {
		drop(item, value_no_added);
		return 0;
	}
	return value_no_added;
}

void InventoryHandler::drop_from_inventory(const int &slot_index, const int &amount, Inventory *inventory) {
	if (inventory == nullptr)
		inventory = get_inventory(0);
	if (inventory->get_slots().size() <= slot_index)
		return;
	if (inventory->is_empty_slot(slot_index))
		return;
	Ref<Slot> slot = inventory->get_slots()[slot_index];
	if (!slot->has_valid())
		return;
	Ref<Item> item = slot->get_item();
	int not_removed = inventory->remove_at(slot_index, item, amount);
	int removed = amount - not_removed;
	drop(item, removed);
}

bool InventoryHandler::pick_to_inventory(Node *dropped_item, Inventory *inventory) {
	if (inventory == nullptr) {
		inventory = get_inventory(0);
	}
	if (!dropped_item->get("is_pickable")) {
		return false;
	}
	Ref<Item> item = dropped_item->get("item");
	if (item == nullptr || item->get_definition() == nullptr) {
		ERR_PRINT("Item or Item Definition in dropped_item is null!");
	}
	if (add_to_inventory(inventory, item) == 0) {
		emit_signal("picked", dropped_item);
		dropped_item->queue_free();
		return true;
	}
	ERR_PRINT("pick_to_inventory return false");
	return false;
}

int InventoryHandler::move_between_inventories(Inventory *from, const int slot_index, const int amount, Inventory *to) {
	if (from->get_database() != to->get_database()) {
		ERR_PRINT("Operation between inventories that do not have the same database is invalid.");
		return amount;
	}
	Ref<Slot> slot = from->get_slots()[slot_index];
	Ref<Item> item = slot->get_item();
	int amount_not_removed = from->remove_at(slot_index, item, amount);
	int amount_for_swap = amount - amount_not_removed;
	int amount_not_swaped = to->add(item, amount_for_swap);
	int amount_not_undo = from->add_at(slot_index, item, amount_not_swaped);
	return amount_not_undo;
}

void InventoryHandler::move_between_inventories_at(Inventory *from, const int slot_index, const int amount, Inventory *to, const int to_slot_index) {
	if (from->get_database() != to->get_database()) {
		ERR_PRINT("Operation between inventories that do not have the same database is invalid.");
		return;
	}
	Ref<Slot> slot = from->get_slots()[slot_index];
	Ref<Item> item = slot->get_item()->duplicate();
	int amount_not_removed = from->remove_at(slot_index, item, amount);
	int amount_for_swap = amount - amount_not_removed;
	int amount_not_swaped = to->add_at(to_slot_index, item, amount_for_swap);
	int amount_not_undo = from->add_at(slot_index, item, amount_not_swaped);
	//drop(item, amount_not_undo)
}

void InventoryHandler::swap_between_inventories(Inventory *inventory, const int slot_index, Inventory *other_inventory, const int other_slot_index, int amount) {
	if (inventory->get_database() != other_inventory->get_database()) {
		ERR_PRINT("Operation between inventories that do not have the same database is invalid.");
		return;
	}
	Ref<Slot> slot = inventory->get_slots()[slot_index];
	Ref<Slot> other_slot = other_inventory->get_slots()[other_slot_index];
	if (other_inventory->is_empty_slot(other_slot_index) || slot->get_item()->get_definition() == other_slot->get_item()->get_definition()) {
		Ref<Item> item = slot->get_item();
		if (item == nullptr || item->get_definition() == nullptr) {
			return;
		}
		int for_trade = 0;
		if (other_inventory->is_empty_slot(other_slot_index)) {
			for_trade = amount;
		} else {
			for_trade = MIN(item->get_definition()->get_max_stack() - other_slot->get_amount(), amount);
		}
		int no_remove = inventory->remove_at(slot_index, item, for_trade);
		other_inventory->add_at(other_slot_index, item, for_trade - no_remove);
	} else {
		if (slot->get_amount() == amount) {
			inventory->set_slot_with_other_slot(slot_index, other_slot);
			other_inventory->set_slot_with_other_slot(other_slot_index, slot);
		}
	}
}

bool InventoryHandler::open(Inventory *inventory) {
	if (opened_inventories.has(inventory->get_path()))
		return false;
	if (!inventory->open())
		return false;
	opened_inventories.append(inventory->get_path());
	emit_signal("opened", inventory);
	return true;
}

bool InventoryHandler::close(Inventory *inventory) {
	int index = opened_inventories.find(inventory->get_path());
	if (index == -1)
		return false;
	if (!inventory->close())
		return false;
	if (inventories_path.find(this->get_path_to(inventory)) != -1) {
		if (is_transaction_active()) {
			Ref<Item> item = transaction_slot->get_item();
			int amount_no_add = inventory->add(item, transaction_slot->get_amount());
			if (amount_no_add > 0)
				drop(item, amount_no_add);
			_set_transaction_slot(nullptr, 0);
		}
	}
	opened_inventories.remove_at(index);
	emit_signal("closed", inventory);
	return true;
}

bool InventoryHandler::is_open_main_inventory() const {
	Inventory *inventory = get_inventory(0);
	return inventory != nullptr && is_open(inventory);
}

bool InventoryHandler::is_open_any_inventory() const {
	return opened_inventories.size() > 0;
}

bool InventoryHandler::is_open(const Inventory *inventory) const {
	return inventory->get_is_open() && opened_inventories.find(inventory->get_path()) != -1;
}

bool InventoryHandler::open_main_inventory() {
	Inventory *inventory = get_inventory(0);
	return inventory != nullptr && open(inventory);
}

bool InventoryHandler::close_main_inventory() {
	Inventory *inventory = get_inventory(0);
	return inventory != nullptr && close(inventory);
}

void InventoryHandler::close_all_inventories() {
	while(opened_inventories.size() > 0) {
		NodePath inv_path = opened_inventories[0];
		Node *node_inv = get_node_or_null(inv_path);
		Inventory *inventory = Object::cast_to<Inventory>(node_inv);
		if (inventory == nullptr || !close(inventory)) {
			opened_inventories.remove_at(0);
		}
	}
}

void InventoryHandler::to_transaction(const int &slot_index, Inventory *inventory, const int &amount) {
	if (is_transaction_active())
		return;
	Ref<Slot> slot = inventory->get_slots()[slot_index];
	Ref<Item> item = slot->get_item()->duplicate();
	if (!slot->has_valid()) {
		return;
	}
	int amount_no_removed = inventory->remove_at(slot_index, item, amount);
	_set_transaction_slot(item, amount - amount_no_removed);
}

void InventoryHandler::transaction_to_at(const int &slot_index, Inventory *inventory, const int &amount_to_move) {
	if (!is_transaction_active())
		return;
	Ref<Slot> slot = inventory->get_slots()[slot_index];
	Ref<Item> item = memnew(Item());
	item->set_definition(transaction_slot->get_item()->get_definition());
	item->set_properties(transaction_slot->get_item()->get_properties().duplicate());
	if (!transaction_slot->has_valid()) {
		return;
	}
	if (inventory->is_empty_slot(slot_index) || item->is_stack_with(slot->get_item())) {
		int amount = transaction_slot->get_amount();
		if (amount_to_move >= 0)
			amount = amount_to_move;
		int amount_no_add = inventory->add_at(slot_index, item, amount);
		_set_transaction_slot(item, (transaction_slot->get_amount() - amount) + amount_no_add);
	} else {
		// Different items in slot and other_slot
		// Check if transaction_slot amount is equal of origin_slot amount
		CategorizedSlot *c_slot = Object::cast_to<CategorizedSlot>(*slot);
		if (c_slot != nullptr) {
			if (!c_slot->is_accept_any_categories_of_item(item->get_definition())) {
				return;
			}
		}
		Ref<Item> temp_item = memnew(Item());
		temp_item->set_definition(slot->get_item()->get_definition());
		temp_item->set_properties(slot->get_item()->get_properties().duplicate());
		int new_amount = slot->get_amount();
		inventory->set_slot_content(slot_index, item->get_definition(), item->get_properties(), transaction_slot->get_amount());
		_set_transaction_slot(temp_item, new_amount);
	}
}

void InventoryHandler::transaction_to(Inventory *inventory) {
	if (!is_transaction_active())
		return;
	Ref<Item> item = transaction_slot->get_item();
	if (item == nullptr)
		return;
	int amount_no_add = inventory->add(item, transaction_slot->get_amount());
	_set_transaction_slot(item, amount_no_add);
}

bool InventoryHandler::is_transaction_active() const {
	return transaction_slot->has_valid();
}

void InventoryHandler::drop_transaction() {
	if (is_transaction_active())
		drop(transaction_slot->get_item(), transaction_slot->get_amount());
	_set_transaction_slot(nullptr, 0);
}

Inventory *InventoryHandler::get_inventory(const int &index) const {
	Node *node_inv = get_node_or_null(inventories_path[index]);
	Inventory *inventory = Object::cast_to<Inventory>(node_inv);
	if (inventory == nullptr) {
		ERR_PRINT("Get Inventory Index is null!");
	}
	return inventory;
}
