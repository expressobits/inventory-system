#include "inventory_database.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>

void InventoryDatabase::_update_items_cache() {
	items_cache.clear();
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemDefinition> item = items[i];
		if (item != nullptr) {
			items_cache[item->get_id()] = item;
		}
	}
}

void InventoryDatabase::_update_items_categories_cache() {
	categories_code_cache.clear();
	for (size_t i = 0; i < item_categories.size(); i++) {
		Ref<ItemCategory> category = item_categories[i];
		if (category == nullptr)
			continue;
		if (!Engine::get_singleton()->is_editor_hint()) {
			category->set_code(pow(2, i));
		}
		categories_code_cache[pow(2, i)] = category;
	}
}

void InventoryDatabase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &InventoryDatabase::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &InventoryDatabase::get_items);
	ClassDB::bind_method(D_METHOD("set_recipes", "recipes"), &InventoryDatabase::set_recipes);
	ClassDB::bind_method(D_METHOD("get_recipes"), &InventoryDatabase::get_recipes);
	ClassDB::bind_method(D_METHOD("set_stations_type", "stations_type"), &InventoryDatabase::set_stations_type);
	ClassDB::bind_method(D_METHOD("get_stations_type"), &InventoryDatabase::get_stations_type);
	ClassDB::bind_method(D_METHOD("set_item_categories", "item_categories"), &InventoryDatabase::set_item_categories);
	ClassDB::bind_method(D_METHOD("get_item_categories"), &InventoryDatabase::get_item_categories);
	ClassDB::bind_method(D_METHOD("add_new_item", "item"), &InventoryDatabase::add_new_item);
	ClassDB::bind_method(D_METHOD("remove_item", "item"), &InventoryDatabase::remove_item);
	ClassDB::bind_method(D_METHOD("add_new_category", "category"), &InventoryDatabase::add_new_category);
	ClassDB::bind_method(D_METHOD("remove_category", "category"), &InventoryDatabase::remove_category);
	ClassDB::bind_method(D_METHOD("get_item", "id"), &InventoryDatabase::get_item);
	ClassDB::bind_method(D_METHOD("has_item_id", "id"), &InventoryDatabase::has_item_id);
	ClassDB::bind_method(D_METHOD("get_valid_id"), &InventoryDatabase::get_valid_id);
	ClassDB::bind_method(D_METHOD("get_new_valid_id"), &InventoryDatabase::get_new_valid_id);
	ClassDB::bind_method(D_METHOD("get_category", "code"), &InventoryDatabase::get_category);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")), "set_items", "get_items");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "recipes", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Recipe")), "set_recipes", "get_recipes");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "stations_type", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType")), "set_stations_type", "get_stations_type");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "item_categories", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")), "set_item_categories", "get_item_categories");
}

InventoryDatabase::InventoryDatabase() {
}

InventoryDatabase::~InventoryDatabase() {
}

void InventoryDatabase::set_items(const TypedArray<ItemDefinition> &new_items) {
	items = new_items;
	_update_items_cache();
}

TypedArray<ItemDefinition> InventoryDatabase::get_items() const {
	return items;
}

void InventoryDatabase::set_recipes(const TypedArray<Recipe> &new_recipes) {
	recipes = new_recipes;
}

TypedArray<Recipe> InventoryDatabase::get_recipes() const {
	return recipes;
}

void InventoryDatabase::set_stations_type(const TypedArray<CraftStationType> &new_stations_type) {
	stations_type = new_stations_type;
}

TypedArray<CraftStationType> InventoryDatabase::get_stations_type() const {
	return stations_type;
}

void InventoryDatabase::set_item_categories(const TypedArray<ItemCategory> &new_item_categories) {
	item_categories = new_item_categories;
	_update_items_categories_cache();
}

TypedArray<ItemCategory> InventoryDatabase::get_item_categories() const {
	return item_categories;
}

void InventoryDatabase::set_items_cache(const Dictionary &new_items_cache) {
	items_cache = new_items_cache;
}

Dictionary InventoryDatabase::get_items_cache() const {
	return items_cache;
}

void InventoryDatabase::set_categories_code_cache(const Dictionary &new_categories_code_cache) {
	categories_code_cache = new_categories_code_cache;
}

Dictionary InventoryDatabase::get_categories_code_cache() const {
	return categories_code_cache;
}

void InventoryDatabase::add_new_item(const Ref<ItemDefinition> item) {
	items.append(item);
	_update_items_cache();
}

void InventoryDatabase::remove_item(const Ref<ItemDefinition> item) {
	int index = items.find(item);
	if (index > -1) {
		items.remove_at(index);
		_update_items_cache();
	}
}

void InventoryDatabase::add_new_category(const Ref<ItemCategory> category) {
	item_categories.append(category);
	_update_items_categories_cache();
}

void InventoryDatabase::remove_category(const Ref<ItemCategory> category) {
	int index = item_categories.find(category);
	if (index > -1) {
		item_categories.remove_at(index);
		_update_items_categories_cache();
	}
}

Ref<ItemDefinition> InventoryDatabase::get_item(int id) const {
	if (has_item_id(id)) {
		return items_cache[id];
	}
	return nullptr;
}

bool InventoryDatabase::has_item_id(int id) const {
	return items_cache.has(id);
}

int InventoryDatabase::get_valid_id() const {
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemDefinition> item = items[i];
		return item->get_id();
	}
	return -1;
}

int InventoryDatabase::get_new_valid_id() const {
	for (size_t i = 0; i < 92233720368547758; i++) {
		if (!has_item_id(i)) {
			return i;
		}
	}
	return -1;
}

Ref<ItemCategory> InventoryDatabase::get_category(int code) {
	return categories_code_cache[code];
}
