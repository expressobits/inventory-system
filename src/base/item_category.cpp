#include "item_category.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>


using namespace godot;

void ItemCategory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_id", "id"), &ItemCategory::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &ItemCategory::get_id);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &ItemCategory::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &ItemCategory::get_name);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &ItemCategory::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &ItemCategory::get_color);
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &ItemCategory::set_icon);
	ClassDB::bind_method(D_METHOD("get_icon"), &ItemCategory::get_icon);
	ClassDB::bind_method(D_METHOD("set_item_properties", "item_properties"), &ItemCategory::set_item_properties);
	ClassDB::bind_method(D_METHOD("get_item_properties"), &ItemCategory::get_item_properties);
	ClassDB::bind_method(D_METHOD("set_item_dynamic_properties", "item_dynamic_properties"), &ItemCategory::set_item_dynamic_properties);
	ClassDB::bind_method(D_METHOD("get_item_dynamic_properties"), &ItemCategory::get_item_dynamic_properties);
	ClassDB::bind_method(D_METHOD("get_code"), &ItemCategory::get_code);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "item_properties"), "set_item_properties", "get_item_properties");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "item_dynamic_properties", PROPERTY_HINT_ARRAY_TYPE, "String"), "set_item_dynamic_properties", "get_item_dynamic_properties");
}

ItemCategory::ItemCategory() {
}

ItemCategory::~ItemCategory() {
}

void ItemCategory::set_id(const String &new_id) {
	this->id = new_id;
}

String ItemCategory::get_id() const {
	return id;
}

// Properties
void ItemCategory::set_name(const String &new_name) {
	name = new_name;
}

String ItemCategory::get_name() const {
	return name;
}

void ItemCategory::set_color(const Color &new_color) {
	color = new_color;
}

Color ItemCategory::get_color() const {
	return color;
}

void ItemCategory::set_icon(const Ref<Texture2D> &p_icon) {
	icon = p_icon;
	notify_property_list_changed();
}

Ref<Texture2D> ItemCategory::get_icon() const {
	return icon;
}

void ItemCategory::set_code(const int &new_code) {
	code = new_code;
}

int ItemCategory::get_code() const {
	return code;
}

void ItemCategory::set_item_properties(const Dictionary &new_item_properties) {
	item_properties = new_item_properties;
}

Dictionary ItemCategory::get_item_properties() const {
	return item_properties;
}

void ItemCategory::set_item_dynamic_properties(const TypedArray<String> &new_item_dynamic_properties) {
	item_dynamic_properties = new_item_dynamic_properties;
}

TypedArray<String> ItemCategory::get_item_dynamic_properties() const {
	return item_dynamic_properties;
}
