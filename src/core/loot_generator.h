#ifndef LOOT_GENERATOR_CLASS_H
#define LOOT_GENERATOR_CLASS_H

#include "base/loot.h"
#include "base/node_inventories.h"
#include "core/inventory.h"
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

class LootGenerator : public NodeInventories {
	GDCLASS(LootGenerator, NodeInventories);

private:
	String loot_id;
	NodePath target_inventory_path;

protected:
	static void _bind_methods();

public:
	LootGenerator();
	~LootGenerator();
	void set_loot_id(const String &new_loot_id);
	String get_loot_id() const;
	Ref<Loot> get_loot() const;
	void set_target_inventory_path(const NodePath &new_target_inventory_path);
	NodePath get_target_inventory_path() const;
	Inventory *get_target_inventory() const;

	// Core loot generation functionality
	void generate_loot(int rolls = -1);

private:
	// Helper method to apply property ranges from LootItem to properties
	void apply_property_ranges(Dictionary &properties, const Dictionary &property_ranges, Ref<RandomNumberGenerator> &rng) const;
};

#endif // LOOT_GENERATOR_CLASS_H