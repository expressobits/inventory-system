/**
 * recipes_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "recipes_editor.h"
#include "recipes_list_editor.h"
#include "recipe_editor.h"

#include "../base/inventory_database.h"
#include "../base/recipe.h"

using namespace godot;

void RecipesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_recipes_list_selected", "recipe"), &RecipesEditor::_on_recipes_list_selected);
	ClassDB::bind_method(D_METHOD("_on_recipe_editor_changed"), &RecipesEditor::_on_recipe_editor_changed);
	ClassDB::bind_method(D_METHOD("_on_recipe_item_editor_request_remove", "recipe"), &RecipesEditor::_on_recipe_item_editor_request_remove);
	ClassDB::bind_method(D_METHOD("_on_recipe_item_editor_changed_products_in_recipe", "recipe"), &RecipesEditor::_on_recipe_item_editor_changed_products_in_recipe);
}

void RecipesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

RecipesEditor::RecipesEditor() {
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

RecipesEditor::~RecipesEditor() {
}

void RecipesEditor::_create_ui() {
	// HSplitContainer
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	hsplit_container->set_offset(SIDE_LEFT, -4);
	hsplit_container->set_offset(SIDE_RIGHT, 4);
	hsplit_container->set_split_offset(-512); // Match addon split offset

	// RecipesListEditor (left side)
	recipes_list = memnew(RecipesListEditor);
	recipes_list->set_custom_minimum_size(Vector2(336, 0));
	hsplit_container->add_child(recipes_list);
	recipes_list->connect("selected", callable_mp(this, &RecipesEditor::_on_recipes_list_selected));
	recipes_list->connect("request_remove", callable_mp(this, &RecipesEditor::_on_recipe_item_editor_request_remove));
	recipes_list->connect("changed_products_in_recipe", callable_mp(this, &RecipesEditor::_on_recipe_item_editor_changed_products_in_recipe));

	// RecipeEditor (right side)
	recipe_editor = memnew(RecipeEditor);
	hsplit_container->add_child(recipe_editor);
	recipe_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	recipe_editor->connect("changed", callable_mp(this, &RecipesEditor::_on_recipe_editor_changed));
}

void RecipesEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	BaseInventoryEditor::set_editor_plugin(p_plugin);
	
	if (recipe_editor) {
		recipe_editor->set_editor_plugin(p_plugin);
	}
	
	_apply_theme();
}

void RecipesEditor::on_load_database() {
	_load_recipes();
}

void RecipesEditor::_load_recipes() {
	if (recipes_list) {
		recipes_list->load_recipes(get_database());
	}
}

void RecipesEditor::_load_recipe(const Ref<Recipe> &p_recipe) {
	if (recipe_editor) {
		recipe_editor->load_recipe(p_recipe, get_database());
	}
}

void RecipesEditor::_apply_theme() {
	BaseInventoryEditor::_apply_theme();
	
	// Apply search icon if available
	if (search_icon && has_theme_icon("Search", "EditorIcons")) {
		search_icon->set_texture(get_theme_icon("Search", "EditorIcons"));
	}
}

// Signal handlers
void RecipesEditor::_on_recipes_list_selected(const Ref<Recipe> &p_recipe) {
	_load_recipe(p_recipe);
}

void RecipesEditor::_on_recipe_editor_changed() {
	if (recipes_list) {
		recipes_list->update_recipes_ui();
	}
	emit_signal("data_changed");
}

void RecipesEditor::_on_recipe_item_editor_request_remove(const Ref<Recipe> &p_recipe) {
	if (p_recipe.is_valid()) {
		String recipe_name = "Recipe";
		
		// Try to get a meaningful name from products
		TypedArray<ItemStack> products = p_recipe->get_products();
		if (products.size() > 0) {
			Ref<ItemStack> first_product = products[0];
			if (first_product.is_valid() && get_database()) {
				Ref<ItemDefinition> item = get_database()->get_item(first_product->get_item_id());
				if (item.is_valid() && !item->get_name().is_empty()) {
					recipe_name = "Recipe for " + item->get_name();
				}
			}
		}
		
		remove_confirmation_dialog->set_text("Remove " + recipe_name + "?");
		remove_confirmation_dialog->popup_centered();
		set_current_data(p_recipe);
	}
}

void RecipesEditor::_on_recipe_item_editor_changed_products_in_recipe(const Ref<Recipe> &p_recipe) {
	_load_recipes();
}

bool RecipesEditor::remove_current_data() {
	// Call super.remove_current_data() like in addon
	bool removed = BaseInventoryEditor::remove_current_data();
	if (removed) {
		_load_recipes();
		emit_signal("data_changed");
		if (recipe_editor && get_database()) {
			recipe_editor->load_recipe(nullptr, get_database());
		}
	}
	return removed;
}

#endif // TOOLS_ENABLED