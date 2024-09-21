#ifndef INVENTORY_HANDLER_CLASS_H
#define INVENTORY_HANDLER_CLASS_H

#include "base/node_inventories.h"
#include "inventory.h"
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

class InventoryHandler : public NodeInventories {
	GDCLASS(InventoryHandler, NodeInventories);

private:
	// TODO More slot transactions (Queue transactions equal Project Zomboid)
	Ref<Slot> transaction_slot;

protected:
	static void _bind_methods();

public:
	InventoryHandler();
	~InventoryHandler();
	virtual void _ready() override;
	void set_transaction_slot(const Ref<Slot> new_transaction_slot);
	Ref<Slot> get_transaction_slot() const;
	void change_transaction_slot(const Ref<Item> &item, const int &amount);
	virtual void to_transaction(const int &slot_index, Inventory *inventory, const int &amount);
	virtual void transaction_to_at(const int &slot_index, Inventory *inventory, const int &amount_to_move = -1);
	virtual void transaction_to(Inventory *inventory);
	bool is_transaction_active() const;
	virtual void drop_transaction(Inventory	*inventory);
};

#endif // INVENTORY_HANDLER_CLASS_H