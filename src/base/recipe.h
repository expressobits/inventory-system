#ifndef RECIPE_CLASS_H
#define RECIPE_CLASS_H

#include "craft_station_type.h"
#include "item_stack.h"
#include "slot.h"
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Recipe : public Resource {
	GDCLASS(Recipe, Resource);

private:
	TypedArray<ItemStack> products;
	float time_to_craft = 4.0;
	Ref<CraftStationType> station;
	TypedArray<ItemStack> ingredients;
	TypedArray<ItemStack> required_items;

protected:
	static void _bind_methods();

public:
	Recipe();
	~Recipe();
	void set_products(const TypedArray<ItemStack> &new_products);
	TypedArray<ItemStack> get_products() const;
	void set_time_to_craft(const float &new_time_to_craft);
	float get_time_to_craft() const;
	void set_station(const Ref<CraftStationType> &new_station);
	Ref<CraftStationType> get_station() const;
	void set_ingredients(const TypedArray<ItemStack> &new_ingredients);
	TypedArray<ItemStack> get_ingredients() const;
	void set_required_items(const TypedArray<ItemStack> &new_required_items);
	TypedArray<ItemStack> get_required_items() const;
};

#endif // RECIPE_CLASS_H