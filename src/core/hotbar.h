#ifndef HOTBAR_CLASS_H
#define HOTBAR_CLASS_H

#include "base/node_inventories.h"
#include "inventory.h"

using namespace godot;

class Hotbar : public NodeInventories {
	GDCLASS(Hotbar, NodeInventories);

private:
	NodePath inventory_path;
	int slots_count = 4;
	int selection_index = -1;
	TypedArray<ItemStack> equipped_stacks;
	void _on_contents_changed();

protected:
	static void _bind_methods();

public:
	Hotbar();
	~Hotbar();
	virtual void _ready() override;
	void set_inventory_path(const NodePath &new_inventory);
	NodePath get_inventory_path() const;
	Inventory *get_inventory() const;
	void set_slots_count(const int &new_slots_count);
	int get_slots_count() const;
	void set_selection_index(const int &new_selection_index);
	int get_selection_index() const;
	void set_equipped_stacks(const TypedArray<ItemStack> new_equipped_stacks);
	TypedArray<ItemStack> get_equipped_stacks() const;

	void equip(Ref<ItemStack> stack, int slot_index = 0);
	void unequip(const int slot_index);
	void next_item();
	void previous_item();
	bool has_valid_item_id() const;
	bool has_item_on_selection() const;
	String get_selected_item_id() const;
	Ref<ItemStack> get_selected_stack() const;
};

#endif // HOTBAR_CLASS_H