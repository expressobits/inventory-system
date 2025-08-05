/**
 * recipe_item_list_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef RECIPE_ITEM_LIST_EDITOR_H
#define RECIPE_ITEM_LIST_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/h_separator.hpp>

using namespace godot;

class Recipe;
class InventoryDatabase;

class RecipeItemListEditor : public Control {
	GDCLASS(RecipeItemListEditor, Control);

private:
	Ref<Recipe> recipe;
	InventoryDatabase *database;
	
	// UI Components
	Panel *panel;
	MarginContainer *margin_container;
	VBoxContainer *main_container;
	Label *time_label;
	TextureRect *craft_station_icon;
	TextureRect *time_icon;
	HBoxContainer *ingredients_list;
	HBoxContainer *products_list;
	HBoxContainer *required_item_list;
	Button *delete_button;
	
	void _create_ui();
	void _setup_style();
	void _update_recipe_display();
	void _clear_item_lists();
	
	void _on_panel_gui_input(const Ref<InputEvent> &p_event);
	void _on_delete_button_pressed();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void load_recipe(const Ref<Recipe> &p_recipe, InventoryDatabase *p_database);
	void update_recipe();
	void select();
	void unselect();
	
	RecipeItemListEditor();
	~RecipeItemListEditor();
};

#endif // RECIPE_ITEM_LIST_EDITOR_H

#endif // TOOLS_ENABLED