#include "loot.h"
#include <godot_cpp/classes/random_number_generator.hpp>

void Loot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &Loot::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Loot::get_items);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &Loot::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &Loot::get_id);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &Loot::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &Loot::get_name);
	ClassDB::bind_method(D_METHOD("get_random_item"), &Loot::get_random_item);
	ClassDB::bind_method(D_METHOD("serialize"), &Loot::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &Loot::deserialize);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, "LootItem"), "set_items", "get_items");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
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

Ref<LootItem> Loot::get_random_item() const {
	if (items.size() == 0) {
		return Ref<LootItem>();
	}

	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();

	// Minetest-style probability selection: 
	// Each item has an independent chance to be selected
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			// Check if random value (0.0 to 1.0) is <= item chance
			float random_value = rng->randf();
			if (random_value <= item->get_chance()) {
				return item;
			}
		}
	}

	// No item was selected based on chance
	return Ref<LootItem>();
}

Dictionary Loot::serialize() const {
	Dictionary data = Dictionary();
	data["id"] = id;
	data["name"] = name;
	
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