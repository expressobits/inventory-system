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

#include "../base/inventory_database.h"
#include "../base/recipe.h"

using namespace godot;

void RecipesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_recipe_selected", "recipe", "index"), &RecipesEditor::_on_recipe_selected);
	ClassDB::bind_method(D_METHOD("_on_recipe_popup_menu_requested", "at_position"), &RecipesEditor::_on_recipe_popup_menu_requested);

	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
}

void RecipesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

RecipesEditor::RecipesEditor() {
	editor_plugin = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

RecipesEditor::~RecipesEditor() {
}

void RecipesEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void RecipesEditor::_create_ui() {
	// Split container
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	// Recipe list editor
	recipe_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(recipe_list_editor);
	recipe_list_editor->set_custom_minimum_size(Vector2(256, 0));
	recipe_list_editor->connect("item_selected", callable_mp(this, &RecipesEditor::_on_recipe_selected));
	recipe_list_editor->connect("item_popup_menu_requested", callable_mp(this, &RecipesEditor::_on_recipe_popup_menu_requested));

	// Details scroll container
	details_scroll = memnew(ScrollContainer);
	hsplit_container->add_child(details_scroll);
	details_scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Details container
	details_container = memnew(VBoxContainer);
	details_scroll->add_child(details_container);

	// No selection label
	no_selection_label = memnew(Label);
	details_container->add_child(no_selection_label);
	no_selection_label->set_text("Select a recipe to edit its properties");
	no_selection_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	no_selection_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	no_selection_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	no_selection_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void RecipesEditor::load_from_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		TypedArray<Recipe> recipes = database->get_recipes();
		Array recipe_array;
		for (int i = 0; i < recipes.size(); i++) {
			recipe_array.append(recipes[i]);
		}
		recipe_list_editor->set_items(recipe_array);
	} else {
		recipe_list_editor->clear_items();
	}
	
	_clear_details();
}

void RecipesEditor::select_recipe(const Ref<Recipe> &p_recipe) {
	if (p_recipe.is_valid()) {
		recipe_list_editor->select_item(p_recipe);
		_update_details(p_recipe);
	}
}

void RecipesEditor::_update_details(const Ref<Recipe> &p_recipe) {
	current_recipe = p_recipe;
	
	// Clear existing details
	_clear_details();
	
	if (p_recipe.is_null()) {
		no_selection_label->set_visible(true);
		return;
	}
	
	no_selection_label->set_visible(false);
	
	// TODO: Create detailed property editor for recipe
	// For now, just show basic info
	Label *recipe_label = memnew(Label);
	details_container->add_child(recipe_label);
	recipe_label->set_text("Recipe: " + p_recipe->get_name());
	
	Label *id_label = memnew(Label);
	details_container->add_child(id_label);
	id_label->set_text("ID: " + p_recipe->get_id());
}

void RecipesEditor::_clear_details() {
	// Remove all children except the no selection label
	Array children = details_container->get_children();
	for (int i = 0; i < children.size(); i++) {
		Node *child = Object::cast_to<Node>(children[i]);
		if (child && child != no_selection_label) {
			details_container->remove_child(child);
			child->queue_free();
		}
	}
}

void RecipesEditor::_emit_removed(const Ref<Recipe> &p_recipe) {
	emit_signal("removed", p_recipe);
}

void RecipesEditor::_on_recipe_selected(const Variant &p_recipe, int p_index) {
	Ref<Recipe> recipe = p_recipe;
	_update_details(recipe);
}

void RecipesEditor::_on_recipe_popup_menu_requested(const Vector2 &p_position) {
	// TODO: Show context menu with options like remove, etc.
	print_line("Context menu requested for recipe at position: " + p_position);
}

#endif // TOOLS_ENABLED