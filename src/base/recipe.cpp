#include "recipe.h"

void Recipe::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_products", "products"), &Recipe::set_products);
	ClassDB::bind_method(D_METHOD("get_products"), &Recipe::get_products);
	ClassDB::bind_method(D_METHOD("set_time_to_craft", "time_to_craft"), &Recipe::set_time_to_craft);
	ClassDB::bind_method(D_METHOD("get_time_to_craft"), &Recipe::get_time_to_craft);
	ClassDB::bind_method(D_METHOD("set_station", "station"), &Recipe::set_station);
	ClassDB::bind_method(D_METHOD("get_station"), &Recipe::get_station);
	ClassDB::bind_method(D_METHOD("set_ingredients", "ingredients"), &Recipe::set_ingredients);
	ClassDB::bind_method(D_METHOD("get_ingredients"), &Recipe::get_ingredients);
	ClassDB::bind_method(D_METHOD("set_required_items", "required_items"), &Recipe::set_required_items);
	ClassDB::bind_method(D_METHOD("get_required_items"), &Recipe::get_required_items);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "products", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Slot")), "set_products", "get_products");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_to_craft"), "set_time_to_craft", "get_time_to_craft");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "station", PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType"), "set_station", "get_station");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "ingredients", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Slot")), "set_ingredients", "get_ingredients");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "required_items", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Slot")), "set_required_items", "get_required_items");
}

Recipe::Recipe() {
}

Recipe::~Recipe() {
}

void Recipe::set_products(const TypedArray<Slot> &new_products) {
	products = new_products;
}

TypedArray<Slot> Recipe::get_products() const {
	return products;
}

void Recipe::set_time_to_craft(const float &new_time_to_craft) {
	time_to_craft = new_time_to_craft;
}

float Recipe::get_time_to_craft() const {
	return time_to_craft;
}

void Recipe::set_station(const Ref<CraftStationType> &new_station) {
	station = new_station;
}

Ref<CraftStationType> Recipe::get_station() const {
	return station;
}

void Recipe::set_ingredients(const TypedArray<Slot> &new_ingredients) {
	ingredients = new_ingredients;
}

TypedArray<Slot> Recipe::get_ingredients() const {
	return ingredients;
}

void Recipe::set_required_items(const TypedArray<Slot> &new_required_items) {
	required_items = new_required_items;
}

TypedArray<Slot> Recipe::get_required_items() const {
	return required_items;
}
