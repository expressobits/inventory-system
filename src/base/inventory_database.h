#ifndef INVENTORY_DATABASE_CLASS_H
#define INVENTORY_DATABASE_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include "craft_station_type.h"
#include "item_category.h"
#include "item_definition.h"
#include "recipe.h"

using namespace godot;

class InventoryDatabase : public Resource {
	GDCLASS(InventoryDatabase, Resource);

private:
	TypedArray<ItemDefinition> items;
	TypedArray<Recipe> recipes;
	TypedArray<CraftStationType> stations_type;
	TypedArray<ItemCategory> item_categories;
	Dictionary items_cache;
	Dictionary categories_code_cache;

	void _update_items_cache();
	void _update_items_categories_cache();

protected:
	static void _bind_methods();

public:
	InventoryDatabase();
	~InventoryDatabase();
	void set_items(const TypedArray<ItemDefinition> &new_items);
	TypedArray<ItemDefinition> get_items() const;
	void set_recipes(const TypedArray<Recipe> &new_recipes);
	TypedArray<Recipe> get_recipes() const;
	void set_stations_type(const TypedArray<CraftStationType> &new_stations_type);
	TypedArray<CraftStationType> get_stations_type() const;
	void set_item_categories(const TypedArray<ItemCategory> &new_item_categories);
	TypedArray<ItemCategory> get_item_categories() const;
	void set_items_cache(const Dictionary &new_items_cache);
	Dictionary get_items_cache() const;
	void set_categories_code_cache(const Dictionary &new_categories_code_cache);
	Dictionary get_categories_code_cache() const;

	void add_new_item(const Ref<ItemDefinition> item);
	void remove_item(const Ref<ItemDefinition> item);
	void add_new_category(const Ref<ItemCategory> category);
	void remove_category(const Ref<ItemCategory> category);
	Ref<ItemDefinition> get_item(int id) const;
	bool has_item_id(int id) const;
	int get_valid_id() const;
	int get_new_valid_id() const;
	Ref<ItemCategory> get_category(int code);
};

#endif // INVENTORY_DATABASE_CLASS_H