/**
 * categories_in_item_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "categories_in_item_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_definition.h"
#include "../base/item_category.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/h_flow_container.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void CategoriesInItemEditor::_bind_methods() {
	// Public methods
	ClassDB::bind_method(D_METHOD("load_item", "database", "item"), &CategoriesInItemEditor::load_item);
	ClassDB::bind_method(D_METHOD("loading_categories"), &CategoriesInItemEditor::loading_categories);

	// Signal matching addon
	ADD_SIGNAL(MethodInfo("changed"));
}

void CategoriesInItemEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

CategoriesInItemEditor::CategoriesInItemEditor() {
	database = nullptr;
	
	// Initialize UI pointers
	categories_label = nullptr;
	scroll_container = nullptr;
	h_flow_container = nullptr;

	// Set minimum size matching addon
	set_custom_minimum_size(Vector2(0, 64));
}

CategoriesInItemEditor::~CategoriesInItemEditor() {
}

void CategoriesInItemEditor::_create_ui() {
	// Match .tscn structure exactly: HBoxContainer -> Label + ScrollContainer -> HFlowContainer
	
	// Categories label - matching addon with 160px width
	categories_label = memnew(Label);
	add_child(categories_label);
	categories_label->set_text("Categories");
	categories_label->set_custom_minimum_size(Vector2(160, 0));

	// ScrollContainer for the category checkboxes
	scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// HFlowContainer inside ScrollContainer for checkbox layout
	h_flow_container = memnew(HFlowContainer);
	scroll_container->add_child(h_flow_container);
	h_flow_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
}

void CategoriesInItemEditor::load_item(InventoryDatabase* p_database, const Ref<ItemDefinition>& p_item) {
	database = p_database;
	item = p_item;
	loading_categories();
}

void CategoriesInItemEditor::loading_categories() {
	_clear_categories();
	
	if (!database || item.is_null()) {
		return;
	}

	// Get all available categories from database
	TypedArray<ItemCategory> available_categories = database->get_item_categories();
	
	for (int i = 0; i < available_categories.size(); i++) {
		Ref<ItemCategory> category = available_categories[i];
		if (category.is_null()) {
			continue;
		}

		// Create CheckBox for this category
		CheckBox* category_checkbox = memnew(CheckBox);
		h_flow_container->add_child(category_checkbox);
		
		// Set properties matching addon
		category_checkbox->set_text(category->get_name());
		category_checkbox->set_button_icon(category->get_icon());
		category_checkbox->set_expand_icon(true);
		
		// Check if item is in this category
		TypedArray<ItemCategory> item_categories = item->get_categories();
		bool is_in_category = false;
		for (int j = 0; j < item_categories.size(); j++) {
			Ref<ItemCategory> item_cat = item_categories[j];
			if (item_cat.is_valid() && category.is_valid() && 
			    item_cat->get_id() == category->get_id()) {
				is_in_category = true;
				break;
			}
		}
		category_checkbox->set_pressed(is_in_category);

		// Connect signal with category bound
		category_checkbox->connect("toggled", callable_mp(this, &CategoriesInItemEditor::_on_category_toggled).bind(category));
		
		// Store reference for cleanup
		category_checkboxes.append(category_checkbox);
	}
}

void CategoriesInItemEditor::_clear_categories() {
	// Remove all existing checkboxes
	for (int i = 0; i < category_checkboxes.size(); i++) {
		CheckBox* checkbox = Object::cast_to<CheckBox>(category_checkboxes[i]);
		if (checkbox) {
			checkbox->queue_free();
		}
	}
	category_checkboxes.clear();
}

void CategoriesInItemEditor::_on_category_toggled(bool toggled, const Ref<ItemCategory>& category) {
	if (item.is_null() || category.is_null()) {
		return;
	}

	// Get current categories array
	TypedArray<ItemCategory> current_categories = item->get_categories();
	TypedArray<ItemCategory> new_categories;
	
	// Copy existing categories (except the one being toggled)
	for (int i = 0; i < current_categories.size(); i++) {
		Ref<ItemCategory> existing_cat = current_categories[i];
		if (existing_cat.is_valid() && category.is_valid() && 
		    existing_cat->get_id() != category->get_id()) {
			new_categories.append(existing_cat);
		}
	}
	
	// Add the category if toggled on
	if (toggled) {
		new_categories.append(category);
	}
	
	// Update item categories
	item->set_categories(new_categories);
	
	// Emit changed signal
	emit_signal("changed");
}

#endif // TOOLS_ENABLED