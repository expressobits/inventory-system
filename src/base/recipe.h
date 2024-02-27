#ifndef RECIPE_CLASS_H
#define RECIPE_CLASS_H

#include "slot.h"
#include "craft_station_type.h"
#include <godot_cpp/classes/resource.hpp>


using namespace godot;

class Recipe : public Resource {
	GDCLASS(Recipe, Resource);

private:
	TypedArray<Slot> products;
	float time_to_craft = 4.0;
	Ref<CraftStationType> station;
	TypedArray<Slot> ingredients;
	TypedArray<Slot> required_items;

protected:
	static void _bind_methods();

public:
	Recipe();
	~Recipe();
	void set_products(const TypedArray<Slot> &new_products);
	TypedArray<Slot> get_products() const;
	void set_time_to_craft(const float &new_time_to_craft);
	float get_time_to_craft() const;
	void set_station(const Ref<CraftStationType> &new_station);
	Ref<CraftStationType> get_station() const;
	void set_ingredients(const TypedArray<Slot> &new_ingredients);
	TypedArray<Slot> get_ingredients() const;
	void set_required_items(const TypedArray<Slot> &new_required_items);
	TypedArray<Slot> get_required_items() const;
};

#endif // RECIPE_CLASS_H