#ifndef INVENTORY_CLASS_H
#define INVENTORY_CLASS_H

#include "base/node_inventories.h"
#include "base/item_stack.h"
#include "constraints/inventory_constraint.h"

using namespace godot;

class Inventory : public NodeInventories {
	GDCLASS(Inventory, NodeInventories);

private:
	int max_size = 16;
	String inventory_name = "Inventory";
	TypedArray<InventoryConstraint> constraints = new TypedArray<InventoryConstraint>();
	void _load_slots();
	void _insert_stack(int stack_index);
	void _remove_stack_at(int stack_index);
	void _call_events(int old_amount);
	int _add_to_stack(int stack_index, const String &item_id, int amount = 1, const Dictionary &properties = Dictionary());
	int _remove_from_stack(int stack_index, const String &item_id, int amount = 1);

protected:
	bool _flag_contents_changed = false;
	TypedArray<ItemStack> stacks;
	static void _bind_methods();
	int _get_max_stack_for_stack(const String item_id, const int amount, const Dictionary properties) const;
	bool _can_add_on_inventory_from_constraints(const String item_id, const int amount, const Dictionary properties) const;
	bool _can_add_new_stack_on_inventory_from_constraints(const String item_id, const int amount, const Dictionary properties) const;
	int _get_max_stack_from_constraints(const String item_id, const int amount, const Dictionary properties) const;
	int _get_amount_to_add_from_constraints(const String item_id, const int amount, const Dictionary properties) const;
	bool _is_override_max_stack_from_constraints(const String item_id, const int amount, const Dictionary properties) const;
	bool _can_swap_to_inventory(const Inventory* inventory, const String item_id, const int amount, const Dictionary properties) const;

public:
	Inventory();
	~Inventory();
	virtual void _enter_tree() override;
	virtual void _process(float delta);
	void set_stack_content(const int stack_index, const String &item_id, const int &amount, const Dictionary &properties);
	void update_stack(const int stack_index);
	bool is_empty() const;
	bool is_full() const;
	void clear();
	bool contains(const String &item, const int &amount = 1) const;
	bool contains_at(const int &stack_index, const String &item_id, const int &amount = 1) const;
	bool contains_category(const Ref<ItemCategory> &category, const int &amount = 1) const;
	virtual bool has_space_for(const String &item_id, const int amount = 1, const Dictionary &properties = Dictionary()) const;
	virtual bool can_stack_with_actual_slots(const String &item_id, const int amount = 1, const Dictionary &properties = Dictionary()) const;
	bool has_stack(const Ref<ItemStack> &stack) const;
	int get_stack_index_with_an_item_of_category(const Ref<ItemCategory> &category) const;
	int amount_of_item(const String &item) const;
	int amount_of_category(const Ref<ItemCategory> &category) const;
	int amount() const;
	virtual int add(const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary(), const bool &drop_excess = false);
	int add_at_index(const int &stack_index, const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary());
	int add_on_new_stack(const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary());
	int insert_stack(const int &stack_index, const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary());
	void remove_stack(const int &stack_index);
	int remove(const String &item_id, const int &amount = 1);
	int remove_at(const int &stack_index, const String &item_id, const int &amount = 1);
	bool split(const int &stack_index, const int &amount = 1);
	int transfer_at(const int &stack_index, Inventory *destination, const int &destination_stack_index, const int &amount = 1);
	int transfer(const int &stack_index, Inventory *destination, const int &amount = 1);
	virtual bool drop(const String &item_id, const int &amount, const Dictionary &properties);
	void drop_from_inventory(const int &stack_index, const int &amount = 1, const Dictionary &properties = Dictionary());
	int add_to_stack(Ref<ItemStack> stack, const String &item_id, const int &amount, const Dictionary &properties = Dictionary());
	int remove_from_stack(Ref<ItemStack> stack, const String &item_id, const int &amount);
	bool is_accept_any_categories(const int categories_flag, const TypedArray<ItemCategory> &categories) const;
	int get_max_stack_of_stack(const Ref<ItemStack> &stack, Ref<ItemDefinition> &item) const;
	bool contains_category_in_stack(const Ref<ItemStack> &slot, const Ref<ItemCategory> &category) const;
	void set_stacks(const TypedArray<ItemStack> &new_items);
	TypedArray<ItemStack> get_stacks() const;
	void set_inventory_name(const String &new_inventory_name);
	String get_inventory_name() const;
	void set_constraints(const TypedArray<InventoryConstraint> &new_constraints);
	TypedArray<InventoryConstraint> get_constraints() const;
	virtual Dictionary serialize() const;
	virtual void deserialize(const Dictionary data);
	virtual bool can_add_new_stack(const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary()) const;
	virtual void on_insert_stack(const int stack_index);
	virtual void on_removed_stack(const Ref<ItemStack> stack, const int stack_index);
};

#endif // INVENTORY_CLASS_H