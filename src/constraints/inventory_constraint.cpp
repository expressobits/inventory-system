#include "inventory_constraint.h"

void InventoryConstraint::_bind_methods() {
    GDVIRTUAL_BIND(_can_add_on_inventory, "inventory", "item_id", "amount", "properties");
    GDVIRTUAL_BIND(_can_add_new_stack_on_inventory, "inventory", "item_id", "amount", "properties");
    GDVIRTUAL_BIND(_get_amount_to_add, "inventory", "item_id", "amount", "properties");
    GDVIRTUAL_BIND(_get_max_stack, "inventory", "item_id", "amount", "properties", "actual_max_stack");
}

InventoryConstraint::InventoryConstraint() {
}

InventoryConstraint::~InventoryConstraint() {
}

bool InventoryConstraint::can_add_on_inventory(const Node *inventory_node, const String item_id, const int amount, const Dictionary properties) {
	bool ret;
    if (GDVIRTUAL_CALL(_can_add_on_inventory, inventory_node, item_id, amount, properties, ret)) {
		return ret;
	}
	return true;
}

bool InventoryConstraint::can_add_new_stack_on_inventory(const Node *inventory_node, const String item_id, const int amount, const Dictionary properties) {
	bool ret;
    if (GDVIRTUAL_CALL(_can_add_new_stack_on_inventory, inventory_node, item_id, amount, properties, ret)) {
		return ret;
	}
	return true;
}

int InventoryConstraint::get_amount_to_add(const Node *inventory_node, const String item_id, const int amount, const Dictionary properties) {
	int to_add = amount;
    if (GDVIRTUAL_CALL(_get_amount_to_add, inventory_node, item_id, amount, properties, to_add)) {
		return to_add;
	}
	return to_add;
}

int InventoryConstraint::get_max_stack(const Node *inventory_node, const String item_id, const int amount, const Dictionary properties, const int actual_max_stack) {
	int new_max_stack = actual_max_stack;
	if (GDVIRTUAL_CALL(_get_max_stack, inventory_node, item_id, amount, properties, actual_max_stack, new_max_stack)) {
		return new_max_stack;
	}
	return new_max_stack;
}
