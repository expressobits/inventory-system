/**
 * recipe_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "recipe_editor.h"

#include "../base/inventory_database.h"
#include "../base/recipe.h"
#include "../base/item_stack.h"
#include "../base/craft_station_type.h"
#include "base/item_stack_selector.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/h_box_container.hpp>

using namespace godot;

void RecipeEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_time_to_craft_changed", "value"), &RecipeEditor::_on_time_to_craft_changed);
	ClassDB::bind_method(D_METHOD("_on_craft_station_selected", "index"), &RecipeEditor::_on_craft_station_selected);
	ClassDB::bind_method(D_METHOD("_on_new_ingredient_pressed"), &RecipeEditor::_on_new_ingredient_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_product_pressed"), &RecipeEditor::_on_new_product_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_required_item_pressed"), &RecipeEditor::_on_new_required_item_pressed);
	ClassDB::bind_method(D_METHOD("_on_ingredient_changed", "item_stack"), &RecipeEditor::_on_ingredient_changed);
	ClassDB::bind_method(D_METHOD("_on_product_changed", "item_stack"), &RecipeEditor::_on_product_changed);
	ClassDB::bind_method(D_METHOD("_on_required_item_changed", "item_stack"), &RecipeEditor::_on_required_item_changed);
	ClassDB::bind_method(D_METHOD("_on_remove_ingredient_pressed", "button"), &RecipeEditor::_on_remove_ingredient_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_product_pressed", "button"), &RecipeEditor::_on_remove_product_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_required_item_pressed", "button"), &RecipeEditor::_on_remove_required_item_pressed);
	
	ADD_SIGNAL(MethodInfo("changed"));
	ADD_SIGNAL(MethodInfo("changed_products", PropertyInfo(Variant::OBJECT, "recipe", PROPERTY_HINT_RESOURCE_TYPE, "Recipe")));
}

void RecipeEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

RecipeEditor::RecipeEditor() {
	editor_plugin = nullptr;
	database = nullptr;
	signals_connected = false;
	
	set_custom_minimum_size(Vector2(0, 384));
	set_anchors_preset(Control::PRESET_TOP_LEFT);
	set_size(Vector2(0, 384));
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
}

RecipeEditor::~RecipeEditor() {
}

void RecipeEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void RecipeEditor::_create_ui() {
	// Main margin container (matching GDScript structure)
	margin_container = memnew(MarginContainer);
	add_child(margin_container);
	margin_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	margin_container->add_theme_constant_override("margin_left", 0);
	margin_container->add_theme_constant_override("margin_top", 0);
	margin_container->add_theme_constant_override("margin_right", 0);
	margin_container->add_theme_constant_override("margin_bottom", 0);
	margin_container->set_visible(false);
	
	// Second margin container
	MarginContainer *inner_margin = memnew(MarginContainer);
	margin_container->add_child(inner_margin);
	inner_margin->add_theme_constant_override("margin_left", 0);
	inner_margin->add_theme_constant_override("margin_top", 0);
	inner_margin->add_theme_constant_override("margin_right", 0);
	inner_margin->add_theme_constant_override("margin_bottom", 0);
	
	// Main VBox container
	main_container = memnew(VBoxContainer);
	inner_margin->add_child(main_container);
	
	// First separator (at top, matching GDScript)
	HSeparator *separator1 = memnew(HSeparator);
	main_container->add_child(separator1);
	
	// PRODUCTS SECTION (FIRST - matching GDScript layout)
	HBoxContainer *products_section = memnew(HBoxContainer);
	main_container->add_child(products_section);
	
	Label *products_label = memnew(Label);
	products_section->add_child(products_label);
	products_label->set_custom_minimum_size(Vector2(160, 0));
	products_label->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	products_label->set_text("Products");
	
	products_container = memnew(VBoxContainer);
	products_section->add_child(products_container);
	products_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	new_product_button = memnew(Button);
	products_container->add_child(new_product_button);
	new_product_button->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	new_product_button->set_text("Add New Product");
	
	// Separator
	HSeparator *separator2 = memnew(HSeparator);
	main_container->add_child(separator2);
	
	// INGREDIENTS SECTION (SECOND)
	HBoxContainer *ingredients_section = memnew(HBoxContainer);
	main_container->add_child(ingredients_section);
	
	Label *ingredients_label = memnew(Label);
	ingredients_section->add_child(ingredients_label);
	ingredients_label->set_custom_minimum_size(Vector2(160, 0));
	ingredients_label->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	ingredients_label->set_text("Ingredients");
	
	ingredients_container = memnew(VBoxContainer);
	ingredients_section->add_child(ingredients_container);
	ingredients_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	new_ingredient_button = memnew(Button);
	ingredients_container->add_child(new_ingredient_button);
	new_ingredient_button->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	new_ingredient_button->set_text("Add New Ingredient");
	
	// Separator
	HSeparator *separator3 = memnew(HSeparator);
	main_container->add_child(separator3);
	
	// REQUIRED ITEMS SECTION (THIRD)
	HBoxContainer *required_section = memnew(HBoxContainer);
	main_container->add_child(required_section);
	
	Label *required_label = memnew(Label);
	required_section->add_child(required_label);
	required_label->set_custom_minimum_size(Vector2(160, 0));
	required_label->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	required_label->set_text("Required Items");
	
	required_items_container = memnew(VBoxContainer);
	required_section->add_child(required_items_container);
	required_items_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	new_required_item_button = memnew(Button);
	required_items_container->add_child(new_required_item_button);
	new_required_item_button->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	new_required_item_button->set_text("Add Required Item (Items not used by craft)");
	
	// Separator
	HSeparator *separator4 = memnew(HSeparator);
	main_container->add_child(separator4);
	
	// TIME TO CRAFT SECTION (FOURTH)
	HBoxContainer *time_section = memnew(HBoxContainer);
	main_container->add_child(time_section);
	
	Label *time_label = memnew(Label);
	time_section->add_child(time_label);
	time_label->set_custom_minimum_size(Vector2(160, 0));
	time_label->set_text("Time To Craft");
	
	time_to_craft_spin_box = memnew(SpinBox);
	time_section->add_child(time_to_craft_spin_box);
	time_to_craft_spin_box->set_max(100000000);
	time_to_craft_spin_box->set_min(0.1);
	time_to_craft_spin_box->set_step(0.1);
	time_to_craft_spin_box->set_value(1.0);
	
	// CRAFT STATION TYPE SECTION (LAST)
	HBoxContainer *station_section = memnew(HBoxContainer);
	main_container->add_child(station_section);
	station_section->set_custom_minimum_size(Vector2(0, 32));
	
	Label *station_label = memnew(Label);
	station_section->add_child(station_label);
	station_label->set_custom_minimum_size(Vector2(160, 0));
	station_label->set_text("Craft Station Type");
	
	craft_station_type_option_button = memnew(OptionButton);
	station_section->add_child(craft_station_type_option_button);
	craft_station_type_option_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	craft_station_type_option_button->set_clip_contents(true);
	
	// Spacer at bottom
	Control *spacer = memnew(Control);
	main_container->add_child(spacer);
	spacer->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void RecipeEditor::_connect_signals() {
	if (signals_connected) {
		return;
	}
	
	time_to_craft_spin_box->connect("value_changed", callable_mp(this, &RecipeEditor::_on_time_to_craft_changed));
	craft_station_type_option_button->connect("item_selected", callable_mp(this, &RecipeEditor::_on_craft_station_selected));
	new_ingredient_button->connect("pressed", callable_mp(this, &RecipeEditor::_on_new_ingredient_pressed));
	new_product_button->connect("pressed", callable_mp(this, &RecipeEditor::_on_new_product_pressed));
	new_required_item_button->connect("pressed", callable_mp(this, &RecipeEditor::_on_new_required_item_pressed));
	
	signals_connected = true;
}

void RecipeEditor::_disconnect_signals() {
	if (!signals_connected) {
		return;
	}
	
	time_to_craft_spin_box->disconnect("value_changed", callable_mp(this, &RecipeEditor::_on_time_to_craft_changed));
	craft_station_type_option_button->disconnect("item_selected", callable_mp(this, &RecipeEditor::_on_craft_station_selected));
	new_ingredient_button->disconnect("pressed", callable_mp(this, &RecipeEditor::_on_new_ingredient_pressed));
	new_product_button->disconnect("pressed", callable_mp(this, &RecipeEditor::_on_new_product_pressed));
	new_required_item_button->disconnect("pressed", callable_mp(this, &RecipeEditor::_on_new_required_item_pressed));
	
	signals_connected = false;
}

void RecipeEditor::load_recipe(const Ref<Recipe> &p_recipe, InventoryDatabase *p_database) {
	_disconnect_signals();
	
	recipe = p_recipe;
	database = p_database;
	
	if (recipe.is_null() || !database) {
		margin_container->set_visible(false);
		return;
	}
	
	margin_container->set_visible(true);
	
	// Update UI with recipe data
	time_to_craft_spin_box->set_value(recipe->get_time_to_craft());
	
	_setup_station();
	_setup_ingredients();
	_setup_products();
	_setup_required_items();
	
	_connect_signals();
}

void RecipeEditor::reload() {
	if (recipe.is_valid() && database) {
		load_recipe(recipe, database);
	}
}

void RecipeEditor::_setup_station() {
	craft_station_type_option_button->clear();
	stations_list.clear();
	
	// Add "No station" option
	craft_station_type_option_button->add_item("No station");
	stations_list.append(Variant()); // null reference
	
	if (!database) {
		return;
	}
	
	// Add available stations
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
			
			craft_station_type_option_button->add_item(display_name);
			stations_list.append(station);
			
			// Select current station
			if (recipe.is_valid() && recipe->get_station() == station) {
				craft_station_type_option_button->select(i + 1); // +1 for "No station" option
			}
		}
	}
	
	// If no station is selected, select "No station"
	if (recipe.is_valid() && recipe->get_station().is_null()) {
		craft_station_type_option_button->select(0);
	}
}

void RecipeEditor::_setup_ingredients() {
	_clear_ingredient_editors();
	
	if (recipe.is_null()) {
		return;
	}
	
	// Create ItemStackSelector widgets for each ingredient
	TypedArray<ItemStack> ingredients = recipe->get_ingredients();
	for (int i = 0; i < ingredients.size(); i++) {
		Ref<ItemStack> ingredient = ingredients[i];
		if (ingredient.is_valid()) {
			// Create container for ingredient selector + remove button
			HBoxContainer *ingredient_container = memnew(HBoxContainer);
			ingredients_container->add_child(ingredient_container);
			
			// Create ItemStackSelector
			ItemStackSelector *selector = memnew(ItemStackSelector);
			ingredient_container->add_child(selector);
			selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			selector->setup(ingredient, database);
			selector->connect("changed", callable_mp(this, &RecipeEditor::_on_ingredient_changed));
			
			// Create remove button
			_create_remove_button(ingredient_container, "Remove this ingredient", 
				callable_mp(this, &RecipeEditor::_on_remove_ingredient_pressed));
			
			ingredient_editors.append(ingredient_container);
		}
	}
}

void RecipeEditor::_setup_products() {
	_clear_product_editors();
	
	if (recipe.is_null()) {
		return;
	}
	
	// Create ItemStackSelector widgets for each product
	TypedArray<ItemStack> products = recipe->get_products();
	for (int i = 0; i < products.size(); i++) {
		Ref<ItemStack> product = products[i];
		if (product.is_valid()) {
			// Create container for product selector + remove button
			HBoxContainer *product_container = memnew(HBoxContainer);
			products_container->add_child(product_container);
			
			// Create ItemStackSelector
			ItemStackSelector *selector = memnew(ItemStackSelector);
			product_container->add_child(selector);
			selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			selector->setup(product, database);
			selector->connect("changed", callable_mp(this, &RecipeEditor::_on_product_changed));
			
			// Create remove button
			_create_remove_button(product_container, "Remove this product", 
				callable_mp(this, &RecipeEditor::_on_remove_product_pressed));
			
			product_editors.append(product_container);
		}
	}
}

void RecipeEditor::_setup_required_items() {
	_clear_required_item_editors();
	
	if (recipe.is_null()) {
		return;
	}
	
	// Create ItemStackSelector widgets for each required item
	TypedArray<ItemStack> required_items = recipe->get_required_items();
	for (int i = 0; i < required_items.size(); i++) {
		Ref<ItemStack> required_item = required_items[i];
		if (required_item.is_valid()) {
			// Create container for required item selector + remove button
			HBoxContainer *required_container = memnew(HBoxContainer);
			required_items_container->add_child(required_container);
			
			// Create ItemStackSelector
			ItemStackSelector *selector = memnew(ItemStackSelector);
			required_container->add_child(selector);
			selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			selector->setup(required_item, database);
			selector->connect("changed", callable_mp(this, &RecipeEditor::_on_required_item_changed));
			
			// Create remove button
			_create_remove_button(required_container, "Remove this required item", 
				callable_mp(this, &RecipeEditor::_on_remove_required_item_pressed));
			
			required_item_editors.append(required_container);
		}
	}
}

void RecipeEditor::_clear_ingredient_editors() {
	for (int i = 0; i < ingredient_editors.size(); i++) {
		Node *editor = Object::cast_to<Node>(ingredient_editors[i]);
		if (editor) {
			ingredients_container->remove_child(editor);
			editor->queue_free();
		}
	}
	ingredient_editors.clear();
}

void RecipeEditor::_clear_product_editors() {
	for (int i = 0; i < product_editors.size(); i++) {
		Node *editor = Object::cast_to<Node>(product_editors[i]);
		if (editor) {
			products_container->remove_child(editor);
			editor->queue_free();
		}
	}
	product_editors.clear();
}

void RecipeEditor::_clear_required_item_editors() {
	for (int i = 0; i < required_item_editors.size(); i++) {
		Node *editor = Object::cast_to<Node>(required_item_editors[i]);
		if (editor) {
			required_items_container->remove_child(editor);
			editor->queue_free();
		}
	}
	required_item_editors.clear();
}

Button* RecipeEditor::_create_remove_button(HBoxContainer* p_parent, const String& p_tooltip, const Callable& p_callback) {
	Button *remove_button = memnew(Button);
	p_parent->add_child(remove_button);
	remove_button->set_button_icon(get_theme_icon("Remove", "EditorIcons"));
	remove_button->set_flat(true);
	remove_button->set_tooltip_text(p_tooltip);
	remove_button->connect("pressed", p_callback.bind(remove_button));
	return remove_button;
}

// Signal handlers
void RecipeEditor::_on_time_to_craft_changed(double p_value) {
	if (recipe.is_valid()) {
		recipe->set_time_to_craft((float)p_value);
		emit_signal("changed");
	}
}

void RecipeEditor::_on_craft_station_selected(int p_index) {
	if (recipe.is_valid() && p_index >= 0 && p_index < stations_list.size()) {
		Variant station_variant = stations_list[p_index];
		if (station_variant.get_type() == Variant::NIL) {
			recipe->set_station(Ref<CraftStationType>());
		} else {
			Ref<CraftStationType> station = station_variant;
			recipe->set_station(station);
		}
		emit_signal("changed");
	}
}

void RecipeEditor::_on_new_ingredient_pressed() {
	if (recipe.is_valid() && database) {
		Ref<ItemStack> new_ingredient = memnew(ItemStack);
		new_ingredient->set_amount(1);
		
		// Set first available item if exists
		TypedArray<ItemDefinition> items = database->get_items();
		if (items.size() > 0) {
			Ref<ItemDefinition> first_item = items[0];
			if (first_item.is_valid()) {
				new_ingredient->set_item_id(first_item->get_id());
			}
		}
		
		// Add to ingredients array
		TypedArray<ItemStack> ingredients = recipe->get_ingredients();
		ingredients.append(new_ingredient);
		recipe->set_ingredients(ingredients);
		
		_setup_ingredients();
		emit_signal("changed");
	}
}

void RecipeEditor::_on_new_product_pressed() {
	if (recipe.is_valid() && database) {
		Ref<ItemStack> new_product = memnew(ItemStack);
		new_product->set_amount(1);
		
		// Set first available item if exists
		TypedArray<ItemDefinition> items = database->get_items();
		if (items.size() > 0) {
			Ref<ItemDefinition> first_item = items[0];
			if (first_item.is_valid()) {
				new_product->set_item_id(first_item->get_id());
			}
		}
		
		// Add to products array
		TypedArray<ItemStack> products = recipe->get_products();
		products.append(new_product);
		recipe->set_products(products);
		
		_setup_products();
		emit_signal("changed");
		emit_signal("changed_products", recipe);
	}
}

void RecipeEditor::_on_new_required_item_pressed() {
	if (recipe.is_valid() && database) {
		Ref<ItemStack> new_required_item = memnew(ItemStack);
		new_required_item->set_amount(1);
		
		// Set first available item if exists
		TypedArray<ItemDefinition> items = database->get_items();
		if (items.size() > 0) {
			Ref<ItemDefinition> first_item = items[0];
			if (first_item.is_valid()) {
				new_required_item->set_item_id(first_item->get_id());
			}
		}
		
		// Add to required items array
		TypedArray<ItemStack> required_items = recipe->get_required_items();
		required_items.append(new_required_item);
		recipe->set_required_items(required_items);
		
		_setup_required_items();
		emit_signal("changed");
	}
}

void RecipeEditor::_on_ingredient_changed(const Ref<ItemStack> &p_item_stack) {
	emit_signal("changed");
}

void RecipeEditor::_on_product_changed(const Ref<ItemStack> &p_item_stack) {
	emit_signal("changed");
	emit_signal("changed_products", recipe);
}

void RecipeEditor::_on_required_item_changed(const Ref<ItemStack> &p_item_stack) {
	emit_signal("changed");
}

void RecipeEditor::_on_remove_ingredient_pressed(Button *p_button) {
	if (!recipe.is_valid() || !p_button) {
		return;
	}
	
	// Find the container that contains this button
	HBoxContainer *container = Object::cast_to<HBoxContainer>(p_button->get_parent());
	if (!container) {
		return;
	}
	
	// Find the ItemStackSelector in the container
	ItemStackSelector *selector = nullptr;
	for (int i = 0; i < container->get_child_count(); i++) {
		selector = Object::cast_to<ItemStackSelector>(container->get_child(i));
		if (selector) {
			break;
		}
	}
	
	if (!selector) {
		return;
	}
	
	// Get the ItemStack from the selector
	Ref<ItemStack> item_stack = selector->get_item_stack();
	if (!item_stack.is_valid()) {
		return;
	}
	
	// Remove from the recipe's ingredients array
	TypedArray<ItemStack> ingredients = recipe->get_ingredients();
	for (int i = 0; i < ingredients.size(); i++) {
		Ref<ItemStack> ingredient = ingredients[i];
		if (ingredient == item_stack) {
			ingredients.remove_at(i);
			break;
		}
	}
	recipe->set_ingredients(ingredients);
	
	_setup_ingredients();
	emit_signal("changed");
}

void RecipeEditor::_on_remove_product_pressed(Button *p_button) {
	if (!recipe.is_valid() || !p_button) {
		return;
	}
	
	// Find the container that contains this button
	HBoxContainer *container = Object::cast_to<HBoxContainer>(p_button->get_parent());
	if (!container) {
		return;
	}
	
	// Find the ItemStackSelector in the container
	ItemStackSelector *selector = nullptr;
	for (int i = 0; i < container->get_child_count(); i++) {
		selector = Object::cast_to<ItemStackSelector>(container->get_child(i));
		if (selector) {
			break;
		}
	}
	
	if (!selector) {
		return;
	}
	
	// Get the ItemStack from the selector
	Ref<ItemStack> item_stack = selector->get_item_stack();
	if (!item_stack.is_valid()) {
		return;
	}
	
	// Remove from the recipe's products array
	TypedArray<ItemStack> products = recipe->get_products();
	for (int i = 0; i < products.size(); i++) {
		Ref<ItemStack> product = products[i];
		if (product == item_stack) {
			products.remove_at(i);
			break;
		}
	}
	recipe->set_products(products);
	
	_setup_products();
	emit_signal("changed");
	emit_signal("changed_products", recipe);
}

void RecipeEditor::_on_remove_required_item_pressed(Button *p_button) {
	if (!recipe.is_valid() || !p_button) {
		return;
	}
	
	// Find the container that contains this button
	HBoxContainer *container = Object::cast_to<HBoxContainer>(p_button->get_parent());
	if (!container) {
		return;
	}
	
	// Find the ItemStackSelector in the container
	ItemStackSelector *selector = nullptr;
	for (int i = 0; i < container->get_child_count(); i++) {
		selector = Object::cast_to<ItemStackSelector>(container->get_child(i));
		if (selector) {
			break;
		}
	}
	
	if (!selector) {
		return;
	}
	
	// Get the ItemStack from the selector
	Ref<ItemStack> item_stack = selector->get_item_stack();
	if (!item_stack.is_valid()) {
		return;
	}
	
	// Remove from the recipe's required items array
	TypedArray<ItemStack> required_items = recipe->get_required_items();
	for (int i = 0; i < required_items.size(); i++) {
		Ref<ItemStack> required_item = required_items[i];
		if (required_item == item_stack) {
			required_items.remove_at(i);
			break;
		}
	}
	recipe->set_required_items(required_items);
	
	_setup_required_items();
	emit_signal("changed");
}

#endif // TOOLS_ENABLED