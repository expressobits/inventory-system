#ifndef INVENTORY_HANDLER_CLASS_H
#define INVENTORY_HANDLER_CLASS_H

#include "base/node_inventories.h"
#include "inventory.h"
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

class InventoryHandler : public NodeInventories {
	GDCLASS(InventoryHandler, NodeInventories);

private:
	NodePath drop_parent_path = NodePath("../../..");
	NodePath drop_parent_position_path = NodePath("../..");
	TypedArray<NodePath> inventories_path;
	Node *drop_parent;
	Node *drop_parent_position;
	// TypedArray<Inventory> inventories;
	TypedArray<NodePath> opened_inventories;
	// TODO More slot transactions (Queue transactions equal Project Zomboid)
	Ref<Slot> transaction_slot;

protected:
	static void _bind_methods();
	void _instantiate_dropped_item(Ref<PackedScene> &dropped_item, const Ref<Item> &item);
	void _set_transaction_slot(const Ref<Item> &item, const int &amount);

public:
	InventoryHandler();
	~InventoryHandler();
	virtual void _ready() override;
	void set_drop_parent_path(const NodePath new_drop_parent_path);
	NodePath get_drop_parent_path() const;
	void set_drop_parent_position_path(const NodePath new_drop_parent_position_path);
	NodePath get_drop_parent_position_path() const;
	void set_inventories_path(const TypedArray<NodePath> new_inventories_path);
	TypedArray<NodePath> get_inventories_path() const;
	void set_transaction_slot(const Ref<Slot> new_transaction_slot);
	Ref<Slot> get_transaction_slot() const;
	bool drop(const Ref<Item> &item, const int &amount = 1);
	int add_to_inventory(Inventory *inventory, const Ref<Item> item, const int &amount = 1, bool drop_excess = false);
	void drop_from_inventory(const int &slot_index, const int &amount = 1, Inventory *inventory = nullptr);
	bool pick_to_inventory(Node *dropped_item, Inventory *inventory = nullptr);
	int move_between_inventories(Inventory *from, const int slot_index, const int amount, Inventory *to);
	void move_between_inventories_at(Inventory *from, const int slot_index, const int amount, Inventory *to, const int to_slot_index);
	void swap_between_inventories(Inventory *inventory, const int slot_index, Inventory *other_inventory, const int other_slot_index, int amount = 1);
	bool open(Inventory *inventory);
	bool close(Inventory *inventory);
	bool is_open_main_inventory() const;
	bool is_open_any_inventory() const;
	bool is_open(const Inventory *inventory) const;
	bool open_main_inventory();
	bool close_main_inventory();
	void close_all_inventories();
	void to_transaction(const int &slot_index, Inventory *inventory, const int &amount);
	void transaction_to_at(const int &slot_index, Inventory *inventory, const int &amount_to_move = -1);
	void transaction_to(Inventory *inventory);
	bool is_transaction_active() const;
	void drop_transaction();
	Inventory *get_inventory(const int &index) const;
};

#endif // INVENTORY_HANDLER_CLASS_H