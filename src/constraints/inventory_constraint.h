#ifndef INVENTORY_CONSTRAINT_CLASS_H
#define INVENTORY_CONSTRAINT_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class InventoryConstraint : public Resource {
	GDCLASS(InventoryConstraint, Resource);

private:

protected:
	static void _bind_methods();

public:
	InventoryConstraint();
	~InventoryConstraint();
	virtual bool can_add_on_inventory(const Node* inventory_node, const String item_id, const int amount, const Dictionary properties);
	virtual bool can_add_new_stack_on_inventory(const Node* inventory_node, const String item_id, const int amount, const Dictionary properties);
	virtual int get_amount_to_add(const Node* inventory_node, const String item_id, const int amount, const Dictionary properties);
	virtual int get_max_stack(const Node* inventory_node, const String item_id, const int amount, const Dictionary properties, const int actual_max_stack);
	GDVIRTUAL4R(bool, _can_add_on_inventory, const Node*, String, int, Dictionary);
	GDVIRTUAL4R(bool, _can_add_new_stack_on_inventory, const Node*, String, int, Dictionary);
	GDVIRTUAL4R(int, _get_amount_to_add, const Node*, String, int, Dictionary);
	GDVIRTUAL5R(int, _get_max_stack, const Node*, String, int, Dictionary, int);

};

#endif // INVENTORY_CONSTRAINT_CLASS_H