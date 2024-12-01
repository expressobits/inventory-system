#ifndef GRID_INVENTORY_CONSTRAINT_CLASS_H
#define GRID_INVENTORY_CONSTRAINT_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GridInventoryConstraint : public Resource {
	GDCLASS(GridInventoryConstraint, Resource);

private:

protected:
	static void _bind_methods();

public:
	GridInventoryConstraint();
	~GridInventoryConstraint();
	virtual bool can_add_on_position(const Node* inventory_node, const Vector2i position, const String item_id, const int amount, const Dictionary properties, const bool is_rotated);
	GDVIRTUAL6R(bool, _can_add_on_position, const Node*, Vector2i, String, int, Dictionary, bool);

};

#endif // GRID_INVENTORY_CONSTRAINT_CLASS_H