#include "loot_generator.h"

void LootGenerator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_loot_table_id", "loot_table_id"), &LootGenerator::set_loot_table_id);
	ClassDB::bind_method(D_METHOD("get_loot_table_id"), &LootGenerator::get_loot_table_id);
	ClassDB::bind_method(D_METHOD("get_loot_table"), &LootGenerator::get_loot_table);
	ClassDB::bind_method(D_METHOD("set_target_inventory_path", "target_inventory_path"), &LootGenerator::set_target_inventory_path);
	ClassDB::bind_method(D_METHOD("get_target_inventory_path"), &LootGenerator::get_target_inventory_path);
	ClassDB::bind_method(D_METHOD("get_target_inventory"), &LootGenerator::get_target_inventory);
	ClassDB::bind_method(D_METHOD("generate_loot"), &LootGenerator::generate_loot);
	ClassDB::bind_method(D_METHOD("generate_loot_count", "count"), &LootGenerator::generate_loot_count);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "loot_table_id"), "set_loot_table_id", "get_loot_table_id");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_inventory_path"), "set_target_inventory_path", "get_target_inventory_path");
}

LootGenerator::LootGenerator() {
}

LootGenerator::~LootGenerator() {
}

void LootGenerator::set_loot_table_id(const String &new_loot_table_id) {
	loot_table_id = new_loot_table_id;
}

String LootGenerator::get_loot_table_id() const {
	return loot_table_id;
}

Ref<Loot> LootGenerator::get_loot_table() const {
	if (loot_table_id.is_empty()) {
		return nullptr;
	}
	
	if (!get_database().is_valid()) {
		return nullptr;
	}
	
	return get_database()->get_loot_table_from_id(loot_table_id);
}

void LootGenerator::set_target_inventory_path(const NodePath &new_target_inventory_path) {
	target_inventory_path = new_target_inventory_path;
}

NodePath LootGenerator::get_target_inventory_path() const {
	return target_inventory_path;
}

Inventory* LootGenerator::get_target_inventory() const {
	if (target_inventory_path.is_empty()) {
		return nullptr;
	}
	
	Node* node = get_node_or_null(target_inventory_path);
	return Object::cast_to<Inventory>(node);
}

void LootGenerator::generate_loot() {
	generate_loot_count(1);
}

void LootGenerator::generate_loot_count(int count) {
	if (loot_table_id.is_empty()) {
		ERR_PRINT("LootGenerator: No loot table ID assigned");
		return;
	}
	
	if (!get_database().is_valid()) {
		ERR_PRINT("LootGenerator: No database assigned");
		return;
	}
	
	Ref<Loot> loot_table = get_database()->get_loot_table_from_id(loot_table_id);
	if (!loot_table.is_valid()) {
		ERR_PRINT("LootGenerator: Loot table with ID '" + loot_table_id + "' not found in database");
		return;
	}
	
	Inventory* target_inventory = get_target_inventory();
	if (!target_inventory) {
		ERR_PRINT("LootGenerator: No target inventory found at path");
		return;
	}
	
	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();
	
	for (int i = 0; i < count; i++) {
		Ref<LootItem> loot_item = loot_table->get_random_item();
		if (!loot_item.is_valid()) {
			continue;
		}
		
		// Check if the item exists in the database
		Ref<ItemDefinition> item_def = get_item_from_id(loot_item->get_item_id());
		if (!item_def.is_valid()) {
			ERR_PRINT("LootGenerator: Item with ID '" + loot_item->get_item_id() + "' not found in database");
			continue;
		}
		
		// Generate random amount between min and max
		int min_amount = loot_item->get_min_amount();
		int max_amount = loot_item->get_max_amount();
		int amount = rng->randi_range(min_amount, max_amount);
		
		// Add items to the inventory using the item_id and amount
		target_inventory->add(loot_item->get_item_id(), amount);
	}
}