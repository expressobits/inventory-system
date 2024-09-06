#include "inventory_handler.h"
#include "inventory.h"
#include <godot_cpp/classes/engine.hpp>

void InventoryHandler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventories_path", "inventories_path"), &InventoryHandler::set_inventories_path);
	ClassDB::bind_method(D_METHOD("get_inventories_path"), &InventoryHandler::get_inventories_path);
	// ClassDB::bind_method(D_METHOD("set_transaction_slot", "transaction_slot"), &InventoryHandler::set_transaction_slot);
	ClassDB::bind_method(D_METHOD("get_transaction_slot"), &InventoryHandler::get_transaction_slot);
	ClassDB::bind_method(D_METHOD("change_transaction_slot", "item", "amount"), &InventoryHandler::change_transaction_slot);
	ClassDB::bind_method(D_METHOD("drop", "item", "amount"), &InventoryHandler::drop, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("add_to_inventory", "inventory", "item", "amount", "drop_excess"), &InventoryHandler::add_to_inventory, DEFVAL(1), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("drop_from_inventory", "slot_index", "amount", "inventory"), &InventoryHandler::drop_from_inventory, DEFVAL(1), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("pick_to_inventory", "dropped_item", "inventory"), &InventoryHandler::pick_to_inventory, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("swap_between_inventories", "inventory", "slot_index", "other_inventory", "other_slot_index", "amount"), &InventoryHandler::swap_between_inventories, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("to_transaction", "slot_index", "inventory", "amount"), &InventoryHandler::to_transaction);
	ClassDB::bind_method(D_METHOD("transaction_to_at", "slot_index", "inventory", "amount_to_move"), &InventoryHandler::transaction_to_at, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("transaction_to", "inventory"), &InventoryHandler::transaction_to);
	ClassDB::bind_method(D_METHOD("is_transaction_active"), &InventoryHandler::is_transaction_active);
	ClassDB::bind_method(D_METHOD("drop_transaction"), &InventoryHandler::drop_transaction);
	ClassDB::bind_method(D_METHOD("get_inventory", "index"), &InventoryHandler::get_inventory);
	ADD_SIGNAL(MethodInfo("dropped", PropertyInfo(Variant::OBJECT, "dropped_item")));
	ADD_SIGNAL(MethodInfo("picked", PropertyInfo(Variant::OBJECT, "dropped_item")));
	ADD_SIGNAL(MethodInfo("added", PropertyInfo(Variant::OBJECT, "item_definition", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("request_drop_obj", PropertyInfo(Variant::STRING, "dropp_item_packed_scene_path"), PropertyInfo(Variant::OBJECT, "item")));
	ADD_SIGNAL(MethodInfo("updated_transaction_slot"));
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "inventories_path", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory")), "set_inventories_path", "get_inventories_path");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "transaction_slot", PROPERTY_HINT_RESOURCE_TYPE, "Slot"), "", "get_transaction_slot");
}

InventoryHandler::InventoryHandler() {
}

InventoryHandler::~InventoryHandler() {
}

void InventoryHandler::_ready() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		if (transaction_slot.is_null()) {
			set_transaction_slot(Ref<Slot>(memnew(Slot())));
		}
	}
	NodeInventories::_ready();
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

void InventoryHandler::change_transaction_slot(const Ref<Item> &item, const int &amount) {
	ERR_FAIL_COND(transaction_slot.is_null());
	transaction_slot->set_amount(amount);
	if (amount > 0 && item.is_valid()) {
		transaction_slot->set_item(item);
	} else {
		transaction_slot->set_item(Ref<Item>());
	}
	emit_signal("updated_transaction_slot");
}

bool InventoryHandler::drop(const Ref<Item> &item, const int &amount) {
	ERR_FAIL_COND_V(item.is_null(), false);
	ERR_FAIL_COND_V(item->get_definition().is_null(), false);
	if (item->get_definition()->get_properties().has("dropped_item")) {
		String path = item->get_definition()->get_properties()["dropped_item"];
		// We have i < 1000 to have some enforced upper limit preventing long loops
		for (size_t i = 0; i < amount && i < 1000; i++) {
			emit_signal("request_drop_obj", path, item);
		}
		return true;
	}
	return false;
}

int InventoryHandler::add_to_inventory(Inventory *inventory, const Ref<Item> item, const int &amount, bool drop_excess) {
	ERR_FAIL_COND_V(inventory == nullptr, amount);
	ERR_FAIL_COND_V(item.is_null(), amount);

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
	ERR_FAIL_COND(inventory == nullptr);
	ERR_FAIL_COND(slot_index < 0 || slot_index >= inventory->get_slots().size());
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
	ERR_FAIL_NULL_V(dropped_item, false);

	if (inventory == nullptr) {
		inventory = get_inventory(0);
	}
	ERR_FAIL_NULL_V(inventory, false);

	if (!dropped_item->get("is_pickable")) {
		return false;
	}
	Variant item_variant = dropped_item->get("item");
	ERR_FAIL_COND_V(item_variant.get_type() != Variant::OBJECT, false);

	Ref<Item> item = item_variant;
	ERR_FAIL_COND_V(item.is_null(), false);
	ERR_FAIL_COND_V(item->get_definition().is_null(), false);

	if (add_to_inventory(inventory, item) == 0) {
		emit_signal("picked", dropped_item);
		dropped_item->queue_free();
		return true;
	}
	ERR_PRINT("pick_to_inventory return false");
	return false;
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

void InventoryHandler::to_transaction(const int &slot_index, Inventory *inventory, const int &amount) {
	if (is_transaction_active())
		return;
	Ref<Slot> slot = inventory->get_slots()[slot_index];
	Ref<Item> item = slot->get_item()->duplicate();
	if (!slot->has_valid()) {
		return;
	}
	int amount_no_removed = inventory->remove_at(slot_index, item, amount);
	change_transaction_slot(item, amount - amount_no_removed);
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
		change_transaction_slot(item, (transaction_slot->get_amount() - amount) + amount_no_add);
	} else {
		// Different items in slot and other_slot
		// Check if transaction_slot amount is equal of origin_slot amount
		if (slot->is_categorized()) {
			if (!slot->is_accept_any_categories_of_item(item->get_definition())) {
				return;
			}
		}
		Ref<Item> temp_item = memnew(Item());
		temp_item->set_definition(slot->get_item()->get_definition());
		temp_item->set_properties(slot->get_item()->get_properties().duplicate());
		int new_amount = slot->get_amount();
		inventory->set_slot_content(slot_index, item->get_definition(), item->get_properties(), transaction_slot->get_amount());
		change_transaction_slot(temp_item, new_amount);
	}
}

void InventoryHandler::transaction_to(Inventory *inventory) {
	if (!is_transaction_active())
		return;
	Ref<Item> item = transaction_slot->get_item();
	if (item == nullptr)
		return;
	int amount_no_add = inventory->add(item, transaction_slot->get_amount());
	change_transaction_slot(item, amount_no_add);
}

bool InventoryHandler::is_transaction_active() const {
	ERR_FAIL_NULL_V_MSG(transaction_slot, false, "transaction_slot is null in is_transaction_active() method on InventoryHandler!");
	return transaction_slot->has_valid();
}

void InventoryHandler::drop_transaction() {
	if (is_transaction_active())
		drop(transaction_slot->get_item(), transaction_slot->get_amount());
	change_transaction_slot(nullptr, 0);
}

Inventory *InventoryHandler::get_inventory(const int &index) const {
	Inventory *inventory = get_node<Inventory>(inventories_path[index]);
	if (inventory == nullptr) {
		ERR_PRINT("Get Inventory Index is null!");
	}
	return inventory;
}