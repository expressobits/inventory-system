#ifndef GRID_INVENTORY_CLASS_H
#define GRID_INVENTORY_CLASS_H

// #include "base/node_inventories.h"
#include "core/inventory.h"

using namespace godot;

class QuadTree : public RefCounted {
public:
	class QuadRect : public RefCounted {
	public:
		Rect2i rect;
		Variant metadata;
		void _init(const Rect2i &rect, const Variant &metadata);
		String to_string() const;
	};

	class QuadNode : public RefCounted {
	private:
		static bool _can_subdivide(const Vector2i &size) {
			return size.x > 1 && size.y > 1;
		}
		static TypedArray<Rect2i> _get_quadrant_rects(const Rect2i &rect);

	public:
		QuadNode();
		~QuadNode();
		TypedArray<QuadNode> quadrants = TypedArray<QuadNode>();
		int quadrant_count = 0;
		TypedArray<QuadRect> quad_rects;
		Rect2i rect;
		void _init(const Rect2i &rect);
		String to_string() const;
		bool is_empty() const;
		Ref<QuadRect> get_first_under_rect(const Rect2i &test_rect, const Variant &exception_metadata = nullptr) const;
		Ref<QuadRect> get_first_containing_point(const Vector2i &point, const Variant &exception_metadata = nullptr) const;
		TypedArray<QuadRect> get_all_under_rect(const Rect2i &test_rect, const Variant &exception_metadata = nullptr) const;
		TypedArray<QuadRect> get_all_containing_point(const Vector2i &point, const Variant &exception_metadata = nullptr) const;
		void add(const Ref<QuadRect> &quad_rect);
		bool remove(const Variant &metadata);
		void _collapse();
	};

	Ref<QuadNode> _root;
	Vector2i size;
	void _init(const Vector2i &size);
	Ref<QuadTree::QuadRect> get_first(const Variant &at, const Variant &exception_metadata = nullptr) const;
	TypedArray<QuadRect> get_all(const Variant &at, const Variant &exception_metadata = nullptr) const;
	void add(const Rect2i &rect, const Variant &metadata);
	bool remove(const Variant &metadata);
	bool is_empty() const;
};

class GridInventory : public Inventory {
	GDCLASS(GridInventory, Inventory);

private:
	Vector2i _swap_position = Vector2i(0, 0);
	Ref<QuadTree> _quad_tree = memnew(Ref<QuadTree>((size)));
	Vector2i size = DEFAULT_SIZE;
	TypedArray<Vector2i> stack_positions = new TypedArray<Vector2i>();
	bool _bounds_broken() const;
	void _refresh_quad_tree();
	void _on_item_grid_info_changed(const Ref<ItemStack> stack);
	bool _on_pre_item_swap(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2);
	bool _size_check(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2);
	void _on_post_item_swap(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2);
	bool _is_sorted();
	void _move_item_to_unsafe(const Ref<ItemStack> &stack, const Vector2i &position);
	bool _compare_stacks(const Ref<ItemStack> &stack1, const Ref<ItemStack> &stack2) const;
	void _sort_if_needed();

protected:
	static void _bind_methods();

public:
	const Vector2i DEFAULT_SIZE = Vector2i(8, 8);
	GridInventory();
	~GridInventory();
	void set_size(const Vector2i &new_size);
	Vector2i get_size() const;
	Vector2i get_item_position(const Ref<ItemStack> &stack) const;
	bool set_item_position(const Ref<ItemStack> &stack, const Vector2i new_position);
	Vector2i get_stack_size(const Ref<ItemStack> &stack) const;
	Rect2i get_stack_rect(const Ref<ItemStack> &stack) const;
	bool set_stack_rect(const Ref<ItemStack> &stack, const Rect2i &new_rect);
	bool set_stack_rotation(const Ref<ItemStack> &stack);
	bool is_stack_rotated(const Ref<ItemStack> &stack) const;
	bool is_stack_rotation_positive(const Ref<ItemStack> &stack) const;
	void set_item_rotation_direction(const Ref<ItemStack> &stack, const bool positive);
	bool rotate_stack(const Ref<ItemStack> &stack);
	bool is_rotate_item(const Ref<ItemStack> &stack) const;
	bool can_rotate_item(const Ref<ItemStack> &stack) const;
	int add_at(const Vector2i position, const String item_id, const int amount = 1, const Dictionary &properties = Dictionary());
	int add_stack_at(const Ref<ItemStack> &stack, const Vector2i &position);
	Ref<ItemStack> get_stack_at(const Vector2i position) const;
	TypedArray<ItemStack> get_stacks_under(const Rect2i rect) const;
	bool move_stack_to(const Ref<ItemStack> stack, const Vector2i position);
	bool move_item_to_free_spot(const Ref<ItemStack> stack);
	bool transfer_to(const Vector2i from_position, const GridInventory *destination, const Vector2i destination_position);
	bool rect_free(const Rect2i &rect, const Ref<ItemStack> &exception = nullptr) const;
	Vector2i find_free_place(const Ref<ItemStack> &stack, const Ref<ItemStack> &exception = nullptr) const;
	bool sort();
	virtual void on_insert_stack(const int stack_index) override;
	virtual void on_removed_stack(const Ref<ItemStack> stack, const int stack_index) override;
};

#endif // GRID_INVENTORY_CLASS_H