/**
 * recipes_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef RECIPES_EDITOR_H
#define RECIPES_EDITOR_H

#include "inventory_item_list_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class InventoryDatabase;
class Recipe;

class RecipesEditor : public Control {
	GDCLASS(RecipesEditor, Control);

private:
	EditorPlugin *editor_plugin;
	Ref<InventoryDatabase> database;

	// UI Components
	HSplitContainer *hsplit_container;
	InventoryItemListEditor *recipe_list_editor;
	ScrollContainer *details_scroll;
	VBoxContainer *details_container;
	Label *no_selection_label;

	Ref<Recipe> current_recipe;

	void _create_ui();
	void _update_details(const Ref<Recipe> &p_recipe);
	void _clear_details();

	void _on_recipe_selected(const Variant &p_recipe, int p_index);
	void _on_recipe_popup_menu_requested(const Vector2 &p_position);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void load_from_database(const Ref<InventoryDatabase> &p_database);
	void select_recipe(const Ref<Recipe> &p_recipe);

	// Signal emissions
	void _emit_removed(const Ref<Recipe> &p_recipe);

	RecipesEditor();
	~RecipesEditor();
};

#endif // RECIPES_EDITOR_H

#endif // TOOLS_ENABLED