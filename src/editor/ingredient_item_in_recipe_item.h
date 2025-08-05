/**
 * ingredient_item_in_recipe_item.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef INGREDIENT_ITEM_IN_RECIPE_ITEM_H
#define INGREDIENT_ITEM_IN_RECIPE_ITEM_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>

using namespace godot;

class InventoryDatabase;
class ItemStack;

class IngredientItemInRecipeItem : public HBoxContainer {
	GDCLASS(IngredientItemInRecipeItem, HBoxContainer)

private:
	TextureRect *texture_rect;
	Label *label;
	
	InventoryDatabase *database;
	Ref<ItemStack> item_stack;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	IngredientItemInRecipeItem();
	~IngredientItemInRecipeItem();

	void setup(InventoryDatabase *p_database, const Ref<ItemStack> &p_item_stack);
	void _create_ui();
};

#endif // INGREDIENT_ITEM_IN_RECIPE_ITEM_H

#endif // TOOLS_ENABLED