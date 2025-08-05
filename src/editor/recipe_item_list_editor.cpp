/**
 * recipe_item_list_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "recipe_item_list_editor.h"
#include "ingredient_item_in_recipe_item.h"

#include "../base/recipe.h"
#include "../base/inventory_database.h"
#include "../base/item_stack.h"
#include "../base/item_definition.h"
#include "../base/craft_station_type.h"

#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void RecipeItemListEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_panel_gui_input", "event"), &RecipeItemListEditor::_on_panel_gui_input);
	ClassDB::bind_method(D_METHOD("_on_delete_button_pressed"), &RecipeItemListEditor::_on_delete_button_pressed);
	
	ADD_SIGNAL(MethodInfo("selected"));
	ADD_SIGNAL(MethodInfo("request_remove", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
}

void RecipeItemListEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
			_setup_style();
		} break;
	}
}

RecipeItemListEditor::RecipeItemListEditor() {
	database = nullptr;
	
	set_custom_minimum_size(Vector2(320, 108));
	set_anchors_and_offsets_preset(Control::PRESET_TOP_LEFT);
}

RecipeItemListEditor::~RecipeItemListEditor() {
}

void RecipeItemListEditor::_create_ui() {
	// Main panel
	panel = memnew(Panel);
	add_child(panel);
	panel->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	panel->connect("gui_input", callable_mp(this, &RecipeItemListEditor::_on_panel_gui_input));
	
	// Margin container
	margin_container = memnew(MarginContainer);
	panel->add_child(margin_container);
	margin_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	margin_container->add_theme_constant_override("margin_left", 4);
	margin_container->add_theme_constant_override("margin_top", 2);
	margin_container->add_theme_constant_override("margin_right", 4);
	margin_container->add_theme_constant_override("margin_bottom", 2);
	
	// Main VBox container
	main_container = memnew(VBoxContainer);
	margin_container->add_child(main_container);
	
	// Top spacer (matching GDScript)
	Control *top_spacer = memnew(Control);
	main_container->add_child(top_spacer);
	top_spacer->set_custom_minimum_size(Vector2(0, 4));
	
	// PRODUCTS SECTION (horizontal layout like GDScript)
	HBoxContainer *products_section = memnew(HBoxContainer);
	main_container->add_child(products_section);
	
	Label *products_label = memnew(Label);
	products_section->add_child(products_label);
	products_label->set_custom_minimum_size(Vector2(128, 0));
	products_label->set_text("Products");
	
	products_list = memnew(HBoxContainer);
	products_section->add_child(products_list);
	products_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// More info section (craft station icon and delete button)
	HBoxContainer *more_infos = memnew(HBoxContainer);
	products_section->add_child(more_infos);
	more_infos->set_custom_minimum_size(Vector2(0, 32));
	more_infos->add_theme_constant_override("separation", 0);
	
	craft_station_icon = memnew(TextureRect);
	more_infos->add_child(craft_station_icon);
	craft_station_icon->set_custom_minimum_size(Vector2(24, 24));
	craft_station_icon->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	craft_station_icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	
	delete_button = memnew(Button);
	more_infos->add_child(delete_button);
	delete_button->set_custom_minimum_size(Vector2(24, 24));
	delete_button->set_flat(true);
	delete_button->set_tooltip_text("Delete");
	delete_button->connect("pressed", callable_mp(this, &RecipeItemListEditor::_on_delete_button_pressed));
	
	// INGREDIENTS SECTION (horizontal layout like GDScript)
	HBoxContainer *ingredients_section = memnew(HBoxContainer);
	main_container->add_child(ingredients_section);
	
	Label *ingredients_label = memnew(Label);
	ingredients_section->add_child(ingredients_label);
	ingredients_label->set_custom_minimum_size(Vector2(128, 0));
	ingredients_label->set_text("Ingredients");
	
	ingredients_list = memnew(HBoxContainer);
	ingredients_section->add_child(ingredients_list);
	ingredients_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// REQUIRED ITEMS SECTION (horizontal layout like GDScript)
	HBoxContainer *required_section = memnew(HBoxContainer);
	main_container->add_child(required_section);
	
	Label *required_label = memnew(Label);
	required_section->add_child(required_label);
	required_label->set_custom_minimum_size(Vector2(128, 0));
	required_label->set_text("Extra Req. Items");
	
	required_item_list = memnew(HBoxContainer);
	required_section->add_child(required_item_list);
	required_item_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// More info section 2 (time info)
	HBoxContainer *more_infos2 = memnew(HBoxContainer);
	required_section->add_child(more_infos2);
	more_infos2->set_custom_minimum_size(Vector2(0, 32));
	more_infos2->add_theme_constant_override("separation", 0);
	
	time_label = memnew(Label);
	more_infos2->add_child(time_label);
	time_label->set_custom_minimum_size(Vector2(64, 0));
	time_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	time_label->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	time_label->set_text("10 Seconds");
	time_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
	time_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	time_label->set_uppercase(true);
	
	time_icon = memnew(TextureRect);
	more_infos2->add_child(time_icon);
	time_icon->set_custom_minimum_size(Vector2(24, 24));
	time_icon->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	time_icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	time_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);
	
	// Bottom separator
	HSeparator *separator = memnew(HSeparator);
	main_container->add_child(separator);
	separator->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void RecipeItemListEditor::_setup_style() {
	// Set up theme icons if available
	if (has_theme_icon("Timer", "EditorIcons")) {
		time_icon->set_texture(get_theme_icon("Timer", "EditorIcons"));
	}
	
	if (has_theme_icon("Remove", "EditorIcons")) {
		delete_button->set_button_icon(get_theme_icon("Remove", "EditorIcons"));
	}
	
	// Create style box for selection (matching GDScript structure)
	Ref<StyleBoxFlat> style_box = memnew(StyleBoxFlat);
	style_box->set_bg_color(Color(0, 0, 0, 1)); // Black background like in GDScript
	style_box->set_corner_radius_all(4);
	panel->add_theme_stylebox_override("panel", style_box);
	
	unselect(); // Set initial unselected state
}

void RecipeItemListEditor::load_recipe(const Ref<Recipe> &p_recipe, InventoryDatabase *p_database) {
	recipe = p_recipe;
	database = p_database;
	
	update_recipe();
}

void RecipeItemListEditor::update_recipe() {
	if (recipe.is_null()) {
		return;
	}
	
	// Update time display (matching GDScript format)
	time_label->set_text(String::num(recipe->get_time_to_craft()) + " secs");
	
	// Update craft station icon
	Ref<CraftStationType> station = recipe->get_station();
	if (station.is_valid() && station->get_icon().is_valid()) {
		craft_station_icon->set_texture(station->get_icon());
	} else {
		craft_station_icon->set_texture(Ref<Texture2D>());
	}
	
	_update_recipe_display();
}

void RecipeItemListEditor::_update_recipe_display() {
	_clear_item_lists();
	
	if (recipe.is_null() || !database) {
		return;
	}
	
	// Display ingredients using IngredientItemInRecipeItem components
	TypedArray<ItemStack> ingredients = recipe->get_ingredients();
	for (int i = 0; i < ingredients.size(); i++) {
		Ref<ItemStack> ingredient = ingredients[i];
		if (ingredient.is_valid()) {
			IngredientItemInRecipeItem *ingredient_item = memnew(IngredientItemInRecipeItem);
			ingredients_list->add_child(ingredient_item);
			ingredient_item->setup(database, ingredient);
		}
	}
	
	// Display products using IngredientItemInRecipeItem components
	TypedArray<ItemStack> products = recipe->get_products();
	for (int i = 0; i < products.size(); i++) {
		Ref<ItemStack> product = products[i];
		if (product.is_valid()) {
			IngredientItemInRecipeItem *product_item = memnew(IngredientItemInRecipeItem);
			products_list->add_child(product_item);
			product_item->setup(database, product);
		}
	}
	
	// Display required items using IngredientItemInRecipeItem components
	TypedArray<ItemStack> required_items = recipe->get_required_items();
	for (int i = 0; i < required_items.size(); i++) {
		Ref<ItemStack> required_item = required_items[i];
		if (required_item.is_valid()) {
			IngredientItemInRecipeItem *required_item_component = memnew(IngredientItemInRecipeItem);
			required_item_list->add_child(required_item_component);
			required_item_component->setup(database, required_item);
		}
	}
}

void RecipeItemListEditor::_clear_item_lists() {
	// Clear ingredients
	Array ingredients_children = ingredients_list->get_children();
	for (int i = 0; i < ingredients_children.size(); i++) {
		Node *child = Object::cast_to<Node>(ingredients_children[i]);
		if (child) {
			ingredients_list->remove_child(child);
			child->queue_free();
		}
	}
	
	// Clear products
	Array products_children = products_list->get_children();
	for (int i = 0; i < products_children.size(); i++) {
		Node *child = Object::cast_to<Node>(products_children[i]);
		if (child) {
			products_list->remove_child(child);
			child->queue_free();
		}
	}
	
	// Clear required items
	Array required_children = required_item_list->get_children();
	for (int i = 0; i < required_children.size(); i++) {
		Node *child = Object::cast_to<Node>(required_children[i]);
		if (child) {
			required_item_list->remove_child(child);
			child->queue_free();
		}
	}
}

void RecipeItemListEditor::select() {
	Ref<StyleBoxFlat> style_box = panel->get_theme_stylebox("panel");
	if (style_box.is_valid()) {
		if (has_theme_color("box_selection_fill_color", "Editor")) {
			style_box->set_bg_color(get_theme_color("box_selection_fill_color", "Editor"));
		} else {
			style_box->set_bg_color(Color(0.3, 0.5, 0.8, 0.5)); // Fallback blue
		}
	}
	emit_signal("selected");
}

void RecipeItemListEditor::unselect() {
	Ref<StyleBoxFlat> style_box = panel->get_theme_stylebox("panel");
	if (style_box.is_valid()) {
		if (has_theme_color("dark_color_1", "Editor")) {
			style_box->set_bg_color(get_theme_color("dark_color_1", "Editor"));
		} else {
			style_box->set_bg_color(Color(0.2, 0.2, 0.2, 1.0)); // Fallback dark
		}
	}
}

void RecipeItemListEditor::_on_panel_gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseButton> mouse_event = p_event;
	if (mouse_event.is_valid() && mouse_event->is_pressed()) {
		if (mouse_event->get_button_index() == MOUSE_BUTTON_LEFT) {
			select();
		}
	}
}

void RecipeItemListEditor::_on_delete_button_pressed() {
	if (recipe.is_valid()) {
		emit_signal("request_remove", recipe);
	}
}

#endif // TOOLS_ENABLED