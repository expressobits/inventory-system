/**
 * recipes_list_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef RECIPES_LIST_EDITOR_H
#define RECIPES_LIST_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

using namespace godot;

class Recipe;
class InventoryDatabase;
class RecipeItemListEditor;

class RecipesListEditor : public VBoxContainer {
	GDCLASS(RecipesListEditor, VBoxContainer);

private:
	InventoryDatabase *database;
	Array recipes; // Filtered list of recipes
	Array recipes_ui; // Array of RecipeItemListEditor instances
	String filter;
	
	// UI Components
	Control *search_container;
	TextureRect *search_icon;
	LineEdit *search_line_edit;
	ScrollContainer *scroll_container;
	VBoxContainer *list_container;
	
	void _create_ui();
	void _clear_list();
	void _update_recipes();
	void _apply_filter();
	bool _is_contains_product_name(const Ref<Recipe> &p_recipe, const String &p_filter_name);
	
	void _on_search_text_changed(const String &p_text);
	void _on_recipe_item_selected(int p_index);
	void _on_recipe_item_request_remove(const Ref<Recipe> &p_recipe);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void load_recipes(InventoryDatabase *p_database);
	void update_recipes_ui();
	void reload();
	void select_last();
	void select_with_recipe(const Ref<Recipe> &p_recipe);
	void select(int p_index);
	void set_filter(const String &p_filter);
	String get_filter() const;
	
	RecipesListEditor();
	~RecipesListEditor();
};

#endif // RECIPES_LIST_EDITOR_H

#endif // TOOLS_ENABLED