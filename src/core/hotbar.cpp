#include "hotbar.h"
#include "grid_inventory.h"
#include <godot_cpp/variant/utility_functions.hpp>

void Hotbar::_on_contents_changed() {
	for (size_t i = 0; i < slots.size(); i++) {
		Ref<Slot> slot = slots[i];
		if (!slot->is_active())
			continue;

		Ref<ItemStack> stack = slot->get_stack();
		if (stack == nullptr)
			continue;
		Inventory *inventory = get_inventory();
		int stack_index = inventory->get_stacks().find(stack);
		if (stack_index == -1) {
			slot->clear();
			emit_signal("equipped_stack_changed", ((int)i));
			unequip(i);
		}
	}
	
	// Perform auto-equip if enabled
	if (auto_equip_mode != AUTO_EQUIP_DISABLED) {
		_perform_auto_equip();
	}
}

void Hotbar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_path", "inventory"), &Hotbar::set_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory_path"), &Hotbar::get_inventory_path);
	ClassDB::bind_method(D_METHOD("get_inventory"), &Hotbar::get_inventory);
	ClassDB::bind_method(D_METHOD("set_max_slots", "max_slots"), &Hotbar::set_max_slots);
	ClassDB::bind_method(D_METHOD("get_max_slots"), &Hotbar::get_max_slots);
	ClassDB::bind_method(D_METHOD("set_selection_index", "selection_index"), &Hotbar::set_selection_index);
	ClassDB::bind_method(D_METHOD("get_selection_index"), &Hotbar::get_selection_index);
	ClassDB::bind_method(D_METHOD("set_auto_equip_mode", "auto_equip_mode"), &Hotbar::set_auto_equip_mode);
	ClassDB::bind_method(D_METHOD("get_auto_equip_mode"), &Hotbar::get_auto_equip_mode);

	ClassDB::bind_method(D_METHOD("active_slot", "slot_index"), &Hotbar::active_slot);
	ClassDB::bind_method(D_METHOD("deactive_slot", "slot_index"), &Hotbar::deactive_slot);
	ClassDB::bind_method(D_METHOD("is_active_slot", "slot_index"), &Hotbar::is_active_slot);
	ClassDB::bind_method(D_METHOD("equip", "stack", "slot_index"), &Hotbar::equip);
	ClassDB::bind_method(D_METHOD("unequip", "slot_index"), &Hotbar::unequip);
	ClassDB::bind_method(D_METHOD("next_item"), &Hotbar::next_item);
	ClassDB::bind_method(D_METHOD("previous_item"), &Hotbar::previous_item);
	ClassDB::bind_method(D_METHOD("has_valid_stack_on_slot", "slot_index"), &Hotbar::has_valid_stack_on_slot);
	ClassDB::bind_method(D_METHOD("has_valid_stack_on_selection"), &Hotbar::has_valid_stack_on_selection);
	ClassDB::bind_method(D_METHOD("get_stack_on_slot", "slot_index"), &Hotbar::get_stack_on_slot);
	ClassDB::bind_method(D_METHOD("get_stack_on_selection"), &Hotbar::get_stack_on_selection);

	BIND_ENUM_CONSTANT(AUTO_EQUIP_DISABLED);
	BIND_ENUM_CONSTANT(AUTO_EQUIP_BY_STACK_ORDER);
	BIND_ENUM_CONSTANT(AUTO_EQUIP_BY_GRID_POSITION);

	ADD_SIGNAL(MethodInfo("on_change_selection", PropertyInfo(Variant::INT, "selection_index")));
	ADD_SIGNAL(MethodInfo("equipped_stack_changed", PropertyInfo(Variant::INT, "slot_index")));
	ADD_SIGNAL(MethodInfo("equipped", PropertyInfo(Variant::INT, "slot_index")));
	ADD_SIGNAL(MethodInfo("unequipped", PropertyInfo(Variant::INT, "slot_index")));

	// ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "equipped_stacks", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "ItemStack"), PROPERTY_USAGE_NONE), "set_equipped_stacks", "get_equipped_stacks");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "inventory", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory"), "set_inventory_path", "get_inventory_path");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_slots"), "set_max_slots", "get_max_slots");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "selection_index"), "set_selection_index", "get_selection_index");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "auto_equip_mode", PROPERTY_HINT_ENUM, "Disabled,By Stack Order,By Grid Position"), "set_auto_equip_mode", "get_auto_equip_mode");
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

void Hotbar::set_max_slots(const int &new_slots_count) {
	max_slots = new_slots_count;
	if (max_slots <= 0)
		max_slots = 1;

	for (size_t i = slots.size(); i < max_slots; i++) {
		slots.append(memnew(Slot));
	}
	for (size_t i = slots.size() - 1; i >= max_slots; i--) {
		slots.remove_at(i);
	}
}

int Hotbar::get_max_slots() const {
	return max_slots;
}

void Hotbar::set_selection_index(const int &new_selection_index) {
	int old_selection = selection_index;
	selection_index = new_selection_index;
	if (slots.size() == 0) {
		selection_index = -1;
	} else {
		selection_index = CLAMP(selection_index, 0, max_slots - 1);
	}
	if (old_selection != selection_index && is_node_ready()) {
		emit_signal("on_change_selection", selection_index);
	}
}

int Hotbar::get_selection_index() const {
	return selection_index;
}

void Hotbar::set_slots(const TypedArray<Hotbar::Slot> new_slots) {
	slots = new_slots;
}

TypedArray<Hotbar::Slot> Hotbar::get_slots() const {
	return TypedArray<Slot>();
}

void Hotbar::active_slot(int slot_index) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= slots.size(), "slot_index' is out of size slots bounds.");
	Ref<Slot> slot = slots[slot_index];
	slot->set_active(true);
}

void Hotbar::deactive_slot(int slot_index) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= slots.size(), "slot_index' is out of size slots bounds.");
	Ref<Slot> slot = slots[slot_index];
	slot->set_active(false);
}

bool Hotbar::is_active_slot(int slot_index) const {
	ERR_FAIL_COND_V_MSG(slot_index < 0 || slot_index >= slots.size(), false, "slot_index' is out of size slots bounds.");
	Ref<Slot> slot = slots[slot_index];
	return slot->is_active();
}

void Hotbar::equip(Ref<ItemStack> stack, int slot_index) {
	ERR_FAIL_COND_MSG(slot_index < 0 || slot_index >= slots.size(), "slot_index' is out of size slots bounds.");
	Ref<Slot> slot = slots[slot_index];
	ERR_FAIL_COND_MSG(slot.is_null(), "Slot is null, can't equip item.");
	if (!slot->is_active())
		return;

	slot->set_stack(stack);
	emit_signal("equipped", slot_index);
}

void Hotbar::unequip(const int slot_index) {
	Ref<Slot> slot = slots[slot_index];

	slot->clear();
	emit_signal("unequipped", slot_index);
}

void Hotbar::next_item() {
	int new_selection = selection_index + 1;
	if (new_selection >= max_slots) {
		new_selection -= max_slots;
	}
	if (new_selection < 0) {
		new_selection += max_slots;
	}
	set_selection_index(new_selection);
	if (!is_active_slot(new_selection))
		next_item();
}

void Hotbar::previous_item() {
	int new_selection = selection_index - 1;
	if (new_selection >= max_slots) {
		new_selection -= max_slots;
	}
	if (new_selection < 0) {
		new_selection += max_slots;
	}
	set_selection_index(new_selection);
	if (!is_active_slot(new_selection))
		previous_item();
}

bool Hotbar::has_valid_stack_on_selection() const {
	return has_valid_stack_on_slot(selection_index);
}

bool Hotbar::has_valid_stack_on_slot(const int slot_index) const {
	Inventory *inventory = get_inventory();
	if (slot_index < 0 || slot_index >= slots.size())
		return false;

	Ref<Slot> slot = slots[slot_index];
	if (!slot->is_active())
		return false;

	Ref<ItemStack> stack = slot->get_stack();
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

Ref<ItemStack> Hotbar::get_stack_on_slot(const int slot_index) const {
	if (slot_index < 0 || slot_index >= slots.size())
		return nullptr;

	Ref<Slot> slot = slots[slot_index];
	if (!slot->is_active())
		return nullptr;

	Ref<ItemStack> stack = slot->get_stack();
	return stack;
}

Ref<ItemStack> Hotbar::get_stack_on_selection() const {
	return get_stack_on_slot(selection_index);
}

void Hotbar::set_auto_equip_mode(AutoEquipMode new_auto_equip_mode) {
	auto_equip_mode = new_auto_equip_mode;
	// Trigger auto-equip when mode is changed to enabled
	if (auto_equip_mode != AUTO_EQUIP_DISABLED && is_node_ready()) {
		_perform_auto_equip();
	}
}

Hotbar::AutoEquipMode Hotbar::get_auto_equip_mode() const {
	return auto_equip_mode;
}

void Hotbar::_perform_auto_equip() {
	Inventory *inventory = get_inventory();
	if (inventory == nullptr) {
		return;
	}

	TypedArray<ItemStack> inventory_stacks = inventory->get_stacks();
	
	if (auto_equip_mode == AUTO_EQUIP_BY_STACK_ORDER) {
		// Auto-equip by stack order (index in inventory)
		for (size_t i = 0; i < inventory_stacks.size() && i < (size_t)max_slots; i++) {
			Ref<ItemStack> stack = inventory_stacks[i];
			if (stack.is_valid() && stack->has_valid()) {
				// Only equip if slot is active and not already equipped with this stack
				if (i < slots.size()) {
					Ref<Slot> slot = slots[i];
					if (slot->is_active() && slot->get_stack() != stack) {
						slot->set_stack(stack);
						emit_signal("equipped", (int)i);
					}
				}
			}
		}
	} else if (auto_equip_mode == AUTO_EQUIP_BY_GRID_POSITION) {
		// Auto-equip by grid position (for GridInventory only)
		GridInventory *grid_inventory = Object::cast_to<GridInventory>(inventory);
		if (grid_inventory != nullptr) {
			for (size_t i = 0; i < inventory_stacks.size(); i++) {
				Ref<ItemStack> stack = inventory_stacks[i];
				if (stack.is_valid() && stack->has_valid()) {
					Vector2i position = grid_inventory->get_stack_position(stack);
					// Calculate hotbar slot from grid position (x + y * grid_width)
					// For the requested mapping: x=0,y=0 -> slot 0, x=1,y=0 -> slot 1
					int hotbar_slot = position.x + position.y * grid_inventory->get_size().x;
					
					// Only equip if within hotbar slot range
					if (hotbar_slot >= 0 && hotbar_slot < max_slots && hotbar_slot < slots.size()) {
						Ref<Slot> slot = slots[hotbar_slot];
						if (slot->is_active() && slot->get_stack() != stack) {
							slot->set_stack(stack);
							emit_signal("equipped", hotbar_slot);
						}
					}
				}
			}
		}
	}
}

Hotbar::Hotbar() {
	set_max_slots(max_slots);
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
		
		// Perform initial auto-equip if enabled
		if (auto_equip_mode != AUTO_EQUIP_DISABLED) {
			_perform_auto_equip();
		}
	}
	NodeInventories::_ready();
}

bool Hotbar::Slot::is_active() const {
	return active;
}

Ref<ItemStack> Hotbar::Slot::get_stack() const {
	return stack;
}

void Hotbar::Slot::clear() {
	set_stack(nullptr);
}

void Hotbar::Slot::set_stack(const Ref<ItemStack> &new_stack) {
	stack = new_stack;
}

void Hotbar::Slot::set_active(const bool new_active) {
	active = new_active;
	if (!active)
		clear();
}

void Hotbar::Slot::_bind_methods() {
}
