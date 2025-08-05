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

#include "base/base_inventory_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

using namespace godot;

class InventoryDatabase;
class Recipe;
class RecipesListEditor;
class RecipeEditor;

class RecipesEditor : public BaseInventoryEditor {
	GDCLASS(RecipesEditor, BaseInventoryEditor);

private:
	// UI Components
	HSplitContainer *hsplit_container;
	RecipesListEditor *recipes_list;
	RecipeEditor *recipe_editor;
	TextureRect *search_icon;

	void _create_ui();
	void _apply_theme();
	void _load_recipes();
	void _load_recipe(const Ref<Recipe> &p_recipe);
	
	// Signal handlers
	void _on_recipes_list_selected(const Ref<Recipe> &p_recipe);
	void _on_recipe_editor_changed();
	void _on_recipe_item_editor_request_remove(const Ref<Recipe> &p_recipe);
	void _on_recipe_item_editor_changed_products_in_recipe(const Ref<Recipe> &p_recipe);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin) override;
	void on_load_database() override;
	bool remove_current_data() override;

	RecipesEditor();
	~RecipesEditor();
};

#endif // RECIPES_EDITOR_H

#endif // TOOLS_ENABLED