#include "inventory_handler.h"
#include "inventory.h"
#include <godot_cpp/classes/engine.hpp>

void InventoryHandler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transaction_slot"), &InventoryHandler::get_transaction_slot);
	ClassDB::bind_method(D_METHOD("change_transaction_slot", "item", "amount"), &InventoryHandler::change_transaction_slot);
	ClassDB::bind_method(D_METHOD("to_transaction", "slot_index", "inventory", "amount"), &InventoryHandler::to_transaction);
	ClassDB::bind_method(D_METHOD("transaction_to_at", "slot_index", "inventory", "amount_to_move"), &InventoryHandler::transaction_to_at, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("transaction_to", "inventory"), &InventoryHandler::transaction_to);
	ClassDB::bind_method(D_METHOD("is_transaction_active"), &InventoryHandler::is_transaction_active);
	ClassDB::bind_method(D_METHOD("drop_transaction"), &InventoryHandler::drop_transaction);
	ADD_SIGNAL(MethodInfo("updated_transaction_slot"));
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

void InventoryHandler::drop_transaction(Inventory *inventory) {
	if (is_transaction_active())
		inventory->drop(transaction_slot->get_item(), transaction_slot->get_amount());
	change_transaction_slot(nullptr, 0);
}
