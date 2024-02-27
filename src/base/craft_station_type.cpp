#include "craft_station_type.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>


using namespace godot;

void CraftStationType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &CraftStationType::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &CraftStationType::get_name);
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &CraftStationType::set_icon);
	ClassDB::bind_method(D_METHOD("get_icon"), &CraftStationType::get_icon);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
}

CraftStationType::CraftStationType() {
}

CraftStationType::~CraftStationType() {
}

void CraftStationType::set_name(const String &new_name) {
	name = new_name;
}

String CraftStationType::get_name() const {
	return name;
}

void CraftStationType::set_icon(const Ref<Texture2D> &new_icon) {
	icon = new_icon;
}

Ref<Texture2D> CraftStationType::get_icon() const {
	return icon;
}
