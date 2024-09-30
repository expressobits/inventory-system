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
	Array items;
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
	Ref<ItemDefinition> get_item(String id) const;
	bool has_item_category_id(String id) const;
	bool has_item_id(String id) const;
	bool has_craft_station_type_id(String id) const;
	String get_valid_id() const;
	String get_new_valid_id() const;
	Ref<ItemCategory> get_category(int code);

	Dictionary serialize_item_definition(const Ref<ItemDefinition> definition) const;
	void deserialize_item_definition(Ref<ItemDefinition> definition, const Dictionary data) const;
	Dictionary serialize_item_category(const Ref<ItemCategory> category) const;
	void deserialize_item_category(Ref<ItemCategory> category, const Dictionary data) const;
	Dictionary serialize_recipe(const Ref<Recipe> recipe) const;
	void deserialize_recipe(Ref<Recipe> recipe, const Dictionary data) const;
	Dictionary serialize_station_type(const Ref<CraftStationType> craft_station_type) const;
	void deserialize_station_type(Ref<CraftStationType> craft_station_type, const Dictionary data) const;
	String serialize_slot(const Ref<Slot> slot) const;
	void deserialize_slot(Ref<Slot> slot, const String data) const;
	Array serialize_slots(const TypedArray<Slot> slots) const;
	void deserialize_slots(TypedArray<Slot> slots, const Array data) const;

	void add_item();
	void add_item_category();
	void add_recipe();
	void add_craft_station_type();

	Ref<ItemCategory> get_category_from_id(String id) const;
	Ref<CraftStationType> get_craft_station_from_id(String id) const;

	Dictionary serialize() const;
	void deserialize(const Dictionary data);

	Array serialize_items() const;
	void deserialize_items(Array datas);
	Array serialize_item_categories() const;
	void deserialize_item_categories(Array datas);
	Array serialize_craft_station_types() const;
	void deserialize_craft_station_types(Array datas);
	Array serialize_recipes() const;
	void deserialize_recipes(Array datas);

	String export_to_invdata() const;
	void import_to_invdata(const String json);
};

#endif // INVENTORY_DATABASE_CLASS_H