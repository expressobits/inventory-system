#include "craft_station.h"
#include <godot_cpp/classes/engine.hpp>

void Crafting::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_recipe_index", "recipe_index"), &Crafting::set_recipe_index);
	ClassDB::bind_method(D_METHOD("get_recipe_index"), &Crafting::get_recipe_index);
	ClassDB::bind_method(D_METHOD("set_time", "time"), &Crafting::set_time);
	ClassDB::bind_method(D_METHOD("get_time"), &Crafting::get_time);
	ClassDB::bind_method(D_METHOD("is_finished"), &Crafting::is_finished);
	ClassDB::bind_method(D_METHOD("process", "delta"), &Crafting::process);
	ClassDB::bind_method(D_METHOD("serialize"), &Crafting::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &Crafting::deserialize);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "recipe_index"), "set_recipe_index", "get_recipe_index");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
}

void Crafting::set_recipe_index(const int &new_recipe) {
	recipe_index = new_recipe;
}

int Crafting::get_recipe_index() const {
	return recipe_index;
}

void Crafting::set_time(const float &new_time) {
	time = new_time;
}

float Crafting::get_time() const {
	return time;
}

bool Crafting::is_finished() const {
	return time <= 0;
}

void Crafting::process(float delta) {
	time -= delta;
}

Dictionary Crafting::serialize() const {
	Dictionary dict = Dictionary();
	dict["recipe_index"] = recipe_index;
	dict["time"] = time;
	return dict;
}

void Crafting::deserialize(Dictionary data) {
	recipe_index = data["recipe_index"];
	time = data["time"];
}

CraftStation::CraftStation() {
}

CraftStation::~CraftStation() {
}

void CraftStation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tick", "delta"), &CraftStation::tick);
	ClassDB::bind_method(D_METHOD("is_crafting"), &CraftStation::is_crafting);
	ClassDB::bind_method(D_METHOD("can_craft", "recipe"), &CraftStation::can_craft);
	ClassDB::bind_method(D_METHOD("contains_ingredients", "recipe"), &CraftStation::contains_ingredients);
	ClassDB::bind_method(D_METHOD("craft", "recipe_index"), &CraftStation::craft);
	ClassDB::bind_method(D_METHOD("cancel_craft", "crafting_index"), &CraftStation::cancel_craft);
	ClassDB::bind_method(D_METHOD("add_crafting", "recipe_index", "recipe"), &CraftStation::add_crafting);
	ClassDB::bind_method(D_METHOD("remove_crafting", "crafting_index"), &CraftStation::remove_crafting);
	ClassDB::bind_method(D_METHOD("finish_crafting", "crafting_index"), &CraftStation::finish_crafting);
	ClassDB::bind_method(D_METHOD("get_input_inventory", "index"), &CraftStation::get_input_inventory, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_input_inventories", "input_inventories"), &CraftStation::set_input_inventories);
	ClassDB::bind_method(D_METHOD("get_input_inventories"), &CraftStation::get_input_inventories);
	ClassDB::bind_method(D_METHOD("set_output_inventories", "output_inventories"), &CraftStation::set_output_inventories);
	ClassDB::bind_method(D_METHOD("get_output_inventories"), &CraftStation::get_output_inventories);
	ClassDB::bind_method(D_METHOD("set_has_limit_crafts", "has_limit_crafts"), &CraftStation::set_has_limit_crafts);
	ClassDB::bind_method(D_METHOD("get_has_limit_crafts"), &CraftStation::get_has_limit_crafts);
	ClassDB::bind_method(D_METHOD("set_limit_number_crafts", "limit_number_crafts"), &CraftStation::set_limit_number_crafts);
	ClassDB::bind_method(D_METHOD("get_limit_number_crafts"), &CraftStation::get_limit_number_crafts);
	ClassDB::bind_method(D_METHOD("set_can_processing_craftings", "can_processing_craftings"), &CraftStation::set_can_processing_craftings);
	ClassDB::bind_method(D_METHOD("get_can_processing_craftings"), &CraftStation::get_can_processing_craftings);
	ClassDB::bind_method(D_METHOD("set_can_finish_craftings", "can_finish_craftings"), &CraftStation::set_can_finish_craftings);
	ClassDB::bind_method(D_METHOD("get_can_finish_craftings"), &CraftStation::get_can_finish_craftings);
	ClassDB::bind_method(D_METHOD("set_type", "type"), &CraftStation::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &CraftStation::get_type);
	ClassDB::bind_method(D_METHOD("set_only_remove_ingredients_after_craft", "only_remove_ingredients_after_craft"), &CraftStation::set_only_remove_ingredients_after_craft);
	ClassDB::bind_method(D_METHOD("get_only_remove_ingredients_after_craft"), &CraftStation::get_only_remove_ingredients_after_craft);
	ClassDB::bind_method(D_METHOD("set_auto_craft", "auto_craft"), &CraftStation::set_auto_craft);
	ClassDB::bind_method(D_METHOD("get_auto_craft"), &CraftStation::get_auto_craft);
	ClassDB::bind_method(D_METHOD("set_processing_mode", "processing_mode"), &CraftStation::set_processing_mode);
	ClassDB::bind_method(D_METHOD("get_processing_mode"), &CraftStation::get_processing_mode);
	ClassDB::bind_method(D_METHOD("set_tick_update_method", "tick_update_method"), &CraftStation::set_tick_update_method);
	ClassDB::bind_method(D_METHOD("get_tick_update_method"), &CraftStation::get_tick_update_method);
	ClassDB::bind_method(D_METHOD("set_craftings", "craftings"), &CraftStation::set_craftings);
	ClassDB::bind_method(D_METHOD("get_craftings"), &CraftStation::get_craftings);
	ClassDB::bind_method(D_METHOD("set_valid_recipes", "valid_recipes"), &CraftStation::set_valid_recipes);
	ClassDB::bind_method(D_METHOD("get_valid_recipes"), &CraftStation::get_valid_recipes);
	ClassDB::bind_method(D_METHOD("add_input_inventory", "input_inventory"), &CraftStation::add_input_inventory);
	ClassDB::bind_method(D_METHOD("remove_input_inventory", "input_inventory"), &CraftStation::remove_input_inventory);
	ADD_SIGNAL(MethodInfo("on_crafted", PropertyInfo(Variant::INT, "recipe_index")));
	ADD_SIGNAL(MethodInfo("on_request_craft", PropertyInfo(Variant::INT, "recipe_index")));
	ADD_SIGNAL(MethodInfo("crafting_added", PropertyInfo(Variant::INT, "crafting_index")));
	ADD_SIGNAL(MethodInfo("crafting_removed", PropertyInfo(Variant::INT, "crafting_index")));
	ADD_SIGNAL(MethodInfo("input_inventory_added", PropertyInfo(Variant::NODE_PATH, "input_inventory_path")));
	ADD_SIGNAL(MethodInfo("input_inventory_removed", PropertyInfo(Variant::NODE_PATH, "input_inventory_path")));
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "input_inventories", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory")), "set_input_inventories", "get_input_inventories");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "output_inventories", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory")), "set_output_inventories", "get_output_inventories");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_limit_crafts"), "set_has_limit_crafts", "get_has_limit_crafts");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_number_crafts"), "set_limit_number_crafts", "get_limit_number_crafts");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_processing_craftings"), "set_can_processing_craftings", "get_can_processing_craftings");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_finish_craftings"), "set_can_finish_craftings", "get_can_finish_craftings");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "type", PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "only_remove_ingredients_after_craft"), "set_only_remove_ingredients_after_craft", "get_only_remove_ingredients_after_craft");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_craft"), "set_auto_craft", "get_auto_craft");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "processing_mode", PROPERTY_HINT_ENUM, "Parallel,Sequential"), "set_processing_mode", "get_processing_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_update_method", PROPERTY_HINT_ENUM, "Process,Physic Process,Custom"), "set_tick_update_method", "get_tick_update_method");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "craftings", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Crafting")), "set_craftings", "get_craftings");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "valid_recipes"), "set_valid_recipes", "get_valid_recipes");
}

void CraftStation::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == StringName("limit_number_crafts")) {
		if (!has_limit_crafts) {
			p_property.usage = PROPERTY_USAGE_NO_EDITOR;
		}
	}
	if (p_property.name == StringName("craftings")) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
	if (p_property.name == StringName("valid_recipes")) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void CraftStation::_process_crafts(float delta) {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	for (size_t i = 0; i < craftings.size(); i++) {
		if (processing_mode == ProcessingMode::SEQUENTIAL && i > 0)
			return;
		Ref<Crafting> c = craftings[i];
		if (!c->is_finished()) {
			c->process(delta);
		}
	}
}

void CraftStation::finish_crafting(int crafting_index) {
	ERR_FAIL_COND_MSG(get_database() == nullptr, "Database is null.");
	ERR_FAIL_COND_MSG(crafting_index < 0 || crafting_index >= craftings.size(), "Crafting Index incorrect!");
	ERR_FAIL_COND_MSG(output_inventories.is_empty(), "Craft Station does not contain any output inventory.");

	Ref<Crafting> crafting = craftings[crafting_index];
	Ref<Recipe> recipe = get_database()->get_recipes()[crafting->get_recipe_index()];
	remove_crafting(crafting_index);
	if (only_remove_ingredients_after_craft) {
		if (!contains_ingredients(recipe)) {
			cancel_craft(crafting_index);
			return;
		}
		_use_items(recipe);
	}
	for (size_t i = 0; i < recipe->get_products().size(); i++) {
		Ref<Slot> product = recipe->get_products()[i];
		int amount_to_add = product->get_amount();
		for (size_t i = 0; i < output_inventories.size(); i++) {
			Inventory *inventory = get_output_inventory(i);
			if (inventory == nullptr) {
				ERR_PRINT("Passed object is not a Inventory!");
				return;
			}
			Ref<Item> item = product->get_item();
			item->create_dynamic_properties();
			amount_to_add = inventory->add(item, product->get_amount());
		}
	}
	emit_signal("on_crafted", crafting->get_recipe_index());
	_check_auto_crafts();
}

bool CraftStation::_use_items(const Ref<Recipe> &recipe) {
	ERR_FAIL_NULL_V_MSG(recipe, false, "'recipe' is null.");

	if (recipe->get_station() != type)
		return false;
	for (size_t i = 0; i < recipe->get_ingredients().size(); i++) {
		Ref<Slot> ingredient = recipe->get_ingredients()[i];
		int amount_to_remove = ingredient->get_amount();
		for (size_t j = 0; j < input_inventories.size(); j++) {
			Inventory *inventory = get_input_inventory(j);
			if (inventory == nullptr) {
				ERR_PRINT("Passed object is not a Inventory!");
				return false;
			}
			amount_to_remove = inventory->remove(ingredient->get_item(), amount_to_remove);
		}
		if (amount_to_remove > 0) {
			return false;
		}
	}
	return true;
}

void CraftStation::add_crafting(int recipe_index, const Ref<Recipe> &recipe) {
	ERR_FAIL_NULL_MSG(recipe, "'recipe' is null.");

	Ref<Crafting> crafting = memnew(Crafting());
	crafting->set_recipe_index(recipe_index);
	crafting->set_time(recipe->get_time_to_craft());
	craftings.append(crafting);
	emit_signal("crafting_added", craftings.size() - 1);
}

void CraftStation::remove_crafting(int crafting_index) {
	ERR_FAIL_COND_MSG(crafting_index < 0 || crafting_index >= craftings.size(), "Crafting Index incorrect!");

	craftings.remove_at(crafting_index);
	emit_signal("crafting_removed", crafting_index);
}

void CraftStation::_on_input_inventory_item_added(Ref<Item> item, int amount) {
	if (auto_craft)
		_check_auto_crafts();
}

void CraftStation::_on_input_inventory_item_removed(Ref<Item> item, int amount) {
	ERR_FAIL_COND_MSG(get_database() == nullptr, "Database is null.");
	if (only_remove_ingredients_after_craft) {
		int i = 0;
		while (i < craftings.size()) {
			Ref<Crafting> crafting = craftings[i];
			Ref<Recipe> recipe = get_database()->get_recipes()[crafting->get_recipe_index()];
			if (!contains_ingredients(recipe)) {
				cancel_craft(i);
				continue;
			}
			i++;
		}
	}
}

void CraftStation::_check_auto_crafts() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	if (!auto_craft)
		return;
	// ERR_FAIL_COND_MSG(get_database() == nullptr, "Database is null!");
	for (size_t i = 0; i < valid_recipes.size(); i++) {
		Ref<Recipe> recipe = get_database()->get_recipes()[valid_recipes[i]];
		if (!can_craft(recipe))
			continue;
		craft(valid_recipes[i]);
	}
}

void CraftStation::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	valid_recipes.clear();
	for (int i = 0; i < get_database()->get_recipes().size(); i++) {
		Ref<Recipe> recipe = get_database()->get_recipes()[i];
		if (recipe->get_station() == type) {
			valid_recipes.append(i);
		}
	}
	_setup_connections();
	NodeInventories::_ready();
}

void CraftStation::_setup_connections() {
	for (size_t i = 0; i < input_inventories.size(); i++) {
		Inventory *inventory = get_input_inventory(i);
		if (inventory == nullptr) {
			ERR_PRINT("Passed object is not a Inventory!");
			continue;
		}
		inventory->connect("item_added", callable_mp(this, &CraftStation::_on_input_inventory_item_added));
		inventory->connect("item_removed", callable_mp(this, &CraftStation::_on_input_inventory_item_removed));
	}
}

void CraftStation::tick(float delta) {
	if (!can_processing_craftings)
		return;
	if (!is_crafting())
		return;
	if (craftings.is_empty())
		return;
	_process_crafts(delta);
	for (size_t i = 0; i < craftings.size(); i++) {
		Ref<Crafting> crafting = craftings[i];
		if (crafting->is_finished() && can_finish_craftings) {
			finish_crafting(i);
			return;
		}
	}
}

void CraftStation::_process(float delta) {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	if (tick_update_method == TickUpdateMethod::PROCESS) {
		tick(delta);
	}
}

void CraftStation::_physic_process(float delta) {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	if (tick_update_method == TickUpdateMethod::PHYSIC_PROCESS) {
		tick(delta);
	}
}


bool CraftStation::is_crafting() const {
	return !craftings.is_empty();
}

bool CraftStation::can_craft(const Ref<Recipe> &recipe) const {
	ERR_FAIL_NULL_V_MSG(recipe, false, "'recipe' is null.");

	if (recipe->get_station() != type)
		return false;
	if (has_limit_crafts && limit_number_crafts <= craftings.size())
		return false;
	return contains_ingredients(recipe);
}

bool CraftStation::contains_ingredients(const Ref<Recipe> &recipe) const {
	ERR_FAIL_NULL_V_MSG(recipe, false, "'recipe' is null.");

	for (size_t i = 0; i < recipe->get_ingredients().size(); i++) {
		Ref<Slot> slot = recipe->get_ingredients()[i];
		int amount_total = 0;
		for (size_t j = 0; j < input_inventories.size(); j++) {
			Inventory *inventory = get_input_inventory(j);
			if (inventory == nullptr) {
				ERR_PRINT("Passed object is not a Inventory!");
				return false;
			}
			amount_total += inventory->amount_of_item(slot->get_item());
		}
		if (amount_total < slot->get_amount()) {
			return false;
		}
	}
	for (size_t i = 0; i < recipe->get_required_items().size(); i++) {
		Ref<Slot> slot = recipe->get_required_items()[i];
		int amount_total = 0;
		for (size_t j = 0; j < input_inventories.size(); j++) {
			Inventory *inventory = get_input_inventory(j);
			if (inventory == nullptr) {
				ERR_PRINT("Passed object is not a Inventory!");
				return false;
			}
			amount_total += inventory->amount_of_item(slot->get_item());
		}
		if (amount_total < slot->get_amount()) {
			return false;
		}
	}
	return true;
}

void CraftStation::craft(int recipe_index) {
	ERR_FAIL_COND_MSG(get_database() == nullptr, "Database is null.");
	ERR_FAIL_COND_MSG(get_database()->get_recipes().size() <= recipe_index || recipe_index < 0, "Recipe index is lower than the number of recipes in the database.");

	Ref<Recipe> recipe = get_database()->get_recipes()[recipe_index];
	emit_signal("on_request_craft", recipe_index);
	if (!can_craft(recipe))
		return;
	if (!only_remove_ingredients_after_craft && !_use_items(recipe))
		return;
	add_crafting(recipe_index, recipe);
}

void CraftStation::cancel_craft(int crafting_index) {
	ERR_FAIL_COND_MSG(get_database() == nullptr, "Database is null.");
	ERR_FAIL_COND_MSG(crafting_index < 0 || crafting_index >= craftings.size(), "Incorrect crafting index.");

	Ref<Crafting> crafting = craftings[crafting_index];
	Ref<Recipe> recipe = get_database()->get_recipes()[crafting->get_recipe_index()];
	if (!only_remove_ingredients_after_craft) {
		for (size_t i = 0; i < recipe->get_ingredients().size(); i++) {
			Ref<Slot> ingredient = recipe->get_ingredients()[i];
			Inventory *inventory = get_input_inventory(i);
			if (inventory == nullptr) {
				ERR_PRINT("Passed object is not a Inventory!");
				return;
			}
			inventory->add(ingredient->get_item(), ingredient->get_amount());
		}
	}
	remove_crafting(crafting_index);
}

Inventory *CraftStation::get_input_inventory(const int &index) const {
	ERR_FAIL_COND_V_MSG(index < 0 || index >= input_inventories.size(), nullptr, "input inventory index out of bounds.");

	Node *node_inv = get_node_or_null(input_inventories[index]);
	Inventory *inventory = Object::cast_to<Inventory>(node_inv);
	if (inventory == nullptr) {
		ERR_PRINT("Get Input Inventory Index is null!");
	}
	return inventory;
}

Inventory *CraftStation::get_output_inventory(const int &index) const {
	ERR_FAIL_COND_V_MSG(index < 0 || index >= input_inventories.size(), nullptr, "output inventory index out of bounds.");

	Node *node_inv = get_node_or_null(output_inventories[index]);
	Inventory *inventory = Object::cast_to<Inventory>(node_inv);
	if (inventory == nullptr) {
		ERR_PRINT("Get Input Inventory Index is null!");
	}
	return inventory;
}

void CraftStation::set_input_inventories(const TypedArray<NodePath> &new_input_inventories) {
	input_inventories = new_input_inventories;
}

TypedArray<NodePath> CraftStation::get_input_inventories() const {
	return input_inventories;
}

void CraftStation::set_output_inventories(const TypedArray<NodePath> &new_output_inventories) {
	output_inventories = new_output_inventories;
}

TypedArray<NodePath> CraftStation::get_output_inventories() const {
	return output_inventories;
}

void CraftStation::set_has_limit_crafts(const bool &new_has_limit_craftings) {
	has_limit_crafts = new_has_limit_craftings;
	notify_property_list_changed();
}

bool CraftStation::get_has_limit_crafts() const {
	return has_limit_crafts;
}

void CraftStation::set_limit_number_crafts(const int &new_limit_number_crafts) {
	limit_number_crafts = new_limit_number_crafts;
}

int CraftStation::get_limit_number_crafts() const {
	return limit_number_crafts;
}

void CraftStation::set_can_processing_craftings(const bool &new_can_processing_craftings) {
	can_processing_craftings = new_can_processing_craftings;
}

bool CraftStation::get_can_processing_craftings() const {
	return can_processing_craftings;
}

void CraftStation::set_can_finish_craftings(const bool &new_can_finish_craftings) {
	can_finish_craftings = new_can_finish_craftings;
}

bool CraftStation::get_can_finish_craftings() const {
	return can_finish_craftings;
}

void CraftStation::set_type(const Ref<CraftStationType> &new_type) {
	type = new_type;
}

Ref<CraftStationType> CraftStation::get_type() const {
	return type;
}

void CraftStation::set_only_remove_ingredients_after_craft(const bool &new_only_remove_ingredients_after_craft) {
	only_remove_ingredients_after_craft = new_only_remove_ingredients_after_craft;
}

bool CraftStation::get_only_remove_ingredients_after_craft() const {
	return only_remove_ingredients_after_craft;
}

void CraftStation::set_auto_craft(const bool &new_auto_craft) {
	auto_craft = new_auto_craft;
	_check_auto_crafts();
}

bool CraftStation::get_auto_craft() const {
	return auto_craft;
}

void CraftStation::set_processing_mode(const int &new_processing_mode) {
	processing_mode = new_processing_mode;
}

int CraftStation::get_processing_mode() const {
	return processing_mode;
}

void CraftStation::set_tick_update_method(const int &new_tick_update_method) {
	tick_update_method = new_tick_update_method;
}

int CraftStation::get_tick_update_method() const {
	return tick_update_method;
}

void CraftStation::set_craftings(const TypedArray<Crafting> &new_craftings) {
	craftings = new_craftings;
}

TypedArray<Crafting> CraftStation::get_craftings() const {
	return craftings;
}

void CraftStation::set_valid_recipes(const TypedArray<int> &new_valid_recipes) {
	valid_recipes = new_valid_recipes;
}

TypedArray<int> CraftStation::get_valid_recipes() const {
	return valid_recipes;
}

void CraftStation::add_input_inventory(Inventory *input_inventory) {
	ERR_FAIL_NULL_MSG(input_inventory, "param 'input_inventory' is null.");

	NodePath path = get_path_to(input_inventory);
	input_inventories.append(path);
	emit_signal("input_inventory_added", path);
}

void CraftStation::remove_input_inventory(Inventory *input_inventory) {
	ERR_FAIL_NULL_MSG(input_inventory, "param 'input_inventory' is null.");

	NodePath path = get_path_to(input_inventory);
	int64_t index = input_inventories.find(path);
	if (index == -1)
		return;
	input_inventories.remove_at(index);
	emit_signal("input_inventory_removed", path);
}
