#include "loot.h"
#include <godot_cpp/classes/random_number_generator.hpp>

void Loot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &Loot::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Loot::get_items);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &Loot::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &Loot::get_id);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &Loot::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &Loot::get_name);
	ClassDB::bind_method(D_METHOD("set_min_rolls", "min_rolls"), &Loot::set_min_rolls);
	ClassDB::bind_method(D_METHOD("get_min_rolls"), &Loot::get_min_rolls);
	ClassDB::bind_method(D_METHOD("set_max_rolls", "max_rolls"), &Loot::set_max_rolls);
	ClassDB::bind_method(D_METHOD("get_max_rolls"), &Loot::get_max_rolls);
	ClassDB::bind_method(D_METHOD("set_none_weight", "none_weight"), &Loot::set_none_weight);
	ClassDB::bind_method(D_METHOD("get_none_weight"), &Loot::get_none_weight);
	ClassDB::bind_method(D_METHOD("get_total_weight"), &Loot::get_total_weight);
	ClassDB::bind_method(D_METHOD("get_random_items", "rolls"), &Loot::get_random_items, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("serialize"), &Loot::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &Loot::deserialize);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, "LootItem"), "set_items", "get_items");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_rolls"), "set_min_rolls", "get_min_rolls");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_rolls"), "set_max_rolls", "get_max_rolls");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "none_weight"), "set_none_weight", "get_none_weight");
}

Loot::Loot() {
}

Loot::~Loot() {
}

void Loot::set_items(const TypedArray<LootItem> &new_items) {
	items = new_items;
}

TypedArray<LootItem> Loot::get_items() const {
	return items;
}

void Loot::set_id(const String &new_id) {
	id = new_id;
}

String Loot::get_id() const {
	return id;
}

void Loot::set_name(const String &new_name) {
	name = new_name;
}

String Loot::get_name() const {
	return name;
}

void Loot::set_min_rolls(const int &new_min_rolls) {
	min_rolls = MAX(0, new_min_rolls);
}

int Loot::get_min_rolls() const {
	return min_rolls;
}

void Loot::set_max_rolls(const int &new_max_rolls) {
	max_rolls = new_max_rolls;
}

int Loot::get_max_rolls() const {
	return max_rolls;
}

void Loot::set_none_weight(const float &new_none_weight) {
	none_weight = MAX(0.0, new_none_weight);
}

float Loot::get_none_weight() const {
	return none_weight;
}

float Loot::get_total_weight() const {
	float total = none_weight;
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			total += item->get_weight();
		}
	}
	return total;
}

TypedArray<LootItem> Loot::get_random_items(int rolls) const {
	TypedArray<LootItem> result;
	
	// If rolls is -1, use the configured min/max rolls
	if (rolls == -1) {
		if (min_rolls < 0 || max_rolls < 0) {
			return result;
		}
		
		Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
		rng->randomize();
		
		rolls = (min_rolls == max_rolls) ? min_rolls : rng->randi_range(min_rolls, max_rolls);
	}
	
	// If rolls is still <= 0 or no items, return empty result
	if (rolls <= 0 || items.size() == 0) {
		return result;
	}
	
	float total_weight = get_total_weight();
	if (total_weight <= 0.0) {
		return result;
	}
	
	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();
	
	for (int roll = 0; roll < rolls; roll++) {
		float random_value = rng->randf() * total_weight;
		float cumulative_weight = 0.0;
		
		// Check if we hit the "none" weight range first
		cumulative_weight += none_weight;
		if (random_value <= cumulative_weight) {
			// Don't add anything to result for "none" case
			continue;
		}
		
		// Check regular items
		for (int i = 0; i < items.size(); i++) {
			Ref<LootItem> item = items[i];
			if (item.is_valid()) {
				cumulative_weight += item->get_weight();
				if (random_value <= cumulative_weight) {
					result.append(item);
					break;
				}
			}
		}
	}
	
	return result;
}

Dictionary Loot::serialize() const {
	Dictionary data = Dictionary();
	data["id"] = id;
	data["name"] = name;
	data["min_rolls"] = min_rolls;
	data["max_rolls"] = max_rolls;
	data["none_weight"] = none_weight;
	
	Array items_data = Array();
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			items_data.append(item->serialize());
		}
	}
	if (!items_data.is_empty()) {
		data["items"] = items_data;
	}
	
	return data;
}

void Loot::deserialize(const Dictionary &data) {
	if (data.has("id")) {
		id = data["id"];
	}
	if (data.has("name")) {
		name = data["name"];
	}
	if (data.has("min_rolls")) {
		min_rolls = data["min_rolls"];
	}
	if (data.has("max_rolls")) {
		max_rolls = data["max_rolls"];
	}
	if (data.has("none_weight")) {
		none_weight = data["none_weight"];
	}
	
	if (data.has("items")) {
		Array items_data = data["items"];
		items.clear();
		for (int i = 0; i < items_data.size(); i++) {
			Ref<LootItem> item = memnew(LootItem());
			item->deserialize(items_data[i]);
			items.append(item);
		}
	}
}