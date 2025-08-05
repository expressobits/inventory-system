/**
 * recipes_list_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "recipes_list_editor.h"
#include "recipe_item_list_editor.h"

#include "../base/recipe.h"
#include "../base/inventory_database.h"
#include "../base/item_stack.h"
#include "../base/item_definition.h"

#include <godot_cpp/classes/label.hpp>

using namespace godot;

void RecipesListEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_search_text_changed", "text"), &RecipesListEditor::_on_search_text_changed);
	ClassDB::bind_method(D_METHOD("_on_recipe_item_selected", "index"), &RecipesListEditor::_on_recipe_item_selected);
	ClassDB::bind_method(D_METHOD("_on_recipe_item_request_remove", "recipe"), &RecipesListEditor::_on_recipe_item_request_remove);
	
	ADD_SIGNAL(MethodInfo("selected", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
	ADD_SIGNAL(MethodInfo("request_remove", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
	ADD_SIGNAL(MethodInfo("changed_products_in_recipe", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
}

void RecipesListEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

RecipesListEditor::RecipesListEditor() {
	database = nullptr;
	filter = "";
	
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

RecipesListEditor::~RecipesListEditor() {
}

void RecipesListEditor::_create_ui() {
	// Search container
	search_container = memnew(Control);
	add_child(search_container);
	search_container->set_custom_minimum_size(Vector2(0, 32));
	search_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Search line edit
	search_line_edit = memnew(LineEdit);
	search_container->add_child(search_line_edit);
	search_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	search_line_edit->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	search_line_edit->set_placeholder("Search recipes...");
	search_line_edit->connect("text_changed", callable_mp(this, &RecipesListEditor::_on_search_text_changed));

	// Search icon
	search_icon = memnew(TextureRect);
	search_container->add_child(search_icon);
	search_icon->set_custom_minimum_size(Vector2(16, 16));
	search_icon->set_anchors_preset(Control::PRESET_CENTER_RIGHT);
	search_icon->set_offset(Side::SIDE_LEFT, -24);
	search_icon->set_offset(Side::SIDE_TOP, -8);
	search_icon->set_offset(Side::SIDE_RIGHT, -8);
	search_icon->set_offset(Side::SIDE_BOTTOM, 8);
	search_icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	search_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	search_icon->set_texture(get_theme_icon("Search", "EditorIcons"));

	// Scroll container
	scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	scroll_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	// List container
	list_container = memnew(VBoxContainer);
	scroll_container->add_child(list_container);
	list_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
}

void RecipesListEditor::load_recipes(InventoryDatabase *p_database) {
	database = p_database;
	select(-1);
	_apply_filter();
}

void RecipesListEditor::_apply_filter() {
	recipes.clear();
	
	if (!database) {
		_update_recipes();
		return;
	}
	
	TypedArray<Recipe> all_recipes = database->get_recipes();
	for (int i = 0; i < all_recipes.size(); i++) {
		Ref<Recipe> recipe = all_recipes[i];
		if (recipe.is_valid() && _is_contains_product_name(recipe, filter)) {
			recipes.append(recipe);
		}
	}
	
	_update_recipes();
}

bool RecipesListEditor::_is_contains_product_name(const Ref<Recipe> &p_recipe, const String &p_filter_name) {
	if (p_filter_name.is_empty()) {
		return true;
	}
	
	if (!p_recipe.is_valid() || !database) {
		return false;
	}
	
	// Search in products
	TypedArray<ItemStack> products = p_recipe->get_products();
	for (int i = 0; i < products.size(); i++) {
		Ref<ItemStack> product = products[i];
		if (product.is_valid()) {
			Ref<ItemDefinition> item = database->get_item(product->get_item_id());
			if (item.is_valid()) {
				String item_name = item->get_name().to_lower();
				String filter_lower = p_filter_name.to_lower();
				if (item_name.contains(filter_lower)) {
					return true;
				}
			}
		}
	}
	
	return false;
}

void RecipesListEditor::_update_recipes() {
	_clear_list();
	
	for (int i = 0; i < recipes.size(); i++) {
		Ref<Recipe> recipe = recipes[i];
		if (recipe.is_valid()) {
			RecipeItemListEditor *recipe_item = memnew(RecipeItemListEditor);
			list_container->add_child(recipe_item);
			recipe_item->load_recipe(recipe, database);
			recipes_ui.append(recipe_item);
			
			// Connect signals with index binding
			recipe_item->connect("selected", callable_mp(this, &RecipesListEditor::_on_recipe_item_selected).bind(i));
			recipe_item->connect("request_remove", callable_mp(this, &RecipesListEditor::_on_recipe_item_request_remove));
		}
	}
}

void RecipesListEditor::_clear_list() {
	for (int i = 0; i < recipes_ui.size(); i++) {
		RecipeItemListEditor *recipe_ui = Object::cast_to<RecipeItemListEditor>(recipes_ui[i]);
		if (recipe_ui) {
			list_container->remove_child(recipe_ui);
			recipe_ui->queue_free();
		}
	}
	recipes_ui.clear();
}

void RecipesListEditor::update_recipes_ui() {
	for (int i = 0; i < recipes_ui.size(); i++) {
		RecipeItemListEditor *recipe_ui = Object::cast_to<RecipeItemListEditor>(recipes_ui[i]);
		if (recipe_ui) {
			recipe_ui->update_recipe();
		}
	}
}

void RecipesListEditor::reload() {
	load_recipes(database);
}

void RecipesListEditor::select_last() {
	if (recipes_ui.size() > 0) {
		RecipeItemListEditor *last_item = Object::cast_to<RecipeItemListEditor>(recipes_ui[recipes_ui.size() - 1]);
		if (last_item) {
			last_item->select();
		}
	}
}

void RecipesListEditor::select_with_recipe(const Ref<Recipe> &p_recipe) {
	for (int i = 0; i < recipes.size(); i++) {
		Ref<Recipe> recipe = recipes[i];
		if (recipe == p_recipe) {
			RecipeItemListEditor *recipe_ui = Object::cast_to<RecipeItemListEditor>(recipes_ui[i]);
			if (recipe_ui) {
				recipe_ui->select();
				_on_recipe_item_selected(i);
			}
			break;
		}
	}
}

void RecipesListEditor::select(int p_index) {
	if (p_index >= 0 && p_index < recipes.size()) {
		Ref<Recipe> recipe = recipes[p_index];
		if (recipe.is_valid()) {
			emit_signal("selected", recipe);
		}
	}
}

void RecipesListEditor::set_filter(const String &p_filter) {
	filter = p_filter;
	_apply_filter();
}

String RecipesListEditor::get_filter() const {
	return filter;
}

void RecipesListEditor::_on_search_text_changed(const String &p_text) {
	set_filter(p_text);
}

void RecipesListEditor::_on_recipe_item_selected(int p_index) {
	// Unselect all other items
	for (int i = 0; i < recipes_ui.size(); i++) {
		if (i != p_index) {
			RecipeItemListEditor *recipe_ui = Object::cast_to<RecipeItemListEditor>(recipes_ui[i]);
			if (recipe_ui) {
				recipe_ui->unselect();
			}
		}
	}
	
	// Emit selection signal
	select(p_index);
}

void RecipesListEditor::_on_recipe_item_request_remove(const Ref<Recipe> &p_recipe) {
	emit_signal("request_remove", p_recipe);
}

#endif // TOOLS_ENABLED