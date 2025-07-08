#include "loot.h"
#include <godot_cpp/classes/random_number_generator.hpp>

void Loot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &Loot::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Loot::get_items);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &Loot::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &Loot::get_name);
	ClassDB::bind_method(D_METHOD("get_total_weight"), &Loot::get_total_weight);
	ClassDB::bind_method(D_METHOD("get_random_item"), &Loot::get_random_item);
	ClassDB::bind_method(D_METHOD("serialize"), &Loot::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &Loot::deserialize);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, "LootItem"), "set_items", "get_items");
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

void Loot::set_name(const String &new_name) {
	name = new_name;
}

String Loot::get_name() const {
	return name;
}

float Loot::get_total_weight() const {
	float total = 0.0;
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			total += item->get_weight();
		}
	}
	return total;
}

Ref<LootItem> Loot::get_random_item() const {
	if (items.size() == 0) {
		return Ref<LootItem>();
	}

	float total_weight = get_total_weight();
	if (total_weight <= 0.0) {
		return Ref<LootItem>();
	}

	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();
	float random_value = rng->randf() * total_weight;

	float cumulative_weight = 0.0;
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			cumulative_weight += item->get_weight();
			if (random_value <= cumulative_weight) {
				return item;
			}
		}
	}

	// Fallback to last item if something goes wrong
	if (items.size() > 0) {
		return items[items.size() - 1];
	}
	
	return Ref<LootItem>();
}

Dictionary Loot::serialize() const {
	Dictionary data = Dictionary();
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