#include "loot_generator.h"

void LootGenerator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_loot_id", "loot_id"), &LootGenerator::set_loot_id);
	ClassDB::bind_method(D_METHOD("get_loot_id"), &LootGenerator::get_loot_id);
	ClassDB::bind_method(D_METHOD("get_loot"), &LootGenerator::get_loot);
	ClassDB::bind_method(D_METHOD("set_target_inventory_path", "target_inventory_path"), &LootGenerator::set_target_inventory_path);
	ClassDB::bind_method(D_METHOD("get_target_inventory_path"), &LootGenerator::get_target_inventory_path);
	ClassDB::bind_method(D_METHOD("get_target_inventory"), &LootGenerator::get_target_inventory);
	ClassDB::bind_method(D_METHOD("generate_loot", "rolls"), &LootGenerator::generate_loot, DEFVAL(-1));

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "loot_id"), "set_loot_id", "get_loot_id");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_inventory_path"), "set_target_inventory_path", "get_target_inventory_path");
}

LootGenerator::LootGenerator() {
}

LootGenerator::~LootGenerator() {
}

void LootGenerator::set_loot_id(const String &new_loot_id) {
	loot_id = new_loot_id;
}

String LootGenerator::get_loot_id() const {
	return loot_id;
}

Ref<Loot> LootGenerator::get_loot() const {
	if (loot_id.is_empty()) {
		return nullptr;
	}

	if (!get_database().is_valid()) {
		return nullptr;
	}

	return get_database()->get_loot_from_id(loot_id);
}

void LootGenerator::set_target_inventory_path(const NodePath &new_target_inventory_path) {
	target_inventory_path = new_target_inventory_path;
}

NodePath LootGenerator::get_target_inventory_path() const {
	return target_inventory_path;
}

Inventory *LootGenerator::get_target_inventory() const {
	if (target_inventory_path.is_empty()) {
		return nullptr;
	}

	Node *node = get_node_or_null(target_inventory_path);
	return Object::cast_to<Inventory>(node);
}

void LootGenerator::generate_loot(int rolls) {
	if (loot_id.is_empty()) {
		ERR_PRINT("LootGenerator: No loot ID assigned");
		return;
	}

	if (!get_database().is_valid()) {
		ERR_PRINT("LootGenerator: No database assigned");
		return;
	}

	Ref<Loot> loot = get_database()->get_loot_from_id(loot_id);
	if (!loot.is_valid()) {
		ERR_PRINT("LootGenerator: Loot with ID '" + loot_id + "' not found in database");
		return;
	}

	Inventory *target_inventory = get_target_inventory();
	if (!target_inventory) {
		ERR_PRINT("LootGenerator: No target inventory found at path");
		return;
	}

	// Use the unified loot API - when rolls = -1 (default), uses configured min/max rolls
	// when rolls > 0, uses that specific roll count
	TypedArray<LootItem> loot_items = loot->get_random_items(rolls);

	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();

	for (int i = 0; i < loot_items.size(); i++) {
		Ref<LootItem> loot_item = loot_items[i];
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

		Dictionary properties = get_database()->create_dynamic_properties(loot_item->get_item_id());

		// Apply property ranges from LootItem
		Dictionary property_ranges = loot_item->get_property_ranges();
		if (!property_ranges.is_empty()) {
			apply_property_ranges(properties, property_ranges, rng);
		}

		// Add items to the inventory using the item_id and amount
		target_inventory->add(loot_item->get_item_id(), amount, properties);
	}
}

void LootGenerator::apply_property_ranges(Dictionary &properties, const Dictionary &property_ranges, Ref<RandomNumberGenerator> &rng) const {
	Array property_keys = property_ranges.keys();

	for (int i = 0; i < property_keys.size(); i++) {
		String property_name = property_keys[i];
		Dictionary range_config = property_ranges[property_name];

		if (range_config.has("min") && range_config.has("max")) {
			// Handle range-based properties (int/float)
			Variant min_val = range_config["min"];
			Variant max_val = range_config["max"];

			if (min_val.get_type() == Variant::INT && max_val.get_type() == Variant::INT) {
				// Integer range
				int min_int = min_val;
				int max_int = max_val;
				int random_value = rng->randi_range(min_int, max_int);
				properties[property_name] = random_value;
			} else if (min_val.get_type() == Variant::FLOAT || max_val.get_type() == Variant::FLOAT) {
				// Float range (convert both to float for consistency)
				float min_float = min_val;
				float max_float = max_val;
				float random_value = rng->randf_range(min_float, max_float);
				properties[property_name] = random_value;
			}
		} else if (range_config.has("value")) {
			// Handle fixed value properties (bool or any other type)
			properties[property_name] = range_config["value"];
		}
	}
}