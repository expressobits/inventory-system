#ifndef INVENTORY_CLASS_H
#define INVENTORY_CLASS_H

#include "base/node_inventories.h"
#include "base/slot.h"

using namespace godot;

class Inventory : public NodeInventories {
	GDCLASS(Inventory, NodeInventories);

private:
	TypedArray<Slot> slots;
	bool is_open = false;
	bool create_slot_if_needed = false;
	bool remove_slot_if_empty = false;
	int slot_amount = 16;
	String inventory_name = "Inventory";
	void _load_slots();
	void _call_events(int old_amount);
	int _add_to_slot(int slot_index, const Ref<Item> &item, int amount = 1);
	int _remove_from_slot(int slot_index, const Ref<Item> &item, int amount = 1);

protected:
	static void _bind_methods();

public:
	Inventory();
	~Inventory();
	virtual void _enter_tree() override;
	void update_slot(const int slot_index);
	void add_slot(int slot_index, bool emit_signal = true);
	void remove_slot(int slot_index, bool emit_signal = true);
	void set_slot(const int &slot_index, const Ref<Item> &item, const int &amount);
	void set_slot_content(const int slot_index, const Ref<ItemDefinition> &definition, const Dictionary &properties, const int &amount);
	void set_slot_with_other_slot(const int slot_index, const Ref<Slot> &other_slot);
	bool is_empty_slot(const int &slot_index) const;
	bool is_empty() const;
	bool is_full() const;
	bool contains(const Ref<Item> &item, const int &amount = 1) const;
	bool contains_at(const int &slot_index, const Ref<Item> &item, const int &amount = 1) const;
	bool contains_category(const Ref<ItemCategory> &category, const int &amount = 1) const;
	int get_slot_index_with_an_item_of_category(const Ref<ItemCategory> &category) const;
	int get_amount_of(const Ref<Item> &item) const;
	int get_amount_of_category(const Ref<ItemCategory> &category) const;
	int get_amount() const;
	int add(const Ref<Item> &item, const int &amount);
	int add_at(const int &slot_index, const Ref<Item> &item, const int &amount = 1);
	int remove(const Ref<Item> &item, const int &amount = 1);
	int remove_at(const int &slot_index, const Ref<Item> &item, const int &amount = 1);
	bool open();
	bool close();
	void set_slots(const TypedArray<Slot> &new_slots);
	TypedArray<Slot> get_slots() const;
	void set_is_open(const bool &new_is_open);
	bool get_is_open() const;
	void set_create_slot_if_needed(const bool &new_create_slot_if_needed);
	bool get_create_slot_if_needed() const;
	void set_remove_slot_if_empty(const bool &new_remove_slot_if_empty);
	bool get_remove_slot_if_empty() const;
	void set_slot_amount(const int &new_slot_amount);
	int get_slot_amount() const;
	void set_inventory_name(const String &new_inventory_name);
	String get_inventory_name() const;
};

#endif // INVENTORY_CLASS_H