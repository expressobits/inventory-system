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

void Inventory::set_slot_content(const int slot_index, const String &item_id, const int &amount, const Dictionary &properties) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= size(), "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_MSG(amount < 0, "The 'amount' is negative.");

	int old_amount = this->amount();
	Ref<Slot> slot = slots[slot_index];
	slot->set_item_id(item_id);
	slot->set_amount(amount);
	slot->set_properties(properties);
	slots[slot_index] = slot;
	emit_signal("updated_slot", slot_index);
	_call_events(old_amount);
}

void Inventory::set_slot_with_other_slot(const int slot_index, const Ref<Slot> &other_slot) {
	ERR_FAIL_NULL_MSG(other_slot, "'other_slot' is null.");

	set_slot_content(slot_index, other_slot->get_item_id(), other_slot->get_amount(), other_slot->get_properties());
}

bool Inventory::is_empty_slot(const int &slot_index) const {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), true, "The 'slot index' is out of bounds.");

	Ref<Slot> slot = slots[slot_index];
	if (slot != nullptr && !slot->is_empty())
		return false;
	return true;
}

bool Inventory::is_empty() const {
	return amount() == 0;
}

bool Inventory::is_full() const {
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (!slot->is_full())
			return false;
	}
	return true;
}

int Inventory::size() const {
	return slots.size();
}

bool Inventory::contains(const String &item_id, const int &amount) const {
	ERR_FAIL_COND_V_MSG(amount < 0, false, "'amount' is negative.");

	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->contains(item_id, 1)) {
			amount_in_inventory += slot->get_amount();
			if (amount_in_inventory >= amount) {
				return true;
			}
		}
	}
	return false;
}

bool Inventory::contains_at(const int &slot_index, const String &item_id, const int &amount) const {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), false, "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_V_MSG(amount < 0, false, "The 'amount' is negative.");
	// ERR_FAIL_NULL_V_MSG(item, false, "'item' is null.");
	// ERR_FAIL_NULL_V_MSG(item->get_definition(), false, "'item definition' is null.");

	if (slot_index < slots.size()) {
		Ref<Slot> slot = slots[slot_index];
		if (slot->contains(item_id, 1)) {
			return slot->get_amount() >= amount;
		}
	}
	return false;
}

bool Inventory::contains_category(const Ref<ItemCategory> &category, const int &amount) const {
	ERR_FAIL_NULL_V_MSG(category, false, "'category' is null.");
	ERR_FAIL_COND_V_MSG(amount < 0, false, "The 'amount' is negative.");

	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (contains_category_in_slot(slot, category)) {
			amount_in_inventory += slot->get_amount();
			if (amount_in_inventory >= amount) {
				return true;
			}
		}
	}
	return false;
}

int Inventory::get_slot_index_with_an_item_of_category(const Ref<ItemCategory> &category) const {
	ERR_FAIL_NULL_V_MSG(category, 0, "'category' is null.");

	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (contains_category_in_slot(slot, category)) {
			return i;
		}
	}
	return -1;
}

int Inventory::amount_of_item(const String &item_id) const {

	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot->contains(item_id, 1)) {
			amount_in_inventory += slot->get_amount();
		}
	}
	return amount_in_inventory;
}

int Inventory::amount_of_category(const Ref<ItemCategory> &category) const {
	ERR_FAIL_NULL_V_MSG(category, 0, "'category' is null.");

	int amount_in_inventory = 0;
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot == nullptr) {
			continue;
		}
		if (contains_category_in_slot(slot, category)) {
			amount_in_inventory += slot->get_amount();
		}
	}
	return amount_in_inventory;
}

int Inventory::amount() const {
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

int Inventory::add(const String &item_id, const int &amount, const Dictionary &properties, const bool &drop_excess) {
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	int amount_in_interact = amount;
	int old_amount = this->amount();

	for (size_t i = 0; i < slots.size(); i++) {
		int previous_amount = amount_in_interact;
		amount_in_interact = _add_to_slot(i, item_id, amount_in_interact, properties);

		// Check for potential integer underflow
		ERR_FAIL_COND_V_MSG(amount_in_interact > previous_amount, amount, "Integer underflow detected in _add_to_slot.");

		if (amount_in_interact == 0) {
			break;
		}
	}

	if (create_slot_if_needed && amount_in_interact > 0) {
		insert_slot(slots.size());
		int previous_amount = amount_in_interact;
		amount_in_interact = _add_to_slot(slots.size() - 1, item_id, amount_in_interact, properties);

		// Check for potential integer underflow
		ERR_FAIL_COND_V_MSG(amount_in_interact > previous_amount, amount, "Integer underflow detected in _add_to_slot after creating new slot.");

		_call_events(old_amount);
	}

	// Use subtraction to avoid potential overflow
	int _added = amount - amount_in_interact;

	// Sanity check
	ERR_FAIL_COND_V_MSG(_added < 0 || _added > amount, amount, "Invalid _added value calculated.");

	if (_added > 0) {
		emit_signal("item_added", item_id, _added);
	}

	if (drop_excess) {
		drop(item_id, amount_in_interact, properties);
		return 0;
	}

	return amount_in_interact;
}

int Inventory::add_at(const int &slot_index, const String &item_id, const int &amount, const Dictionary &properties) {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), amount, "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	int amount_in_interact = amount;
	int old_amount = this->amount();
	if (slot_index < slots.size()) {
		amount_in_interact = _add_to_slot(slot_index, item_id, amount_in_interact, properties);
		_call_events(old_amount);
	}
	int _added = amount - amount_in_interact;
	if (_added > 0) {
		emit_signal("item_added", item_id, _added);
	}
	return amount_in_interact;
}

int Inventory::remove(const String &item_id, const int &amount) {
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	int amount_in_interact = amount;
	int old_amount = this->amount();
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		amount_in_interact = _remove_from_slot(i, item_id, amount_in_interact);
		if (remove_slot_if_empty && slot->get_amount() == 0) {
			remove_slot_at(i);
			_call_events(old_amount);
		}
		if (amount_in_interact == 0) {
			break;
		}
	}
	int _removed = amount - amount_in_interact;
	if (_removed > 0) {
		emit_signal("item_removed", item_id, _removed);
	}
	return amount_in_interact;
}

int Inventory::remove_at(const int &slot_index, const String &item_id, const int &amount) {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), amount, "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	int amount_in_interact = amount;
	int old_amount = this->amount();
	if (slot_index < slots.size()) {
		Ref<Slot> slot = slots[slot_index];
		amount_in_interact = _remove_from_slot(slot_index, item_id, amount_in_interact);
		if (remove_slot_if_empty && slot->get_amount() == 0) {
			remove_slot_at(slot_index);
			_call_events(old_amount);
		}
	}
	int _removed = amount - amount_in_interact;
	if (_removed > 0) {
		emit_signal("item_removed", item_id, _removed);
	}
	return amount_in_interact;
}

void Inventory::transfer(const int &slot_index, Inventory *destination, const int &destination_slot_index, const int &amount) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= size(), "The 'slot index' is out of bounds.");
	ERR_FAIL_NULL_MSG(destination, "Destination inventory is null on transfer.");
	ERR_FAIL_NULL_MSG(get_database(), "InventoryDatabase is null.");
	ERR_FAIL_NULL_MSG(destination->get_database(), "InventoryDatabase is null.");
	ERR_FAIL_COND_MSG(get_database() != destination->get_database(), "Operation between inventories that do not have the same database is invalid.");
	ERR_FAIL_COND_MSG(destination_slot_index >= destination->size() || destination_slot_index < 0, "The 'destination slot index' exceeds the destination inventory size or negative value.");
	ERR_FAIL_COND_MSG(amount < 0, "The 'amount' is negative.");

	Ref<Slot> slot = get_slots()[slot_index];
	String item_id = slot->get_item_id();
	Dictionary properties = slot->get_properties();
	int amount_to_interact = amount;

	if (amount_to_interact == -1) {
		amount_to_interact = slot->get_amount();
	}
	Ref<Slot> destination_slot = destination->get_slots()[destination_slot_index];
	int amount_to_left = destination_slot->left_to_fill();
	if (amount_to_left > -1) {
		amount_to_interact = MIN(amount_to_interact, amount_to_left);
	}
	if (amount_to_interact == 0)
		return;
	int amount_not_removed = remove_at(slot_index, item_id, amount_to_interact);
	int amount_to_transfer = amount_to_interact - amount_not_removed;
	if (amount_to_transfer == 0)
		return;
	int amount_not_transferred = destination->add_at(destination_slot_index, item_id, amount_to_transfer, properties);
	if (amount_not_transferred == 0)
		return;
	add_at(slot_index, item_id, amount_not_transferred, properties);
}

void Inventory::set_slots(const TypedArray<Slot> &new_slots) {
	slots = new_slots;
}

TypedArray<Slot> Inventory::get_slots() const {
	return slots;
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

Dictionary Inventory::serialize() const {
	Dictionary data = Dictionary();
	data["slots"] = get_database()->serialize_slots(slots);;
	return data;
}

void Inventory::deserialize(const Dictionary data) {
	ERR_FAIL_COND_MSG(!data.has("slots"), "Data to deserialize is invalid: Does not contain the 'slots' field");
	Array slots_data = data["slots"];
	get_database()->deserialize_slots(slots, slots_data);
}

bool Inventory::drop(const String &item_id, const int &amount, const Dictionary &properties) {
	ERR_FAIL_NULL_V_MSG(get_database(), false, "'database' is null.");
	Ref<ItemDefinition> _definition = get_database()->get_item(item_id);
	ERR_FAIL_NULL_V_MSG(_definition, false,"'item_definition' is null.");
	if (_definition->get_properties().has("dropped_item")) {
		String path = _definition->get_properties()["dropped_item"];
		// We have i < 1000 to have some enforced upper limit preventing long loops
		for (size_t i = 0; i < amount && i < 1000; i++) {
			emit_signal("request_drop_obj", path, item_id, properties);
		}
		return true;
	}
	return false;
}

void Inventory::drop_from_inventory(const int &slot_index, const int &amount, const Dictionary &properties) {
	ERR_FAIL_COND(slot_index < 0 || slot_index >= slots.size());

	if (slots.size() <= slot_index)
		return;
	if (is_empty_slot(slot_index))
		return;
	Ref<Slot> slot = slots[slot_index];
	if (!slot->has_valid())
		return;
	String item_id = slot->get_item_id();
	int not_removed = remove_at(slot_index, item_id, amount);
	int removed = amount - not_removed;
	drop(item_id, removed, properties);
}

int Inventory::add_to_slot(Ref<Slot> slot, const String &item_id, const int &amount, const Dictionary &properties) {
	ERR_FAIL_COND_V_MSG(amount < 0, 0, "The 'amount' is negative.");

	ERR_FAIL_NULL_V_MSG(get_database(), amount, "The 'database' is null.");
	Ref<ItemDefinition> definition = get_database()->get_item(item_id);
	ERR_FAIL_NULL_V_MSG(definition, amount, "The 'definition' is null.");

	if (slot->is_categorized()) {
		int flag_category = get_flag_categories_of_slot(slot);
		if (flag_category != 0 && !is_accept_any_categories(flag_category ,definition->get_categories())) {
			return amount;
		}
	}
	if (amount <= 0 || (slot->has_valid() && (slot->get_item_id() != item_id || slot->get_properties() != properties))) {
		return amount;
	}

	int max_stack = get_max_stack_of_slot(slot, definition);
	int amount_to_add = MIN(amount, max_stack - slot->get_amount());
	if (amount_to_add > 0 && !slot->has_valid()) {
		slot->set_item_id(item_id);
		slot->set_properties(properties);
		slot->emit_signal("updated");
	}
	slot->set_amount(slot->get_amount() + amount_to_add);
	return amount - amount_to_add;
}

int Inventory::remove_from_slot(Ref<Slot> slot, const String &item_id, const int &amount) {
	if (slot->get_item_id() == "") {
		return amount;
	}
	if (amount <= 0 || (slot->has_valid() && slot->get_item_id() != item_id)) {
		return amount;
	}
	int amount_to_remove = MIN(amount, slot->get_amount());
	slot->set_amount(slot->get_amount() - amount_to_remove);
	slot->emit_signal("updated");
	if (slot->get_amount() <= 0) {
		slot->set_item_id("");
		slot->set_properties(Dictionary());
	}
	return amount - amount_to_remove;
}

int Inventory::get_flag_categories_of_slot(const Ref<Slot> slot) const{
	int accepted_categories_code = 0;
	if (!Engine::get_singleton()->is_editor_hint()) {
		for (size_t i = 0; i < slot->get_accepted_categories().size(); i++) {
			String category_id = slot->get_accepted_categories()[i];
			Ref<ItemCategory> c = get_database()->get_category_from_id(category_id);
			if (c == nullptr)
				continue;
			accepted_categories_code |= c->get_code();
		}
	}
	return accepted_categories_code;
}

bool Inventory::is_accept_any_categories(const int categories_flag, const TypedArray<ItemCategory> &other_list) const {
	for (size_t i = 0; i < other_list.size(); i++) {
		Ref<ItemCategory> c = other_list[i];
		if (c == nullptr)
			continue;
		if ((categories_flag & c->get_code()) > 0) {
			return true;
		}
	}
	return false;
}

int Inventory::get_max_stack_of_slot(const Ref<Slot> slot, Ref<ItemDefinition> item) const {
	if (slot->get_max_stack() == -1 && item != nullptr) {
		return item->get_max_stack();
	} else {
		return slot->get_max_stack();
	}
}

bool Inventory::contains_category_in_slot(const Ref<Slot> slot, Ref<ItemCategory> category) const {
	Ref<ItemDefinition> definition = get_database()->get_item(slot->get_item_id());
	if (definition == nullptr) {
		return false;
	} else {
		return definition->is_in_category(category);
	}
}

void Inventory::_load_slots() {
	Array slots = this->slots.duplicate(true);
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (slot == nullptr) {
			Ref<Slot> temp_slot = memnew(Slot());
			slot = temp_slot;
		}
		slots[i] = slot;
	}
	this->slots = slots;
}

void Inventory::remove_slot_at(int slot_index) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= size(), "The 'slot index' is out of bounds.");

	slots.remove_at(slot_index);
	this->emit_signal("slot_removed", slot_index);
}

void Inventory::add_slot() {
	Ref<Slot> slot = memnew(Slot());
	slot->set_item_id("");
	slot->set_amount(0);
	slots.append(slot);
	this->emit_signal("slot_added", slots.size());
}

void Inventory::insert_slot(int slot_index) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index > size(), "The 'slot index' is out of bounds.");

	Ref<Slot> slot = memnew(Slot());
	slot->set_item_id("");
	slot->set_amount(0);
	slots.insert(slot_index, slot);
	this->emit_signal("slot_added", slot_index);
}

void Inventory::_call_events(int old_amount) {
	int actual_amount = amount();
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

int Inventory::_add_to_slot(int slot_index, const String &item_id, int amount, const Dictionary &properties) {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), 0, "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_V_MSG(amount < 0, 0, "The 'amount' is negative.");

	Ref<Slot> slot = slots[slot_index];
	ERR_FAIL_NULL_V_MSG(slot, 0, "The 'slot' is null.");

	int _remaining_amount = add_to_slot(slot, item_id, amount, properties);

	if (_remaining_amount == amount) {
		return amount;
	}

	emit_signal("updated_slot", slot_index);
	return _remaining_amount;
}


int Inventory::_remove_from_slot(int slot_index, const String &item_id, int amount) {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= size(), amount, "The 'slot index' is out of bounds.");
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	Ref<Slot> slot = slots[slot_index];
	int _remaining_amount = remove_from_slot(slot, item_id, amount);
	if (_remaining_amount == amount) {
		return amount;
	}
	emit_signal("updated_slot", slot_index);
	return _remaining_amount;
}

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_slot"), &Inventory::add_slot);
	ClassDB::bind_method(D_METHOD("insert_slot", "slot_index"), &Inventory::insert_slot);
	ClassDB::bind_method(D_METHOD("remove_slot_at", "slot_index"), &Inventory::remove_slot_at);
	ClassDB::bind_method(D_METHOD("set_slot_content", "slot_index", "item_id", "amount", "properties"), &Inventory::set_slot_content, DEFVAL(1), DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("set_slot_with_other_slot", "slot_index", "other_slot"), &Inventory::set_slot_with_other_slot);
	ClassDB::bind_method(D_METHOD("is_empty_slot", "slot_index"), &Inventory::is_empty_slot);
	ClassDB::bind_method(D_METHOD("is_empty"), &Inventory::is_empty);
	ClassDB::bind_method(D_METHOD("is_full"), &Inventory::is_full);
	ClassDB::bind_method(D_METHOD("size"), &Inventory::size);
	ClassDB::bind_method(D_METHOD("contains", "item_id", "amount"), &Inventory::contains, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("contains_at", "slot_index", "item_id", "amount"), &Inventory::contains_at, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("contains_category", "category", "amount"), &Inventory::contains_category, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("get_slot_index_with_an_item_of_category", "category"), &Inventory::get_slot_index_with_an_item_of_category);
	ClassDB::bind_method(D_METHOD("amount_of_item", "item_id"), &Inventory::amount_of_item);
	ClassDB::bind_method(D_METHOD("get_amount_of_category", "category"), &Inventory::amount_of_category);
	ClassDB::bind_method(D_METHOD("get_amount"), &Inventory::amount);
	ClassDB::bind_method(D_METHOD("add", "item_id", "amount", "properties", "drop_excess"), &Inventory::add, DEFVAL(1), DEFVAL(Dictionary()), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("add_at", "slot_index", "item_id", "amount", "properties"), &Inventory::add_at, DEFVAL(1), DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("remove", "item_id", "amount"), &Inventory::remove, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("remove_at", "slot_index", "item_id", "amount"), &Inventory::remove_at, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("transfer", "slot_index", "destination", "destination_slot_index", "amount"), &Inventory::transfer, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("drop", "item_id", "amount", "properties"), &Inventory::drop, DEFVAL(1), DEFVAL(Dictionary()), DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("drop_from_inventory", "slot_index", "amount", "properties"), &Inventory::drop_from_inventory, DEFVAL(1), DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("add_to_slot", "slot", "item_id", "amount", "properties"), &Inventory::add_to_slot, DEFVAL(1), DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("remove_from_slot", "slot", "item_id", "amount"), &Inventory::remove_from_slot, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("get_flag_categories_of_slot", "slot"), &Inventory::get_flag_categories_of_slot);
	ClassDB::bind_method(D_METHOD("is_accept_any_categories", "categories_flag", "slot"), &Inventory::is_accept_any_categories);
	ClassDB::bind_method(D_METHOD("contains_category_in_slot", "slot", "category"), &Inventory::contains_category_in_slot);
	ClassDB::bind_method(D_METHOD("serialize"), &Inventory::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &Inventory::deserialize);

	ClassDB::bind_method(D_METHOD("set_slots", "slots"), &Inventory::set_slots);
	ClassDB::bind_method(D_METHOD("get_slots"), &Inventory::get_slots);
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
	ADD_SIGNAL(MethodInfo("item_added", PropertyInfo(Variant::STRING, "item_id"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("item_removed", PropertyInfo(Variant::STRING, "item_id"), PropertyInfo(Variant::INT, "amount")));
	ADD_SIGNAL(MethodInfo("filled"));
	ADD_SIGNAL(MethodInfo("emptied"));
	ADD_SIGNAL(MethodInfo("updated_slot", PropertyInfo(Variant::INT, "slot_index")));

	ADD_SIGNAL(MethodInfo("request_drop_obj", PropertyInfo(Variant::STRING, "drop_item_packed_scene_path"), PropertyInfo(Variant::STRING, "item_id"), PropertyInfo(Variant::DICTIONARY, "item_properties")));

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "slots", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Slot")), "set_slots", "get_slots");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "create_slot_if_needed"), "set_create_slot_if_needed", "get_create_slot_if_needed");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "remove_slot_if_empty"), "set_remove_slot_if_empty", "get_remove_slot_if_empty");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "slot_amount"), "set_slot_amount", "get_slot_amount");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "inventory_name"), "set_inventory_name", "get_inventory_name");
}

void Inventory::update_slot(const int slot_index) {
	emit_signal("updated_slot", slot_index);
	_call_events(amount());
}
