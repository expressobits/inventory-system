#ifndef GRID_INVENTORY_CLASS_H
#define GRID_INVENTORY_CLASS_H

// #include "base/node_inventories.h"
#include "constraints/grid_inventory_constraint.h"
#include "core/inventory.h"
#include "core/quad_tree.h"

using namespace godot;


class GridInventory : public Inventory {
	GDCLASS(GridInventory, Inventory);

private:
	Vector2i _swap_position = Vector2i(0, 0);
	Ref<QuadTree> quad_tree;
	Vector2i size = Vector2i(8, 8);
	TypedArray<GridInventoryConstraint> grid_constraints;
	TypedArray<Vector2i> stack_positions;
	TypedArray<bool> stack_rotations;
	bool _bounds_broken() const;
	void _refresh_quad_tree();
	bool _size_check(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2);
	bool _is_sorted();
	void _move_stack_to_unsafe(const Ref<ItemStack> &stack, const Vector2i &position);
	bool _compare_stacks(const Ref<ItemStack> &stack1, const Ref<ItemStack> &stack2) const;
	void _sort_if_needed();
	bool _can_add_on_position(const Vector2i position, const String item_id, const int amount, const Dictionary properties, const bool is_rotated) const;

protected:
	static void _bind_methods();

public:
	const Vector2i DEFAULT_SIZE = Vector2i(8, 8);
	GridInventory();
	~GridInventory();
	void set_size(const Vector2i &new_size);
	Vector2i get_size() const;
	void set_grid_constraints(const TypedArray<GridInventoryConstraint> &new_grid_constraints);
	TypedArray<GridInventoryConstraint> get_grid_constraints() const;
	void set_quad_tree(const Ref<QuadTree> &new_quad_tree);
	Ref<QuadTree> get_quad_tree() const;
	void set_stack_positions(const TypedArray<Vector2i> &new_stack_positions);
	TypedArray<Vector2i> get_stack_positions() const;
	void set_stack_rotations(const TypedArray<bool> &new_stack_rotations);
	TypedArray<bool> get_stack_rotations() const;

	Vector2i get_stack_position(const Ref<ItemStack> &stack) const;
	bool set_stack_position(const Ref<ItemStack> &stack, const Vector2i new_position);
	bool can_rotate_item(const Ref<ItemStack> &stack) const;
	void rotate(const Ref<ItemStack> &stack);
	bool is_stack_rotated(const Ref<ItemStack> &stack) const;
	Vector2i get_stack_size(const Ref<ItemStack> &stack) const;
	Rect2i get_stack_rect(const Ref<ItemStack> &stack) const;

	Ref<ItemStack> get_stack_at(const Vector2i position) const;
	int get_stack_index_at(const Vector2i position) const;
	int add_at_position(const Vector2i position, const String item_id, const int amount = 1, const Dictionary &properties = Dictionary(), const bool is_rotated = false);
	TypedArray<ItemStack> get_stacks_under(const Rect2i rect) const;
	bool move_stack_to(const Ref<ItemStack> stack, const Vector2i position);
	int transfer_to(const Vector2i from_position, GridInventory *destination, const Vector2i destination_position, const int &amount = 1, const bool is_rotated = false);
	bool swap_stacks(const Vector2i position, GridInventory *other_inventory, const Vector2i other_position);
	bool rect_free(const Rect2i &rect, const Ref<ItemStack> &exception = nullptr) const;
	Vector2i find_free_place(const Vector2i stack_size, const String item_id, const int amount, const Dictionary properties, const bool is_rotated, const Ref<ItemStack> &exception = nullptr) const;
	bool sort();
	virtual Dictionary serialize() const;
	virtual void deserialize(const Dictionary data);
	virtual bool can_add_new_stack(const String &item_id, const int &amount, const Dictionary &properties) const;
	virtual bool has_space_for(const String &item_id, const int amount = 1, const Dictionary &properties = Dictionary(), const bool is_rotated = false) const;
	virtual void on_insert_stack(const int stack_index) override;
	virtual void on_removed_stack(const Ref<ItemStack> stack, const int stack_index) override;
};

#endif // GRID_INVENTORY_CLASS_H