#include "inventory_database.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/variant.hpp>

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
	ClassDB::bind_method(D_METHOD("has_item_category_id", "id"), &InventoryDatabase::has_item_category_id);
	ClassDB::bind_method(D_METHOD("has_item_id", "id"), &InventoryDatabase::has_item_id);
	ClassDB::bind_method(D_METHOD("has_craft_station_type_id", "id"), &InventoryDatabase::has_craft_station_type_id);
	ClassDB::bind_method(D_METHOD("get_valid_id"), &InventoryDatabase::get_valid_id);
	ClassDB::bind_method(D_METHOD("get_new_valid_id"), &InventoryDatabase::get_new_valid_id);
	ClassDB::bind_method(D_METHOD("get_category", "code"), &InventoryDatabase::get_category);
	ClassDB::bind_method(D_METHOD("serialize_item_definition", "definition"), &InventoryDatabase::serialize_item_definition);
	ClassDB::bind_method(D_METHOD("deserialize_item_definition", "definition", "data"), &InventoryDatabase::deserialize_item_definition);
	ClassDB::bind_method(D_METHOD("serialize_item_category", "category"), &InventoryDatabase::serialize_item_category);
	ClassDB::bind_method(D_METHOD("deserialize_item_category", "category", "data"), &InventoryDatabase::deserialize_item_category);
	ClassDB::bind_method(D_METHOD("serialize_recipe", "recipe"), &InventoryDatabase::serialize_recipe);
	ClassDB::bind_method(D_METHOD("deserialize_recipe", "recipe", "data"), &InventoryDatabase::deserialize_recipe);
	ClassDB::bind_method(D_METHOD("serialize_station_type", "station_type"), &InventoryDatabase::serialize_station_type);
	ClassDB::bind_method(D_METHOD("deserialize_station_type", "station_type", "data"), &InventoryDatabase::deserialize_station_type);
	ClassDB::bind_method(D_METHOD("serialize_slot", "slot"), &InventoryDatabase::serialize_slot);
	ClassDB::bind_method(D_METHOD("deserialize_slot", "slot", "data"), &InventoryDatabase::deserialize_slot);
	ClassDB::bind_method(D_METHOD("serialize_slots", "slots"), &InventoryDatabase::serialize_slots);
	ClassDB::bind_method(D_METHOD("deserialize_slots", "slots", "data"), &InventoryDatabase::deserialize_slots);

	ClassDB::bind_method(D_METHOD("get_category_from_id", "id"), &InventoryDatabase::get_category_from_id);

	ClassDB::bind_method(D_METHOD("add_item"), &InventoryDatabase::add_item);
	ClassDB::bind_method(D_METHOD("add_item_category"), &InventoryDatabase::add_item_category);
	ClassDB::bind_method(D_METHOD("add_recipe"), &InventoryDatabase::add_recipe);
	ClassDB::bind_method(D_METHOD("add_craft_station_type"), &InventoryDatabase::add_craft_station_type);

	ClassDB::bind_method(D_METHOD("export_to_invdata"), &InventoryDatabase::export_to_invdata);
	ClassDB::bind_method(D_METHOD("import_from_invdata", "json"), &InventoryDatabase::import_from_invdata);
	ClassDB::bind_method(D_METHOD("clear_current_data"), &InventoryDatabase::clear_current_data);
	ClassDB::bind_method(D_METHOD("import_json_file", "path"), &InventoryDatabase::import_json_file);
	ClassDB::bind_method(D_METHOD("export_json_file", "path"), &InventoryDatabase::export_json_file);

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

Ref<ItemDefinition> InventoryDatabase::get_item(String id) const {
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemDefinition> item = items[i];
		if (item->get_id() == id)
			return item;
	}
	return nullptr;
}

bool InventoryDatabase::has_item_category_id(String id) const {
	for (size_t i = 0; i < item_categories.size(); i++) {
		Ref<ItemCategory> category = item_categories[i];
		if (category->get_id() == id)
			return true;
	}
	return false;
}

bool InventoryDatabase::has_item_id(String id) const {
	return items_cache.has(id);
}

bool InventoryDatabase::has_craft_station_type_id(String id) const {
	for (size_t i = 0; i < stations_type.size(); i++) {
		Ref<CraftStationType> craft_station_type = stations_type[i];
		if (craft_station_type->get_id() == id)
			return true;
	}
	return false;
}

String InventoryDatabase::get_valid_id() const {
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemDefinition> item = items[i];
		return item->get_id();
	}
	return "";
}

String InventoryDatabase::get_new_valid_id() const {
	return "";
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
	if (definition->get_icon() != nullptr) {
		data["icon"] = definition->get_icon()->get_path();
	}
	data["weight"] = definition->get_weight();
	if (!definition->get_properties().is_empty())
		data["properties"] = definition->get_properties();
	if (!definition->get_dynamic_properties().is_empty())
		data["dynamic_properties"] = definition->get_dynamic_properties();
	if (!definition->get_categories().is_empty()) {
		TypedArray<String> categories_ids = TypedArray<String>();
		TypedArray<ItemCategory> categories = definition->get_categories();
		for (size_t category_index = 0; category_index < categories.size(); category_index++) {
			Ref<ItemCategory> category = categories[category_index];
			categories_ids.append(category->get_id());
		}
		data["categories"] = categories_ids;
	}
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
	if (data.has("icon")) {
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(data["icon"]);
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
	if (data.has("categories")) {
		TypedArray<ItemCategory> categories = TypedArray<ItemCategory>();
		TypedArray<String> categories_names = data["categories"];
		for (size_t category_index = 0; category_index < categories_names.size(); category_index++) {
			String category_id = categories_names[category_index];
			for (size_t i = 0; i < item_categories.size(); i++) {
				Ref<ItemCategory> category = item_categories[i];
				if (category->get_id() == category_id) {
					categories.append(category);
				}
			}
		}
		definition->set_categories(categories);
	}
}

Dictionary InventoryDatabase::serialize_item_category(const Ref<ItemCategory> category) const {
	Dictionary data = Dictionary();
	data["id"] = category->get_id();
	data["name"] = category->get_name();
	if (category->get_icon() != nullptr) {
		data["icon"] = category->get_icon()->get_path();
	}
	data["color"] = category->get_color().to_html();
	if (!category->get_item_properties().is_empty())
		data["item_properties"] = category->get_item_properties();
	if (!category->get_item_dynamic_properties().is_empty())
		data["item_dynamic_properties"] = category->get_item_dynamic_properties();
	return data;
}

void InventoryDatabase::deserialize_item_category(Ref<ItemCategory> category, const Dictionary data) const {
	if (data.has("id")) {
		category->set_id(data["id"]);
	}
	if (data.has("name")) {
		category->set_name(data["name"]);
	}
	if (data.has("icon")) {
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(data["icon"]);
		category->set_icon(icon);
	}
	if (data.has("color") && Color::html_is_valid(data["color"])) {
		category->set_color(Color::html(data["color"]));
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
	if (!recipe->get_products().is_empty())
		data["products"] = serialize_item_stacks(recipe->get_products());
	data["time_to_craft"] = recipe->get_time_to_craft();
	if (!recipe->get_ingredients().is_empty())
		data["ingredients"] = serialize_item_stacks(recipe->get_ingredients());
	if (!recipe->get_required_items().is_empty())
		data["required_items"] = serialize_item_stacks(recipe->get_required_items());
	if (recipe->get_station() != nullptr)
		data["craft_station_type"] = recipe->get_station()->get_id();

	return data;
}

void InventoryDatabase::deserialize_recipe(Ref<Recipe> recipe, const Dictionary data) const {
	if (data.has("products")) {
		TypedArray<ItemStack> item_stacks = recipe->get_products();
		deserialize_item_stacks(item_stacks, data["products"]);
	}
	if (data.has("time_to_craft")) {
		recipe->set_time_to_craft(data["time_to_craft"]);
	}
	if (data.has("ingredients")) {
		TypedArray<ItemStack> item_stacks = recipe->get_ingredients();
		deserialize_item_stacks(item_stacks, data["ingredients"]);
	}
	if (data.has("required_items")) {
		TypedArray<ItemStack> item_stacks = recipe->get_required_items();
		deserialize_item_stacks(item_stacks, data["required_items"]);
	}
	if (data.has("craft_station_type")) {
		Ref<CraftStationType> craft_station_type = get_craft_station_from_id(data["craft_station_type"]);
		recipe->set_station(craft_station_type);
	}
}

Dictionary InventoryDatabase::serialize_station_type(const Ref<CraftStationType> craft_station_type) const {
	Dictionary data = Dictionary();
	data["id"] = craft_station_type->get_id();
	data["name"] = craft_station_type->get_name();
	if (craft_station_type->get_icon() != nullptr) {
		data["icon"] = craft_station_type->get_icon()->get_path();
	}
	return data;
}

void InventoryDatabase::deserialize_station_type(Ref<CraftStationType> craft_station_type, const Dictionary data) const {
	if (data.has("id")) {
		craft_station_type->set_id(data["id"]);
	}
	if (data.has("name")) {
		craft_station_type->set_name(data["name"]);
	}
	if (data.has("icon")) {
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(data["icon"]);
		craft_station_type->set_icon(icon);
	}
}

String InventoryDatabase::serialize_slot(const Ref<Slot> slot) const {
	String data = String();
	Ref<Item> item = slot->get_item();
	if (item != nullptr) {
		Dictionary item_data = Dictionary();
		if (item->get_definition() == nullptr) {
			data += "";
		} else {
			data += item->get_definition()->get_id();
		}
		// item_data["properties"] = item->get_properties();
		// data["item"] = item_data;
	}
	data += " ";
	data += String::num_int64(slot->get_amount());
	// data["categorized"] = slot->is_categorized();
	return data;
}

void InventoryDatabase::deserialize_slot(Ref<Slot> slot, const String data) const {
	PackedStringArray array = data.split(" ");
	ERR_FAIL_COND_MSG(array.size() < 2, "Data to deserialize slot is invalid: Does not contain the 'amount' field");
	// ERR_FAIL_COND_MSG(!data.has("categorized"), "Data to deserialize slot is invalid: Does not contain the 'categorized' field");
	if (!array[0].is_empty()) {
		// Dictionary item_data = data["item"];
		// ERR_FAIL_COND_MSG(!item_data.has("id"), "Data to deserialize slot is invalid: Does not contain the 'id' field");
		// ERR_FAIL_COND_MSG(!item_data.has("properties"), "Data to deserialize slot is invalid: Does not contain the 'properties' field");
		Ref<Item> item = slot->get_item();
		if (item == nullptr) {
			item.instantiate();
		}
		if (!array[0].is_empty()) {
			item->set_definition(get_item(array[0]));
		}
		// item->set_properties(item_data["properties"]);
		slot->set_item(item);
	}
	if (!array[1].is_empty()) {
		slot->set_amount(array[1].to_int());
	}
	// slot->set_categorized(data["categorized"]);
}

Array InventoryDatabase::serialize_slots(const TypedArray<Slot> slots) const {
	Array slots_data = Array();
	for (size_t slot_index = 0; slot_index < slots.size(); slot_index++) {
		String data = serialize_slot(slots[slot_index]);
		slots_data.append(data);
	}
	return slots_data;
}

void InventoryDatabase::deserialize_slots(TypedArray<Slot> slots, const Array data) const {
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

Array InventoryDatabase::serialize_item_stacks(const TypedArray<ItemStack> item_stacks) const {
	Array slots_data = Array();
	for (size_t item_stack_index = 0; item_stack_index < item_stacks.size(); item_stack_index++) {
		Ref<ItemStack> item_stack = item_stacks[item_stack_index];
		String data = item_stack->serialize();
		slots_data.append(data);
	}
	return slots_data;
}

void InventoryDatabase::deserialize_item_stacks(TypedArray<ItemStack> item_stacks, const Array data) const {
	for (size_t item_stack_index = 0; item_stack_index < data.size(); item_stack_index++) {
		if (item_stack_index >= item_stacks.size()) {
			Ref<ItemStack> item_stack = memnew(ItemStack());
			item_stack->deserialize(data[item_stack_index]);
			item_stacks.append(item_stack);
		} else {
			Ref<ItemStack> item_stack = item_stacks[item_stack_index];
			item_stack->deserialize(data[item_stack_index]);
		}
	}
	int size = item_stacks.size();
	for (size_t item_stack_index = data.size(); item_stack_index < size; item_stack_index++) {
		item_stacks.remove_at(data.size());
	}
}

void InventoryDatabase::add_item() {
	Ref<ItemDefinition> definition = memnew(ItemDefinition());
	items.append(definition);
}

void InventoryDatabase::add_item_category() {
	Ref<ItemCategory> category = memnew(ItemCategory());
	item_categories.append(category);
}

void InventoryDatabase::add_recipe() {
	Ref<Recipe> recipe = memnew(Recipe());
	recipes.append(recipe);
}

void InventoryDatabase::add_craft_station_type() {
	Ref<CraftStationType> craft_station_type = memnew(CraftStationType());
	stations_type.append(craft_station_type);
}

Ref<ItemCategory> InventoryDatabase::get_category_from_id(String id) const {
	for (size_t category_index = 0; category_index < item_categories.size(); category_index++) {
		Ref<ItemCategory> category = item_categories[category_index];
		if (category->get_id() == id)
			return category;
	}
	return nullptr;
}

Ref<CraftStationType> InventoryDatabase::get_craft_station_from_id(String id) const {
	for (size_t i = 0; i < stations_type.size(); i++) {
		Ref<CraftStationType> station = stations_type[i];
		if (station->get_id() == id)
			return station;
	}
	return nullptr;
}

Dictionary InventoryDatabase::serialize() const {
	Dictionary data = Dictionary();
	Array item_categories_data = serialize_item_categories();
	if (!item_categories_data.is_empty())
		data["item_categories"] = item_categories_data;
	Array items_data = serialize_items();
	if (!items_data.is_empty())
		data["items"] = items_data;
	Array recipes_data = serialize_recipes();
	if (!recipes_data.is_empty())
		data["recipes"] = recipes_data;
	Array craft_station_types_data = serialize_craft_station_types();
	if (!craft_station_types_data.is_empty())
		data["craft_station_types"] = craft_station_types_data;
	return data;
}

void InventoryDatabase::deserialize(const Dictionary data) {
	if (data.has("item_categories")) {
		deserialize_item_categories(data["item_categories"]);
	}
	if (data.has("items")) {
		deserialize_items(data["items"]);
	}
	_update_items_cache();
	if (data.has("craft_station_types")) {
		deserialize_craft_station_types(data["craft_station_types"]);
	}
	if (data.has("recipes")) {
		deserialize_recipes(data["recipes"]);
	}
}

Array InventoryDatabase::serialize_items() const {
	Array items_data = Array();
	for (size_t i = 0; i < this->items.size(); i++) {
		Ref<ItemDefinition> definition = this->items[i];
		if (definition == nullptr)
			continue;
		Dictionary item_data = serialize_item_definition(definition);
		items_data.append(item_data);
	}
	return items_data;
}

void InventoryDatabase::deserialize_items(Array datas) {
	for (size_t i = 0; i < datas.size(); i++) {
		Ref<ItemDefinition> definition = memnew(ItemDefinition());
		deserialize_item_definition(definition, datas[i]);
		items.append(definition);
	}
}

Array InventoryDatabase::serialize_item_categories() const {
	Array categories_data = Array();
	for (size_t i = 0; i < this->item_categories.size(); i++) {
		Ref<ItemCategory> category = this->item_categories[i];
		if (category == nullptr)
			continue;
		Dictionary data = serialize_item_category(category);
		categories_data.append(data);
	}
	return categories_data;
}

void InventoryDatabase::deserialize_item_categories(Array datas) {
	for (size_t i = 0; i < datas.size(); i++) {
		Ref<ItemCategory> category = memnew(ItemCategory());
		deserialize_item_category(category, datas[i]);
		item_categories.append(category);
	}
}

Array InventoryDatabase::serialize_craft_station_types() const {
	Array datas = Array();
	for (size_t i = 0; i < this->stations_type.size(); i++) {
		Ref<CraftStationType> station = this->stations_type[i];
		if (station == nullptr)
			continue;
		Dictionary data = serialize_station_type(station);
		datas.append(data);
	}
	return datas;
}

void InventoryDatabase::deserialize_craft_station_types(Array datas) {
	for (size_t i = 0; i < datas.size(); i++) {
		Ref<CraftStationType> station = memnew(CraftStationType());
		deserialize_station_type(station, datas[i]);
		stations_type.append(station);
	}
}

Array InventoryDatabase::serialize_recipes() const {
	Array datas = Array();
	for (size_t i = 0; i < this->recipes.size(); i++) {
		Ref<Recipe> recipe = this->recipes[i];
		if (recipe == nullptr)
			continue;
		Dictionary data = serialize_recipe(recipe);
		datas.append(data);
	}
	return datas;
}

void InventoryDatabase::deserialize_recipes(Array datas) {
	for (size_t i = 0; i < datas.size(); i++) {
		Ref<Recipe> recipe = memnew(Recipe());
		deserialize_recipe(recipe, datas[i]);
		recipes.append(recipe);
	}
}

void InventoryDatabase::clear_current_data() {
	items.clear();
	item_categories.clear();
	stations_type.clear();
	recipes.clear();
}

String InventoryDatabase::export_to_invdata() const {
	Dictionary data = serialize();
	String json = JSON::stringify(data, "\t");
	return json;
}

void InventoryDatabase::import_from_invdata(const String json) {
	Dictionary data = JSON::parse_string(json);
	deserialize(data);
}

Error InventoryDatabase::import_json_file(const String path) {
	ERR_FAIL_COND_V_MSG(path.is_empty(), Error::ERR_INVALID_PARAMETER, "'path' is empty.");
	Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
	if (file == nullptr) {
		return FileAccess::get_open_error();
	}

	String json = "";
	while (file->get_position() < file->get_length())
		json += file->get_line() + "\n";

	import_from_invdata(json);
	return Error::OK;
}

Error InventoryDatabase::export_json_file(const String path) {
	ERR_FAIL_COND_V_MSG(path.is_empty(), Error::ERR_INVALID_PARAMETER, "'path' is empty.");
	Ref<FileAccess> file = FileAccess::open(path, FileAccess::WRITE);
	if (file == nullptr) {
		return FileAccess::get_open_error();
	}
	String json = export_to_invdata();
	file->store_string(json);
	file->close();
	return Error::OK;
}
