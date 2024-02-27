#include "item_category.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>


using namespace godot;

void ItemCategory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &ItemCategory::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &ItemCategory::get_name);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &ItemCategory::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &ItemCategory::get_color);
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &ItemCategory::set_icon);
	ClassDB::bind_method(D_METHOD("get_icon"), &ItemCategory::get_icon);
	ClassDB::bind_method(D_METHOD("set_code", "code"), &ItemCategory::set_code);
	ClassDB::bind_method(D_METHOD("get_code"), &ItemCategory::get_code);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "code"), "set_code", "get_code");
}

ItemCategory::ItemCategory() {
}

ItemCategory::~ItemCategory() {
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
