#include "item_definition.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ItemDefinition::_check_invalid_dynamic_properties() {
	// Checking if dynamic property is a reference to a property that does not exist
	for (size_t i = 0; i < dynamic_properties.size(); i++)
	{
		if (properties.keys().find(dynamic_properties[i]) == -1) {
			dynamic_properties.remove_at(i);
			i--;
		}
	}
}

void ItemDefinition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_id", "id"), &ItemDefinition::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &ItemDefinition::get_id);
	ClassDB::bind_method(D_METHOD("set_can_stack", "can_stack"), &ItemDefinition::set_can_stack);
	ClassDB::bind_method(D_METHOD("get_can_stack"), &ItemDefinition::get_can_stack);
	ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &ItemDefinition::set_max_stack);
	ClassDB::bind_method(D_METHOD("get_max_stack"), &ItemDefinition::get_max_stack);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &ItemDefinition::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &ItemDefinition::get_name);
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &ItemDefinition::set_icon);
	ClassDB::bind_method(D_METHOD("get_icon"), &ItemDefinition::get_icon);
	ClassDB::bind_method(D_METHOD("set_weight", "weight"), &ItemDefinition::set_weight);
	ClassDB::bind_method(D_METHOD("get_weight"), &ItemDefinition::get_weight);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &ItemDefinition::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &ItemDefinition::get_properties);
	ClassDB::bind_method(D_METHOD("set_dynamic_properties", "dynamic_properties"), &ItemDefinition::set_dynamic_properties);
	ClassDB::bind_method(D_METHOD("get_dynamic_properties"), &ItemDefinition::get_dynamic_properties);
	ClassDB::bind_method(D_METHOD("set_categories", "categories"), &ItemDefinition::set_categories);
	ClassDB::bind_method(D_METHOD("get_categories"), &ItemDefinition::get_categories);
	ClassDB::bind_method(D_METHOD("is_of_category", "category"), &ItemDefinition::is_in_category);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_stack"), "set_can_stack", "get_can_stack");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_stack"), "set_max_stack", "get_max_stack");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weight"), "set_weight", "get_weight");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dynamic_properties", PROPERTY_HINT_ARRAY_TYPE, "String"), "set_dynamic_properties", "get_dynamic_properties");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "categories", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")), "set_categories", "get_categories");
}

ItemDefinition::ItemDefinition() {
}

ItemDefinition::~ItemDefinition() {
}

// Properties

void ItemDefinition::set_id(const String &new_id) {
	id = new_id;
}

String ItemDefinition::get_id() const {
	return id;
}

void ItemDefinition::set_can_stack(const bool &new_can_stack) {
	can_stack = new_can_stack;
}

bool ItemDefinition::get_can_stack() const {
	return can_stack;
}

void ItemDefinition::set_name(const String &new_name) {
	name = new_name;
}

String ItemDefinition::get_name() const {
	return name;
}

void ItemDefinition::set_max_stack(const int &new_max_stack) {
	max_stack = new_max_stack;
}

int ItemDefinition::get_max_stack() const {
	if (can_stack) {
		return max_stack;
	}
	return 1;
}

void ItemDefinition::set_icon(const Ref<Texture2D> &p_icon) {
	icon = p_icon;
	notify_property_list_changed();
}

Ref<Texture2D> ItemDefinition::get_icon() const {
	return icon;
}

void ItemDefinition::set_weight(const float &new_weight) {
	weight = new_weight;
}

float ItemDefinition::get_weight() const {
	return weight;
}

void ItemDefinition::set_properties(const Dictionary &new_properties) {
	properties = new_properties;
	_check_invalid_dynamic_properties();
}

Dictionary ItemDefinition::get_properties() const {
	return properties;
}

void ItemDefinition::set_dynamic_properties(const TypedArray<String> &new_dynamic_properties) {
	dynamic_properties = new_dynamic_properties;
}

TypedArray<String> ItemDefinition::get_dynamic_properties() const {
	return dynamic_properties;
}

void ItemDefinition::set_categories(const TypedArray<ItemCategory> &new_categories) {
	categories = new_categories;
	Dictionary properties = get_properties();
	Array dynamic_properties = get_dynamic_properties();
	for (size_t i = 0; i < categories.size(); i++) {
		Ref<ItemCategory> category = categories[i];
		ERR_FAIL_NULL_MSG(category, "'category' is null.");
		for (size_t i = 0; i < category->get_item_properties().size(); i++) {
			Variant property_key = category->get_item_properties().keys()[i];
			if (properties.keys().find(property_key) == -1) {
				// The item does not have the key in its properties, which is found in the category properties template.
				properties[property_key] = category->get_item_properties()[property_key];
				if(category->get_item_dynamic_properties().find(property_key) != -1){
					dynamic_properties.append(property_key);
				}
			}
		}
	}
}

TypedArray<ItemCategory> ItemDefinition::get_categories() const {
	return categories;
}

bool ItemDefinition::is_in_category(const Ref<ItemCategory> category) const {
	ERR_FAIL_NULL_V_MSG(category, false, "'category' is null.");

	for (size_t i = 0; i < categories.size(); i++) {
		Ref<ItemCategory> c = categories[i];
		if (c == category) {
			return true;
		}
	}
	return false;
}
