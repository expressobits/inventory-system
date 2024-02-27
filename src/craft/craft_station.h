#ifndef CRAFT_STATION_CLASS_H
#define CRAFT_STATION_CLASS_H

#include "base/craft_station_type.h"
#include "base/inventory_database.h"
#include "base/node_inventories.h"
#include "core/inventory.h"
#include "crafting.h"
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class CraftStation : public NodeInventories {
	GDCLASS(CraftStation, NodeInventories);

private:
	TypedArray<NodePath> input_inventories;
	TypedArray<NodePath> output_inventories;
	int limit_number_crafts = -1;
	bool can_processing_craftings = true;
	Ref<CraftStationType> type;
	bool only_remove_ingredients_after_craft = false;
	bool auto_craft = false;
	int processing_mode = 0;
	bool is_open = false;
	TypedArray<Crafting> craftings;
	TypedArray<int> valid_recipes;

	void _process_crafts(float delta);
	bool _use_items(const Ref<Recipe> &recipe);
	void _on_input_inventory_item_added(Ref<Item> item, int amount);
	void _on_input_inventory_item_removed(Ref<Item> item, int amount);
	void _check_auto_crafts();

protected:
	static void _bind_methods();

public:
	enum ProcessingMode {
		PARALLEL,
		SEQUENTIAL,
	};

	CraftStation();
	~CraftStation();
	virtual void _ready() override;
	virtual void _process(float delta);
	void _setup_connections();
	void add_crafting(int recipe_index, const Ref<Recipe> &recipe);
	void remove_crafting(int crafting_index);
	void finish_crafting(int crafting_index);
	bool is_crafting() const;
	int crafting_count() const;
	bool can_craft(const Ref<Recipe> &recipe) const;
	bool contains_ingredients(const Ref<Recipe> &recipe) const;
	void craft(int recipe_index);
	void cancel_craft(int crafting_index);
	bool open();
	bool close();
	Inventory *get_input_inventory(const int &index = 0) const;
	Inventory *get_output_inventory(const int &index = 0) const;
	void set_input_inventories(const TypedArray<NodePath> &new_input_inventories);
	TypedArray<NodePath> get_input_inventories() const;
	void set_output_inventories(const TypedArray<NodePath> &new_output_inventories);
	TypedArray<NodePath> get_output_inventories() const;
	void set_limit_number_crafts(const int &new_limit_number_crafts);
	int get_limit_number_crafts() const;
	void set_can_processing_craftings(const bool &new_can_processing_craftings);
	bool get_can_processing_craftings() const;
	void set_type(const Ref<CraftStationType> &new_type);
	Ref<CraftStationType> get_type() const;
	void set_only_remove_ingredients_after_craft(const bool &new_only_remove_ingredients_after_craft);
	bool get_only_remove_ingredients_after_craft() const;
	void set_auto_craft(const bool &new_auto_craft);
	bool get_auto_craft() const;
	void set_processing_mode(const int &new_processing_mode);
	int get_processing_mode() const;
	void set_is_open(const bool &new_is_open);
	bool get_is_open() const;
	void set_craftings(const TypedArray<Crafting> &new_craftings);
	TypedArray<Crafting> get_craftings() const;
	void set_valid_recipes(const TypedArray<int> &new_valid_recipes);
	TypedArray<int> get_valid_recipes() const;
};

VARIANT_ENUM_CAST(CraftStation::ProcessingMode);

#endif // CRAFT_STATION_CLASS_H