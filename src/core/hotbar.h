#ifndef HOTBAR_CLASS_H
#define HOTBAR_CLASS_H

#include "base/node_inventories.h"
#include "base/slot.h"
#include "inventory.h"


using namespace godot;

class Hotbar : public NodeInventories {
	GDCLASS(Hotbar, NodeInventories);

private:
	NodePath inventory_path;
	int slots_in_hot_bar = 8;
	int selection_index = 0;
	void _on_updated_slot(const int slot_index);

protected:
	static void _bind_methods();

public:
	Hotbar();
	~Hotbar();
	virtual void _ready() override;
	void set_inventory_path(const NodePath &new_inventory);
	NodePath get_inventory_path() const;
	Inventory *get_inventory() const;
	void set_slots_in_hot_bar(const int &new_slots_in_hot_bar);
	int get_slots_in_hot_bar() const;
	void set_selection_index(const int &new_selection_index);
	int get_selection_index() const;
	void change_selection(const int &new_index);
	void next_item();
	void previous_item();
	bool has_valid_item_id() const;
	bool has_item_on_selection() const;
	Ref<Item> get_selected_item() const;
};

#endif // HOTBAR_CLASS_H