#ifndef INVENTORY_CLASS_H
#define INVENTORY_CLASS_H

#include "base/node_inventories.h"
#include "base/slot.h"
#include "base/item_stack.h"

using namespace godot;

class Inventory : public NodeInventories {
	GDCLASS(Inventory, NodeInventories);

private:
	TypedArray<ItemStack> items;
	int slot_amount = 16;
	int max_size = 16;
	String inventory_name = "Inventory";
	void _load_slots();
	void _insert_stack(int stack_index);
	void _remove_stack_at(int stack_index);
	void _call_events(int old_amount);
	int _add_to_stack(int stack_index, const String &item_id, int amount = 1, const Dictionary &properties = Dictionary());
	int _remove_from_stack(int stack_index, const String &item_id, int amount = 1);

protected:
	static void _bind_methods();

public:
	Inventory();
	~Inventory();
	virtual void _enter_tree() override;
	void set_stack_content(const int stack_index, const String &item_id, const int &amount, const Dictionary &properties);
	void update_stack(const int stack_index);
	bool is_empty() const;
	bool is_full() const;
	bool contains(const String &item, const int &amount = 1) const;
	bool contains_at(const int &stack_index, const String &item_id, const int &amount = 1) const;
	bool contains_category(const Ref<ItemCategory> &category, const int &amount = 1) const;
	int get_stack_index_with_an_item_of_category(const Ref<ItemCategory> &category) const;
	int amount_of_item(const String &item) const;
	int amount_of_category(const Ref<ItemCategory> &category) const;
	int amount() const;
	int add(const String &item_id, const int &amount, const Dictionary &properties = Dictionary(), const bool &drop_excess = false);
	int add_at(const int &stack_index, const String &item_id, const int &amount = 1, const Dictionary &properties = Dictionary());
	int remove(const String &item_id, const int &amount = 1);
	int remove_at(const int &stack_index, const String &item_id, const int &amount = 1);
	void transfer_at(const int &stack_index, Inventory *destination, const int &destination_stack_index, const int &amount = 1);
	void transfer(const int &stack_index, Inventory *destination, const int &amount = 1);
	virtual bool drop(const String &item_id, const int &amount, const Dictionary &properties);
	void drop_from_inventory(const int &stack_index, const int &amount = 1, const Dictionary &properties = Dictionary());
	int add_to_stack(Ref<ItemStack> stack, const String &item_id, const int &amount, const Dictionary &properties = Dictionary());
	int remove_from_stack(Ref<ItemStack> stack, const String &item_id, const int &amount);
	bool is_accept_any_categories(const int categories_flag, const TypedArray<ItemCategory> &categories) const;
	int get_max_stack_of_stack(const Ref<ItemStack> &stack, Ref<ItemDefinition> &item) const;
	bool contains_category_in_stack(const Ref<ItemStack> &slot, const Ref<ItemCategory> &category) const;
	void set_items(const TypedArray<ItemStack> &new_items);
	TypedArray<ItemStack> get_items() const;
	void set_slot_amount(const int &new_slot_amount);
	int get_slot_amount() const;
	void set_inventory_name(const String &new_inventory_name);
	String get_inventory_name() const;
	Dictionary serialize() const;
	void deserialize(const Dictionary data);
};

#endif // INVENTORY_CLASS_H