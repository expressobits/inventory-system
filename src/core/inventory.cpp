#include "inventory.h"
#include <godot_cpp/classes/engine.hpp>

Inventory::Inventory() {
}

Inventory::~Inventory() {
}

void Inventory::_enter_tree() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		_load_slots();
	}
}

void Inventory::set_slot(const int &slot_index, const Ref<Item> &item, const int &amount) {
	set_slot_content(slot_index, item->get_definition(), item->get_properties(), amount);
}

void Inventory::set_slot_content(const int slot_index, const Ref<ItemDefinition> &definition, const Dictionary &properties, const int &amount) {
	if (slot_index >= slots.size())
		return;
	int old_amount = get_amount();
	Ref<Slot> slot = slots[slot_index];
	slot->get_item()->set_definition(definition);
	slot->set_amount(amount);
	slot->get_item()->set_properties(properties);
	slots[slot_index] = slot;
	emit_signal("updated_slot", slot_index);
	_call_events(old_amount);
}

void Inventory::set_slot_with_other_slot(const int slot_index, const Ref<Slot> &other_slot) {
	if (slot_index >= slots.size())
		return;
	set_slot(slot_index, other_slot->get_item(), other_slot->get_amount());
}

bool Inventory::is_empty_slot(const int &slot_index) const {
	if (slot_index >= slots.size())
		return true;
	Ref<Slot> slot = slots[slot_index];
	if (slot != nullptr && !slot->is_empty())
		return false;
	return true;
}

bool Inventory::is_empty() const {
	return get_amount() == 0;
}

bool Inventory::is_full() const {
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (!slot->is_full())
			return false;
	}
	return true;
}

bool Inventory::contains(const Ref<Item> &item, const int &amount) const {
	if (item == nullptr || item->get_definition() == nullptr) {
		return false;
		int amount_in_inventory = 0;
		for (size_t i = 0; i < slots.size(); i++) {
			Ref<Slot> slot = slots[i];
			if (slot->contains(item, 1)) {
				amount_in_inventory += slot->get_amount();
				if (amount_in_inventory >= amount) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Inventory::contains_at(const int &slot_index, const Ref<Item> &item, const int &amount) const {
	if (item == nullptr || item->get_definition() == nullptr) {
		return false;
	}
	if (slot_index < slots.size()) {
		Ref<Slot> slot = slots[slot_index];
		if (slot->contains(item, 1)) {
			return slot->get_amount() >= amount;
		}
	}
	return false;
}

bool Inventory::contains_category(const Ref<ItemCategory> &category, const int &amount) const {
	if (category == nullptr) {
		return false;
	}
	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->contains_category(category)) {
			amount_in_inventory += slot->get_amount();
			if (amount_in_inventory >= amount) {
				return true;
			}
		}
	}
	return false;
}

int Inventory::get_slot_index_with_an_item_of_category(const Ref<ItemCategory> &category) const {
	if (category == nullptr) {
		return false;
	}
	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->contains_category(category)) {
			return i;
		}
	}
	return -1;
}

int Inventory::get_amount_of(const Ref<Item> &item) const {
	if (item == nullptr) {
		return 0;
	}
	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->contains(item, 1)) {
			amount_in_inventory += slot->get_amount();
		}
	}
	return amount_in_inventory;
}

int Inventory::get_amount_of_category(const Ref<ItemCategory> &category) const {
	if (category == nullptr) {
		return 0;
	}
	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot == nullptr) {
			continue;
		}
		if (slot->contains_category(category)) {
			amount_in_inventory += slot->get_amount();
		}
	}
	return amount_in_inventory;
}

int Inventory::get_amount() const {
	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot == nullptr) {
			continue;
		}
		amount_in_inventory += slot->get_amount();
	}
	return amount_in_inventory;
}

int Inventory::add(const Ref<Item> &item, const int &amount) {
	int amount_in_interact = amount;
	int old_amount = get_amount();
	for (size_t i = 0; i < slots.size(); i++) {
		amount_in_interact = _add_to_slot(i, item, amount_in_interact);
	}
	if (create_slot_if_needed && amount_in_interact > 0) {
		add_slot(slots.size());
		amount_in_interact = _add_to_slot(slots.size() - 1, item, amount_in_interact);
		_call_events(old_amount);
	}
	int _added = amount - amount_in_interact;
	if (_added > 0) {
		emit_signal("item_added", item, _added);
	}
	return amount_in_interact;
}

int Inventory::add_at(const int &slot_index, const Ref<Item> &item, const int &amount) {
	int amount_in_interact = amount;
	int old_amount = get_amount();
	if (slot_index < slots.size()) {
		amount_in_interact = _add_to_slot(slot_index, item, amount_in_interact);
		_call_events(old_amount);
	}
	int _added = amount - amount_in_interact;
	if (_added > 0) {
		emit_signal("item_added", item, _added);
	}
	return amount_in_interact;
}

int Inventory::remove(const Ref<Item> &item, const int &amount) {
	int amount_in_interact = amount;
	int old_amount = get_amount();
	Ref<ItemDefinition> _definition = item->get_definition();
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		amount_in_interact = _remove_from_slot(i, item, amount_in_interact);
		if (remove_slot_if_empty && slot->get_amount() == 0) {
			remove_slot(i);
			_call_events(old_amount);
		}
	}
	int _removed = amount - amount_in_interact;
	if (_removed > 0) {
		emit_signal("item_removed", _definition, _removed);
	}
	return amount_in_interact;
}

int Inventory::remove_at(const int &slot_index, const Ref<Item> &item, const int &amount) {
	int amount_in_interact = amount;
	int old_amount = get_amount();
	Ref<ItemDefinition> _definition = item->get_definition();
	if (slot_index < slots.size()) {
		Ref<Slot> slot = slots[slot_index];
		amount_in_interact = _remove_from_slot(slot_index, item, amount_in_interact);
		if (remove_slot_if_empty && slot->get_amount() == 0) {
			remove_slot(slot_index);
			_call_events(old_amount);
		}
	}
	int _removed = amount - amount_in_interact;
	if (_removed > 0) {
		emit_signal("item_removed", _definition, _removed);
	}
	return amount_in_interact;
}

bool Inventory::open() {
	if (!is_open) {
		is_open = true;
		emit_signal("opened");
		return true;
	}
	return false;
}

bool Inventory::close() {
	if (is_open) {
		is_open = false;
		emit_signal("closed");
		return true;
	}
	return false;
}

void Inventory::set_slots(const TypedArray<Slot> &new_slots) {
	slots = new_slots;
}

TypedArray<Slot> Inventory::get_slots() const {
	return slots;
}

void Inventory::set_is_open(const bool &new_is_open) {
	is_open = new_is_open;
}

bool Inventory::get_is_open() const {
	return is_open;
}

void Inventory::set_create_slot_if_needed(const bool &new_create_slot_if_needed) {
	create_slot_if_needed = new_create_slot_if_needed;
}

bool Inventory::get_create_slot_if_needed() const {
	return create_slot_if_needed;
}

void Inventory::set_remove_slot_if_empty(const bool &new_remove_slot_if_empty) {
	remove_slot_if_empty = new_remove_slot_if_empty;
}

bool Inventory::get_remove_slot_if_empty() const {
	return remove_slot_if_empty;
}

void Inventory::set_slot_amount(const int &new_slot_amount) {
	slot_amount = new_slot_amount;
}

int Inventory::get_slot_amount() const {
	return slot_amount;
}

void Inventory::set_inventory_name(const String &new_inventory_name) {
	inventory_name = new_inventory_name;
}

String Inventory::get_inventory_name() const {
	return inventory_name;
}

void Inventory::_load_slots() {
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot == nullptr) {
			Ref<Slot> temp_slot = memnew(Slot());
			slot = temp_slot;
		}
		slots[i] = slot;
	}
	TypedArray<Slot> temp_slots = slots.duplicate(true);
	slots.clear();
	for (size_t i = 0; i < temp_slots.size(); i++) {
		slots.append(temp_slots[i].duplicate());
	}
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->get_item() == nullptr) {
			slot->set_item(memnew(Item()));
		}
		slots[i] = slot;
	}
}

void Inventory::remove_slot(int slot_index, bool emit_signal) {
	slots.remove_at(slot_index);
	if (emit_signal) {
		this->emit_signal("slot_removed", slot_index);
	}
}

void Inventory::add_slot(int slot_index, bool emit_signal) {
	Ref<Slot> slot = memnew(Slot());
	slot->set_item(memnew(Item()));
	slot->set_amount(0);
	slots.insert(slot_index, slot);
	if (emit_signal) {
		this->emit_signal("slot_added", slot_index);
	}
}

void Inventory::_call_events(int old_amount) {
	int actual_amount = get_amount();
	if (old_amount != actual_amount) {
		emit_signal("inventory_changed");
		if (is_empty()) {
			emit_signal("emptied");
		}
		if (is_full()) {
			emit_signal("filled");
		}
	}
}

int Inventory::_add_to_slot(int slot_index, const Ref<Item> &item, int amount) {
	Ref<Slot> slot = slots[slot_index];
	int _remaining_amount = slot->add(item, amount);
	if (_remaining_amount == amount) {
		return amount;
	}
	emit_signal("updated_slot", slot_index);
	return _remaining_amount;
}

int Inventory::_remove_from_slot(int slot_index, const Ref<Item> &item, int amount) {
	Ref<Slot> slot = slots[slot_index];
	int _remaining_amount = slot->remove(item, amount);
	if (_remaining_amount == amount) {
		return amount;
	}
	emit_signal("updated_slot", slot_index);
	return _remaining_amount;
}

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_slot", "slot_index", "emit_signal"), &Inventory::add_slot, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("remove_slot", "slot_index", "emit_signal"), &Inventory::remove_slot, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("set_slot", "slot_index", "item", "amount"), &Inventory::set_slot);
	ClassDB::bind_method(D_METHOD("set_slot_content", "slot_index", "item", "properties", "amount"), &Inventory::set_slot_content);
	ClassDB::bind_method(D_METHOD("set_slot_with_other_slot", "slot_index", "other_slot"), &Inventory::set_slot_with_other_slot);
	ClassDB::bind_method(D_METHOD("is_empty_slot", "slot_index"), &Inventory::is_empty_slot);
	ClassDB::bind_method(D_METHOD("is_empty"), &Inventory::is_empty);
	ClassDB::bind_method(D_METHOD("is_full"), &Inventory::is_full);
	ClassDB::bind_method(D_METHOD("contains", "item", "amount"), &Inventory::contains, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("contains_at", "slot_index", "item", "amount"), &Inventory::contains_at, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("contains_category", "category", "amount"), &Inventory::contains_category, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("get_slot_index_with_an_item_of_category", "category"), &Inventory::get_slot_index_with_an_item_of_category);
	ClassDB::bind_method(D_METHOD("get_amount_of", "item"), &Inventory::get_amount_of);
	ClassDB::bind_method(D_METHOD("get_amount_of_category", "category"), &Inventory::get_amount_of_category);
	ClassDB::bind_method(D_METHOD("get_amount"), &Inventory::get_amount);
	ClassDB::bind_method(D_METHOD("add", "item", "amount"), &Inventory::add, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("add_at", "slot_index", "item", "amount"), &Inventory::add_at, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("remove", "item", "amount"), &Inventory::remove, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("remove_at", "slot_index", "item", "amount"), &Inventory::remove_at, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("open"), &Inventory::open);
	ClassDB::bind_method(D_METHOD("close"), &Inventory::close);

	ClassDB::bind_method(D_METHOD("set_slots", "slots"), &Inventory::set_slots);
	ClassDB::bind_method(D_METHOD("get_slots"), &Inventory::get_slots);
	ClassDB::bind_method(D_METHOD("set_is_open", "is_open"), &Inventory::set_is_open);
	ClassDB::bind_method(D_METHOD("get_is_open"), &Inventory::get_is_open);
	ClassDB::bind_method(D_METHOD("set_create_slot_if_needed", "create_slot_if_needed"), &Inventory::set_create_slot_if_needed);
	ClassDB::bind_method(D_METHOD("get_create_slot_if_needed"), &Inventory::get_create_slot_if_needed);
	ClassDB::bind_method(D_METHOD("set_remove_slot_if_empty", "remove_slot_if_empty"), &Inventory::set_remove_slot_if_empty);
	ClassDB::bind_method(D_METHOD("get_remove_slot_if_empty"), &Inventory::get_remove_slot_if_empty);
	ClassDB::bind_method(D_METHOD("set_slot_amount", "slot_amount"), &Inventory::set_slot_amount);
	ClassDB::bind_method(D_METHOD("get_slot_amount"), &Inventory::get_slot_amount);
	ClassDB::bind_method(D_METHOD("set_inventory_name", "inventory_name"), &Inventory::set_inventory_name);
	ClassDB::bind_method(D_METHOD("get_inventory_name"), &Inventory::get_inventory_name);
	ClassDB::bind_method(D_METHOD("update_slot", "slot_index"), &Inventory::update_slot);
	ADD_SIGNAL(MethodInfo("inventory_changed"));
	ADD_SIGNAL(MethodInfo("slot_added", PropertyInfo(Variant::INT, "slot_index")));
	ADD_SIGNAL(MethodInfo("slot_removed", PropertyInfo(Variant::INT, "slot_index")));
	ADD_SIGNAL(MethodInfo("item_added", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "Item"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("item_removed", PropertyInfo(Variant::OBJECT, "item_definition", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("filled"));
	ADD_SIGNAL(MethodInfo("emptied"));
	ADD_SIGNAL(MethodInfo("updated_slot", PropertyInfo(Variant::INT, "slot_index")));
	ADD_SIGNAL(MethodInfo("opened"));
	ADD_SIGNAL(MethodInfo("closed"));

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "slots", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Slot")), "set_slots", "get_slots");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_open"), "set_is_open", "get_is_open");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "create_slot_if_needed"), "set_create_slot_if_needed", "get_create_slot_if_needed");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "remove_slot_if_empty"), "set_remove_slot_if_empty", "get_remove_slot_if_empty");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "slot_amount"), "set_slot_amount", "get_slot_amount");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "inventory_name"), "set_inventory_name", "get_inventory_name");
}

void Inventory::update_slot(const int slot_index) {
	emit_signal("updated_slot", slot_index);
	_call_events(get_amount());
}
