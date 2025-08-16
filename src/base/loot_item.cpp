#include "loot_item.h"

void LootItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_id", "item_id"), &LootItem::set_item_id);
	ClassDB::bind_method(D_METHOD("get_item_id"), &LootItem::get_item_id);
	ClassDB::bind_method(D_METHOD("set_weight", "weight"), &LootItem::set_weight);
	ClassDB::bind_method(D_METHOD("get_weight"), &LootItem::get_weight);
	ClassDB::bind_method(D_METHOD("set_min_amount", "min_amount"), &LootItem::set_min_amount);
	ClassDB::bind_method(D_METHOD("get_min_amount"), &LootItem::get_min_amount);
	ClassDB::bind_method(D_METHOD("set_max_amount", "max_amount"), &LootItem::set_max_amount);
	ClassDB::bind_method(D_METHOD("get_max_amount"), &LootItem::get_max_amount);
	ClassDB::bind_method(D_METHOD("set_property_ranges", "property_ranges"), &LootItem::set_property_ranges);
	ClassDB::bind_method(D_METHOD("get_property_ranges"), &LootItem::get_property_ranges);
	ClassDB::bind_method(D_METHOD("serialize"), &LootItem::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &LootItem::deserialize);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_id"), "set_item_id", "get_item_id");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weight"), "set_weight", "get_weight");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_amount"), "set_min_amount", "get_min_amount");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_amount"), "set_max_amount", "get_max_amount");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "property_ranges"), "set_property_ranges", "get_property_ranges");
}

LootItem::LootItem() {
}

LootItem::~LootItem() {
}

void LootItem::set_item_id(const String &new_item_id) {
	item_id = new_item_id;
}

String LootItem::get_item_id() const {
	return item_id;
}

void LootItem::set_weight(const float &new_weight) {
	weight = new_weight;
}

float LootItem::get_weight() const {
	return weight;
}

void LootItem::set_min_amount(const int &new_min_amount) {
	min_amount = MAX(0, new_min_amount);
}

int LootItem::get_min_amount() const {
	return min_amount;
}

void LootItem::set_max_amount(const int &new_max_amount) {
	max_amount = new_max_amount;
}

int LootItem::get_max_amount() const {
	return max_amount;
}

void LootItem::set_property_ranges(const Dictionary &new_property_ranges) {
	property_ranges = new_property_ranges;
}

Dictionary LootItem::get_property_ranges() const {
	return property_ranges;
}

Dictionary LootItem::serialize() const {
	Dictionary data = Dictionary();
	data["item_id"] = item_id;
	data["weight"] = weight;
	data["min_amount"] = min_amount;
	data["max_amount"] = max_amount;
	if (!property_ranges.is_empty()) {
		data["property_ranges"] = property_ranges;
	}
	return data;
}

void LootItem::deserialize(const Dictionary &data) {
	if (data.has("item_id")) {
		item_id = data["item_id"];
	}
	if (data.has("weight")) {
		weight = data["weight"];
	}
	if (data.has("min_amount")) {
		min_amount = data["min_amount"];
	}
	if (data.has("max_amount")) {
		max_amount = data["max_amount"];
	}
	if (data.has("property_ranges")) {
		property_ranges = data["property_ranges"];
	}
}