#ifndef LOOT_GENERATOR_CLASS_H
#define LOOT_GENERATOR_CLASS_H

#include "base/node_inventories.h"
#include "base/loot.h"
#include "core/inventory.h"
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

class LootGenerator : public NodeInventories {
	GDCLASS(LootGenerator, NodeInventories);

private:
	Ref<Loot> loot_table;
	NodePath target_inventory_path;

protected:
	static void _bind_methods();

public:
	LootGenerator();
	~LootGenerator();
	void set_loot_table(const Ref<Loot> &new_loot_table);
	Ref<Loot> get_loot_table() const;
	void set_target_inventory_path(const NodePath &new_target_inventory_path);
	NodePath get_target_inventory_path() const;
	Inventory* get_target_inventory() const;
	
	// Core loot generation functionality
	void generate_loot();
	void generate_loot_count(int count);
};

#endif // LOOT_GENERATOR_CLASS_H