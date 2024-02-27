#ifndef NODE_INVENTORIES_CLASS_H
#define NODE_INVENTORIES_CLASS_H

#include "inventory_database.h"
#include "item_definition.h"
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class NodeInventories : public Node {
	GDCLASS(NodeInventories, Node);

private:
	Ref<InventoryDatabase> database;
	Dictionary items_cache;

protected:
	static void _bind_methods();

public:
	NodeInventories();
	~NodeInventories();
	virtual void _ready() override;
	virtual PackedStringArray _get_configuration_warnings() const override;
	void set_database(const Ref<InventoryDatabase> new_database);
	Ref<InventoryDatabase> get_database() const;
	Ref<ItemDefinition> get_item_from_id(const int id) const;
	int get_id_from_item(const Ref<ItemDefinition> item) const;
};

#endif // NODE_INVENTORIES_CLASS_H