#include "inventory_database.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
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
	ClassDB::bind_method(D_METHOD("serialize_item_definition", "definition"), &InventoryDatabase::serialize_item_definition);
	ClassDB::bind_method(D_METHOD("deserialize_item_definition", "definition", "data"), &InventoryDatabase::deserialize_item_definition);
	ClassDB::bind_method(D_METHOD("serialize_item_category", "category"), &InventoryDatabase::serialize_item_category);
	ClassDB::bind_method(D_METHOD("deserialize_item_category", "category", "data"), &InventoryDatabase::deserialize_item_category);
	ClassDB::bind_method(D_METHOD("serialize_slot", "slot"), &InventoryDatabase::serialize_slot);
	ClassDB::bind_method(D_METHOD("deserialize_slot", "slot", "data"), &InventoryDatabase::deserialize_slot);

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
	ERR_FAIL_NULL_MSG(category, "'category' is null.");

	item_categories.append(category);
	_update_items_categories_cache();
}

void InventoryDatabase::remove_category(const Ref<ItemCategory> category) {
	ERR_FAIL_NULL_MSG(category, "'category' is null.");

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

Dictionary InventoryDatabase::serialize_item_definition(const Ref<ItemDefinition> definition) const {
	Dictionary data = Dictionary();
	data["id"] = definition->get_id();
	data["can_stack"] = definition->get_can_stack();
	data["max_stack"] = definition->get_max_stack();
	data["name"] = definition->get_name();
	data["icon_path"] = definition->get_icon()->get_path();
	data["weight"] = definition->get_weight();
	data["properties"] = definition->get_properties();
	data["dynamic_properties"] = definition->get_dynamic_properties();
	TypedArray<String> categories_path = TypedArray<String>();
	TypedArray<ItemCategory> categories = definition->get_categories();
	for (size_t category_index = 0; category_index < categories.size(); category_index++) {
		Ref<ItemCategory> category = categories[category_index];
		categories_path.append(category->get_path());
	}
	data["categories_path"] = categories_path;
	return data;
}

void InventoryDatabase::deserialize_item_definition(Ref<ItemDefinition> definition, const Dictionary data) const {
	if (data.has("id")) {
		definition->set_id(data["id"]);
	}
	if (data.has("can_stack")) {
		definition->set_can_stack(data["can_stack"]);
	}
	if (data.has("max_stack")) {
		definition->set_max_stack(data["max_stack"]);
	}
	if (data.has("name")) {
		definition->set_name(data["name"]);
	}
	if (data.has("icon_path")) {
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(data["icon_path"]);
		definition->set_icon(icon);
	}
	if (data.has("weight")) {
		definition->set_weight(data["weight"]);
	}
	if (data.has("properties")) {
		definition->set_properties(data["properties"]);
	}
	if (data.has("dynamic_properties")) {
		definition->set_dynamic_properties(data["dynamic_properties"]);
	}
	if (data.has("categories_path")) {
		TypedArray<ItemCategory> categories = TypedArray<ItemCategory>();
		TypedArray<String> categories_path = data["categories_path"];
		for (size_t category_index = 0; category_index < categories_path.size(); category_index++) {
			Ref<ItemCategory> category = ResourceLoader::get_singleton()->load(categories_path[category_index]);
			categories.append(category);
		}
		definition->set_categories(categories);
	}
}

Dictionary InventoryDatabase::serialize_item_category(const Ref<ItemCategory> category) const {
	Dictionary data = Dictionary();
	data["name"] = category->get_name();
	data["icon_path"] = category->get_icon()->get_path();
	data["color"] = category->get_color();
	data["code"] = category->get_code();
	data["item_properties"] = category->get_item_properties();
	data["item_dynamic_properties"] = category->get_item_dynamic_properties();
	return data;
}

void InventoryDatabase::deserialize_item_category(Ref<ItemCategory> category, const Dictionary data) const {
	if (data.has("name")) {
		category->set_name(data["name"]);
	}
	if (data.has("icon_path")) {
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(data["icon_path"]);
		category->set_icon(icon);
	}
	if (data.has("color")) {
		category->set_color(data["color"]);
	}
	if (data.has("code")) {
		category->set_code(data["code"]);
	}
	if (data.has("item_properties")) {
		category->set_item_properties(data["item_properties"]);
	}
	if (data.has("item_dynamic_properties")) {
		category->set_item_dynamic_properties(data["item_dynamic_properties"]);
	}
}

Dictionary InventoryDatabase::serialize_recipe(const Ref<Recipe> recipe) const {
	Dictionary data = Dictionary();
	data["products"] = serialize_slots(recipe->get_products());
	data["time_to_craft"] = recipe->get_time_to_craft();
	data["ingredients"] = serialize_slots(recipe->get_ingredients());
	data["required_items"] = serialize_slots(recipe->get_required_items());
	return data;
}

void InventoryDatabase::deserialize_recipe(Ref<Recipe> recipe, const Dictionary data) const {
	if (data.has("products")) {
		TypedArray<Slot> slots = recipe->get_products();
		deserialize_slots(slots, data["products"]);
	}
	if (data.has("time_to_craft")) {
		recipe->set_time_to_craft(data["time_to_craft"]);
	}
	if (data.has("ingredients")) {
		TypedArray<Slot> slots = recipe->get_ingredients();
		deserialize_slots(slots, data["ingredients"]);
	}
	if (data.has("required_items")) {
		TypedArray<Slot> slots = recipe->get_required_items();
		deserialize_slots(slots, data["required_items"]);
	}
}

Dictionary InventoryDatabase::serialize_slot(const Ref<Slot> slot) const {
	Dictionary data = Dictionary();
	Ref<Item> item = slot->get_item();
	if (item != nullptr) {
		Dictionary item_data = Dictionary();
		if (item->get_definition() == nullptr) {
			item_data["id"] = ItemDefinition::NONE;
		} else {
			item_data["id"] = item->get_definition()->get_id();
		}
		item_data["properties"] = item->get_properties();
		data["item"] = item_data;
	}
	data["amount"] = slot->get_amount();
	// data["categorized"] = slot->is_categorized();
	return data;
}

void InventoryDatabase::deserialize_slot(Ref<Slot> slot, const Dictionary data) const {
	ERR_FAIL_COND_MSG(!data.has("amount"), "Data to deserialize slot is invalid: Does not contain the 'amount' field");
	// ERR_FAIL_COND_MSG(!data.has("categorized"), "Data to deserialize slot is invalid: Does not contain the 'categorized' field");
	if (data.has("item")) {
		Dictionary item_data = data["item"];
		ERR_FAIL_COND_MSG(!item_data.has("id"), "Data to deserialize slot is invalid: Does not contain the 'id' field");
		ERR_FAIL_COND_MSG(!item_data.has("properties"), "Data to deserialize slot is invalid: Does not contain the 'properties' field");
		Ref<Item> item = slot->get_item();
		if (item == nullptr) {
			item.instantiate();
		}
		item->set_definition(get_item(item_data["id"]));
		item->set_properties(item_data["properties"]);
		slot->set_item(item);
	}
	slot->set_amount(data["amount"]);
	// slot->set_categorized(data["categorized"]);
}

TypedArray<Dictionary> InventoryDatabase::serialize_slots(const TypedArray<Slot> slots) const {
	TypedArray<Dictionary> slots_data = TypedArray<Dictionary>();;
	for (size_t slot_index = 0; slot_index < slots.size(); slot_index++) {
		Dictionary data = serialize_slot(slots[slot_index]);
		slots_data.append(data);
	}
	return slots_data;
}

void InventoryDatabase::deserialize_slots(TypedArray<Slot> slots, const TypedArray<Dictionary> data) const {
	for (size_t slot_index = 0; slot_index < data.size(); slot_index++) {
		if (slot_index >= slots.size()) {
			Ref<Slot> slot = memnew(Slot());
			deserialize_slot(slot, data[slot_index]);
			slots.append(slot);
		} else {
			Ref<Slot> slot = slots[slot_index];
			deserialize_slot(slot, data[slot_index]);
		}
	}
	int size = slots.size();
	for (size_t slot_index = data.size(); slot_index < size; slot_index++) {
		slots.remove_at(data.size());
	}
}
