#include "crafting.h"

void Crafting::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_recipe_index", "recipe_index"), &Crafting::set_recipe_index);
	ClassDB::bind_method(D_METHOD("get_recipe_index"), &Crafting::get_recipe_index);
	ClassDB::bind_method(D_METHOD("set_time", "time"), &Crafting::set_time);
	ClassDB::bind_method(D_METHOD("get_time"), &Crafting::get_time);
	ClassDB::bind_method(D_METHOD("is_finished"), &Crafting::is_finished);
	ClassDB::bind_method(D_METHOD("process", "delta"), &Crafting::process);
	ClassDB::bind_method(D_METHOD("to_data"), &Crafting::to_data);
	ClassDB::bind_method(D_METHOD("from_data", "data"), &Crafting::from_data);
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

Array Crafting::to_data() const {
	Array a = Array();
	a.append(recipe_index);
	a.append(time);
	return a;
}

void Crafting::from_data(Array data) {
	recipe_index = data[0];
	time = data[1];
}
