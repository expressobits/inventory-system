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

#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/h_box_container.hpp>

using namespace godot;

void RecipesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_recipe_selected", "recipe", "index"), &RecipesEditor::_on_recipe_selected);
	ClassDB::bind_method(D_METHOD("_on_recipe_popup_menu_requested", "at_position"), &RecipesEditor::_on_recipe_popup_menu_requested);
	
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_id_changed", "text"), &RecipesEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_id_focus_exited"), &RecipesEditor::_on_id_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_time_changed", "value"), &RecipesEditor::_on_time_changed);
	ClassDB::bind_method(D_METHOD("_on_station_selected", "index"), &RecipesEditor::_on_station_selected);

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
	
	// Create proper editor UI components
	
	// ID field
	Label *id_label = memnew(Label);
	details_container->add_child(id_label);
	id_label->set_text("Recipe ID:");
	
	LineEdit *id_edit = memnew(LineEdit);
	details_container->add_child(id_edit);
	id_edit->set_text(p_recipe->get_id());
	id_edit->set_placeholder("Enter unique recipe ID");
	id_edit->connect("text_submitted", callable_mp(this, &RecipesEditor::_on_id_changed));
	id_edit->connect("focus_exited", callable_mp(this, &RecipesEditor::_on_id_focus_exited));
	
	// Time to craft
	Label *time_label = memnew(Label);
	details_container->add_child(time_label);
	time_label->set_text("Time to Craft (seconds):");
	
	SpinBox *time_spin = memnew(SpinBox);
	details_container->add_child(time_spin);
	time_spin->set_min(0.1);
	time_spin->set_max(999.0);
	time_spin->set_step(0.1);
	time_spin->set_value(p_recipe->get_time_to_craft());
	time_spin->connect("value_changed", callable_mp(this, &RecipesEditor::_on_time_changed));
	
	// Craft station
	Label *station_label = memnew(Label);
	details_container->add_child(station_label);
	station_label->set_text("Required Craft Station:");
	
	OptionButton *station_option = memnew(OptionButton);
	details_container->add_child(station_option);
	station_option->add_item("(None)", -1);
	
	// Populate craft stations from database
	if (database.is_valid()) {
		TypedArray<CraftStationType> stations = database->get_stations_type();
		for (int i = 0; i < stations.size(); i++) {
			Ref<CraftStationType> station = stations[i];
			if (station.is_valid()) {
				String display_name = station->get_name();
				if (display_name.is_empty()) {
					display_name = station->get_id();
				}
				if (display_name.is_empty()) {
					display_name = "Unnamed Station";
				}
				station_option->add_item(display_name, i);
				
				// Select current station
				if (p_recipe->get_station() == station) {
					station_option->select(i + 1); // +1 because of "None" option
				}
			}
		}
	}
	station_option->connect("item_selected", callable_mp(this, &RecipesEditor::_on_station_selected));
	
	// Ingredients section
	HSeparator *separator1 = memnew(HSeparator);
	details_container->add_child(separator1);
	
	Label *ingredients_title = memnew(Label);
	details_container->add_child(ingredients_title);
	ingredients_title->set_text("Ingredients:");
	ingredients_title->add_theme_font_size_override("font_size", 14);
	
	// List ingredients
	TypedArray<ItemStack> ingredients = p_recipe->get_ingredients();
	for (int i = 0; i < ingredients.size(); i++) {
		Ref<ItemStack> ingredient = ingredients[i];
		if (ingredient.is_valid() && database.is_valid()) {
			Ref<ItemDefinition> item_def = database->get_item(ingredient->get_item_id());
			HBoxContainer *ingredient_row = memnew(HBoxContainer);
			details_container->add_child(ingredient_row);
			
			Label *ingredient_label = memnew(Label);
			ingredient_row->add_child(ingredient_label);
			String item_name = item_def.is_valid() ? item_def->get_name() : ingredient->get_item_id();
			ingredient_label->set_text(String::num(ingredient->get_amount()) + "x " + item_name);
		}
	}
	
	// Products section
	HSeparator *separator2 = memnew(HSeparator);
	details_container->add_child(separator2);
	
	Label *products_title = memnew(Label);
	details_container->add_child(products_title);
	products_title->set_text("Products:");
	products_title->add_theme_font_size_override("font_size", 14);
	
	// List products
	TypedArray<ItemStack> products = p_recipe->get_products();
	for (int i = 0; i < products.size(); i++) {
		Ref<ItemStack> product = products[i];
		if (product.is_valid() && database.is_valid()) {
			Ref<ItemDefinition> item_def = database->get_item(product->get_item_id());
			HBoxContainer *product_row = memnew(HBoxContainer);
			details_container->add_child(product_row);
			
			Label *product_label = memnew(Label);
			product_row->add_child(product_label);
			String item_name = item_def.is_valid() ? item_def->get_name() : product->get_item_id();
			product_label->set_text(String::num(product->get_amount()) + "x " + item_name);
		}
	}
	
	// Required items section (tools/equipment that are not consumed)
	if (!p_recipe->get_required_items().is_empty()) {
		HSeparator *separator3 = memnew(HSeparator);
		details_container->add_child(separator3);
		
		Label *required_title = memnew(Label);
		details_container->add_child(required_title);
		required_title->set_text("Required Items (not consumed):");
		required_title->add_theme_font_size_override("font_size", 14);
		
		TypedArray<ItemStack> required_items = p_recipe->get_required_items();
		for (int i = 0; i < required_items.size(); i++) {
			Ref<ItemStack> required_item = required_items[i];
			if (required_item.is_valid() && database.is_valid()) {
				Ref<ItemDefinition> item_def = database->get_item(required_item->get_item_id());
				HBoxContainer *required_row = memnew(HBoxContainer);
				details_container->add_child(required_row);
				
				Label *required_label = memnew(Label);
				required_row->add_child(required_label);
				String item_name = item_def.is_valid() ? item_def->get_name() : required_item->get_item_id();
				required_label->set_text(String::num(required_item->get_amount()) + "x " + item_name);
			}
		}
	}
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

// Property change handlers
void RecipesEditor::_on_id_changed(const String &p_text) {
	if (current_recipe.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter unique recipe ID") {
				current_recipe->set_id(line_edit->get_text());
				break;
			}
		}
	}
}

void RecipesEditor::_on_id_focus_exited() {
	// Handle focus exit - just trigger the change handler
	_on_id_changed("");
}

void RecipesEditor::_on_time_changed(double p_value) {
	if (current_recipe.is_valid()) {
		current_recipe->set_time_to_craft((float)p_value);
	}
}

void RecipesEditor::_on_station_selected(int p_index) {
	if (current_recipe.is_valid() && database.is_valid()) {
		if (p_index == 0) {
			// "None" selected
			current_recipe->set_station(Ref<CraftStationType>());
		} else {
			TypedArray<CraftStationType> stations = database->get_stations_type();
			int station_index = p_index - 1; // -1 because of "None" option
			if (station_index >= 0 && station_index < stations.size()) {
				current_recipe->set_station(stations[station_index]);
			}
		}
	}
}

#endif // TOOLS_ENABLED