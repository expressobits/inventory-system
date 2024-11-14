#include "hotbar.h"
#include <godot_cpp/variant/utility_functions.hpp>

void Hotbar::_on_contents_changed() {
	for (size_t i = 0; i < equipped_stacks.size(); i++) {
		Ref<ItemStack> stack = equipped_stacks[i];
		if (stack == nullptr)
			continue;
		Inventory *inventory = get_inventory();
		int stack_index = inventory->get_stacks().find(stack);
		if (stack_index == -1) {
			equipped_stacks[i] = nullptr;
			emit_signal("equipped_stack_changed", i);
		}
	}
}

void Hotbar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_path", "inventory"), &Hotbar::set_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory_path"), &Hotbar::get_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory"), &Hotbar::get_inventory);
	ClassDB::bind_method(D_METHOD("set_slots_count", "slots_count"), &Hotbar::set_slots_count);
	ClassDB::bind_method(D_METHOD("get_slots_count"), &Hotbar::get_slots_count);
	ClassDB::bind_method(D_METHOD("set_selection_index", "selection_index"), &Hotbar::set_selection_index);
	ClassDB::bind_method(D_METHOD("get_selection_index"), &Hotbar::get_selection_index);
	ClassDB::bind_method(D_METHOD("equip", "stack", "slot_index"), &Hotbar::equip);
	ClassDB::bind_method(D_METHOD("next_item"), &Hotbar::next_item);
	ClassDB::bind_method(D_METHOD("previous_item"), &Hotbar::previous_item);
	ClassDB::bind_method(D_METHOD("has_valid_item_id"), &Hotbar::has_valid_item_id);
	ClassDB::bind_method(D_METHOD("has_item_on_selection"), &Hotbar::has_item_on_selection);
	ClassDB::bind_method(D_METHOD("get_selected_item_id"), &Hotbar::get_selected_item_id);
	ClassDB::bind_method(D_METHOD("get_selected_stack"), &Hotbar::get_selected_stack);

	ADD_SIGNAL(MethodInfo("on_change_selection", PropertyInfo(Variant::INT, "selection_index")));
	ADD_SIGNAL(MethodInfo("equipped_stack_changed", PropertyInfo(Variant::INT, "slot_index")));

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "inventory", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory"), "set_inventory_path", "get_inventory_path");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "slots_count"), "set_slots_count", "get_slots_count");
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

void Hotbar::set_slots_count(const int &new_slots_count) {
	slots_count = new_slots_count;
	if (slots_count <= 0)
		slots_count = 1;

	for (size_t i = equipped_stacks.size(); i < slots_count; i++) {
		equipped_stacks.append(nullptr);
	}
	for (size_t i = equipped_stacks.size() - 1; i >= slots_count; i--) {
		equipped_stacks.remove_at(i);
	}
}

int Hotbar::get_slots_count() const {
	return slots_count;
}

void Hotbar::set_selection_index(const int &new_selection_index) {
	int old_selection = selection_index;
	selection_index = new_selection_index;
	if (selection_index >= slots_count) {
		selection_index -= slots_count;
	}
	if (selection_index < 0) {
		selection_index += slots_count;
	}
	if (old_selection != selection_index && is_node_ready()) {
		emit_signal("on_change_selection", selection_index);
	}
}

int Hotbar::get_selection_index() const {
	return selection_index;
}

void Hotbar::equip(Ref<ItemStack> stack, int slot_index) {
	equipped_stacks[slot_index] = stack;
	emit_signal("equipped_stack_changed", slot_index);
}

void Hotbar::unequip(Ref<ItemStack> stack) {
	int slot_index = equipped_stacks.find(stack);
	if (slot_index == -1)
		return;
	equipped_stacks[slot_index] = nullptr;
}

void Hotbar::next_item() {
	set_selection_index(selection_index + 1);
}

void Hotbar::previous_item() {
	set_selection_index(selection_index - 1);
}

bool Hotbar::has_valid_item_id() const {
	Inventory *inventory = get_inventory();
	Ref<ItemStack> stack = equipped_stacks[selection_index];
	int stack_index = inventory->get_stacks().find(stack);
	if (stack_index == -1) {
		return false;
	} else {
		stack = inventory->get_stacks()[stack_index];
	}

	if (stack == nullptr) {
		return false;
	}
	return stack->has_valid();
}

bool Hotbar::has_item_on_selection() const {
	return has_valid_item_id();
}

String Hotbar::get_selected_item_id() const {
	Ref<ItemStack> stack = get_selected_stack();
	if (stack == nullptr) {
		return "";
	}
	return stack->get_item_id();
}

Ref<ItemStack> Hotbar::get_selected_stack() const {
	Ref<ItemStack> stack = equipped_stacks[selection_index];
	return stack;
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
		inventory->connect("contents_changed", callable_mp(this, &Hotbar::_on_contents_changed));
	}
	NodeInventories::_ready();
}
