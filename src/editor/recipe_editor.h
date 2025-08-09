/**
 * recipe_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef RECIPE_EDITOR_H
#define RECIPE_EDITOR_H

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>

#include "base_resource_editor.h"
#include "item_stack_selector.h"

using namespace godot;

class Recipe;
class InventoryDatabase;
class ItemStack;
class CraftStationType;

class RecipeEditor : public BaseResourceEditor {
	GDCLASS(RecipeEditor, BaseResourceEditor);

private:
	EditorPlugin *editor_plugin;
	Ref<Recipe> recipe;
	InventoryDatabase *database;
	
	// UI Components
	SpinBox *time_to_craft_spin_box;
	OptionButton *craft_station_type_option_button;
	VBoxContainer *ingredients_container;
	VBoxContainer *products_container;
	VBoxContainer *required_items_container;
	Button *new_ingredient_button;
	Button *new_product_button;
	Button *new_required_item_button;
	
	// Lists for dynamic UI management
	Array stations_list;
	Array ingredient_editors;
	Array product_editors;  
	Array required_item_editors;
	
	bool signals_connected;
	
	void _create_ui() override;
	void _connect_signals() override;
	void _disconnect_signals() override;
	void _setup_station();
	void _setup_ingredients();
	void _setup_products();
	void _setup_required_items();
	void _clear_ingredient_editors();
	void _clear_product_editors();
	void _clear_required_item_editors();
	
	// Helper methods
	Button* _create_remove_button(HBoxContainer* p_parent, const String& p_tooltip, const Callable& p_callback);
	
	// Signal handlers
	void _on_time_to_craft_changed(double p_value);
	void _on_craft_station_selected(int p_index);
	void _on_new_ingredient_pressed();
	void _on_new_product_pressed();
	void _on_new_required_item_pressed();
	void _on_ingredient_changed(const Ref<ItemStack> &p_item_stack);
	void _on_product_changed(const Ref<ItemStack> &p_item_stack);
	void _on_required_item_changed(const Ref<ItemStack> &p_item_stack);
	void _on_remove_ingredient_pressed(Button *p_button);
	void _on_remove_product_pressed(Button *p_button);
	void _on_remove_required_item_pressed(Button *p_button);

protected:
	static void _bind_methods();

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void load_recipe(const Ref<Recipe> &p_recipe, InventoryDatabase *p_database);
	void reload();
	
	RecipeEditor();
	~RecipeEditor();
};

#endif // RECIPE_EDITOR_H

#endif // TOOLS_ENABLED