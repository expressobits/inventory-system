#include "hotbar.h"

void Hotbar::_on_updated_slot(const int slot_index) {
	if (slot_index == selection_index) {
		emit_signal("on_update_selection_slot");
	}
}

void Hotbar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_path", "inventory"), &Hotbar::set_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory_path"), &Hotbar::get_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory"), &Hotbar::get_inventory);
	ClassDB::bind_method(D_METHOD("set_slots_in_hot_bar", "slots_in_hot_bar"), &Hotbar::set_slots_in_hot_bar);
	ClassDB::bind_method(D_METHOD("get_slots_in_hot_bar"), &Hotbar::get_slots_in_hot_bar);
	ClassDB::bind_method(D_METHOD("set_selection_index", "selection_index"), &Hotbar::set_selection_index);
	ClassDB::bind_method(D_METHOD("get_selection_index"), &Hotbar::get_selection_index);
	ClassDB::bind_method(D_METHOD("change_selection", "index"), &Hotbar::change_selection);
	ClassDB::bind_method(D_METHOD("next_item"), &Hotbar::next_item);
	ClassDB::bind_method(D_METHOD("previous_item"), &Hotbar::previous_item);
	ClassDB::bind_method(D_METHOD("has_valid_item_id"), &Hotbar::has_valid_item_id);
	ClassDB::bind_method(D_METHOD("has_item_on_selection"), &Hotbar::has_item_on_selection);
	ClassDB::bind_method(D_METHOD("get_selected_item"), &Hotbar::get_selected_item);

	ADD_SIGNAL(MethodInfo("on_change_selection", PropertyInfo(Variant::INT, "selection_index")));
	ADD_SIGNAL(MethodInfo("on_update_selection_slot"));

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "inventory", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory"), "set_inventory_path", "get_inventory_path");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "slots_in_hot_bar"), "set_slots_in_hot_bar", "get_slots_in_hot_bar");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "selection_index"), "set_selection_index", "get_selection_index");
}

void Hotbar::set_inventory_path(const NodePath &new_inventory) {
	inventory_path = new_inventory;
}

NodePath Hotbar::get_inventory_path() const {
	return inventory_path;
}

Inventory *Hotbar::get_inventory() const {
	Node *node_inv = get_node_or_null(inventory_path);
	Inventory *inventory = Object::cast_to<Inventory>(node_inv);
	if (node_inv == nullptr) {
		ERR_PRINT("Missing setup inventory node on hotbar.");
		return nullptr;
	}
	if (!inventory) {
		ERR_PRINT("Inventory param must be 'Inventory' Node");
		return nullptr;
	}
	return inventory;
}

void Hotbar::set_slots_in_hot_bar(const int &new_slots_in_hot_bar) {
	slots_in_hot_bar = new_slots_in_hot_bar;
}

int Hotbar::get_slots_in_hot_bar() const {
	return slots_in_hot_bar;
}

void Hotbar::set_selection_index(const int &new_selection_index) {
	selection_index = new_selection_index;
}

int Hotbar::get_selection_index() const {
	return selection_index;
}

void Hotbar::change_selection(const int &new_index) {
	selection_index = new_index;
	if (selection_index >= slots_in_hot_bar) {
		selection_index -= slots_in_hot_bar;
	}
	if (selection_index < 0) {
		selection_index += slots_in_hot_bar;
	}
	emit_signal("on_change_selection", selection_index);
}

void Hotbar::next_item() {
	change_selection(selection_index + 1);
}

void Hotbar::previous_item() {
	change_selection(selection_index - 1);
}

bool Hotbar::has_valid_item_id() const {
	Inventory *inventory = get_inventory();
	if (selection_index >= inventory->get_slots().size()) {
		return false;
	}
	Ref<Slot> slot = inventory->get_slots()[selection_index];
	if (slot == nullptr) {
		return false;
	}
	return slot->has_valid();
}

bool Hotbar::has_item_on_selection() const {
	return has_valid_item_id();
}

Ref<Item> Hotbar::get_selected_item() const {
	Inventory *inventory = get_inventory();
	if (!has_valid_item_id()) {
		return nullptr;
	}
	Ref<Slot> slot = inventory->get_slots()[selection_index];
	return slot->get_item();
}

Hotbar::Hotbar() {
}

Hotbar::~Hotbar() {
}

void Hotbar::_ready() {
	Node *node_inv = get_node_or_null(inventory_path);
	Inventory *inventory = Object::cast_to<Inventory>(node_inv);
	if (node_inv == nullptr) {
		ERR_PRINT("Missing setup inventory node on hotbar.");
		return;
	}
	if (!inventory) {
		ERR_PRINT("Inventory param must be 'Inventory' Node");
		return;
	}
	if (inventory != nullptr) {
		inventory->connect("updated_slot", callable_mp(this, &Hotbar::_on_updated_slot));
	}
	NodeInventories::_ready();
}
