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
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void CategoriesInItemEditor::_bind_methods() {
	// Public methods
	ClassDB::bind_method(D_METHOD("load_item", "database", "item"), &CategoriesInItemEditor::load_item);
	ClassDB::bind_method(D_METHOD("loading_categories"), &CategoriesInItemEditor::loading_categories);

	// Theme and UI methods
	ClassDB::bind_method(D_METHOD("_apply_theme"), &CategoriesInItemEditor::_apply_theme);

	// Signal handlers
	ClassDB::bind_method(D_METHOD("_on_category_selected", "index"), &CategoriesInItemEditor::_on_category_selected);
	ClassDB::bind_method(D_METHOD("_on_add_button_pressed"), &CategoriesInItemEditor::_on_add_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_button_pressed"), &CategoriesInItemEditor::_on_remove_button_pressed);

	// Signal matching addon
	ADD_SIGNAL(MethodInfo("changed"));
}

void CategoriesInItemEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
			_apply_theme();
		} break;
		case NOTIFICATION_THEME_CHANGED: {
			_apply_theme();
		} break;
	}
}

CategoriesInItemEditor::CategoriesInItemEditor() {
	database = nullptr;
	
	// Initialize UI pointers
	categories_label = nullptr;
	categories_container = nullptr;
	categories_list = nullptr;
	add_category_container = nullptr;
	available_categories_option = nullptr;
	add_button = nullptr;
	remove_button = nullptr;

	// Set minimum size to accommodate the new UI layout
	set_custom_minimum_size(Vector2(0, 140));
}

CategoriesInItemEditor::~CategoriesInItemEditor() {
}

void CategoriesInItemEditor::_create_ui() {
	// Container for the categories content - positioned to the right
	categories_container = memnew(VBoxContainer);
	add_child(categories_container);
	categories_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Categories label - matching addon with 160px width
	categories_label = memnew(Label);
	categories_container->add_child(categories_label);
	categories_label->set_text("Categories");
	categories_label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	categories_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	categories_label->set_custom_minimum_size(Vector2(160, 0));

	// Container for add category controls
	add_category_container = memnew(HBoxContainer);
	categories_container->add_child(add_category_container);
	add_category_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// OptionButton for selecting available categories
	available_categories_option = memnew(OptionButton);
	add_category_container->add_child(available_categories_option);
	available_categories_option->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	// available_categories_option->set_expand_icon(true);
	available_categories_option->add_theme_constant_override("icon_max_width", 16);
	available_categories_option->set_custom_minimum_size(Vector2(120, 32));

	add_button = memnew(Button);
	add_category_container->add_child(add_button);
	add_button->set_tooltip_text("Add");
	add_button->set_custom_minimum_size(Vector2(32, 32));
	add_button->set_flat(true);

	remove_button = memnew(Button);
	add_category_container->add_child(remove_button);
	remove_button->set_tooltip_text("Remove");
	remove_button->set_custom_minimum_size(Vector2(32, 32));
	remove_button->set_flat(true);
	remove_button->set_disabled(true);

	// ItemList for assigned categories only
	categories_list = memnew(ItemList);
	categories_container->add_child(categories_list);
	categories_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	categories_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	categories_list->set_select_mode(ItemList::SELECT_SINGLE); // Single selection for removal
	categories_list->set_fixed_icon_size(Vector2i(16, 16));
	categories_list->set_custom_minimum_size(Vector2(160, 80));

	// Connect signals
	categories_list->connect("item_selected", callable_mp(this, &CategoriesInItemEditor::_on_category_selected));
	add_button->connect("pressed", callable_mp(this, &CategoriesInItemEditor::_on_add_button_pressed));
	remove_button->connect("pressed", callable_mp(this, &CategoriesInItemEditor::_on_remove_button_pressed));
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

	// Store the item's current categories
	item_categories = item->get_categories().duplicate();
	
	// Update both the item list and available categories option
	_update_item_categories_list();
	_update_available_categories_option();
}

void CategoriesInItemEditor::_clear_categories() {
	if (categories_list) {
		categories_list->clear();
	}
	if (available_categories_option) {
		available_categories_option->clear();
	}
	item_categories.clear();
}

void CategoriesInItemEditor::_update_item_categories_list() {
	if (!categories_list) return;
	
	categories_list->clear();
	
	// Populate the ItemList with only the categories assigned to the item
	for (int i = 0; i < item_categories.size(); i++) {
		Ref<ItemCategory> category = item_categories[i];
		if (category.is_valid()) {
			categories_list->add_item(category->get_name());
			
			// Set icon if available
			Ref<Texture2D> icon = category->get_icon();
			if (icon.is_valid()) {
				categories_list->set_item_icon(i, icon);
			}
		}
	}
}

void CategoriesInItemEditor::_update_available_categories_option() {
	if (!available_categories_option || !database) return;
	
	available_categories_option->clear();
	available_categories_option->add_item("Select category to add...", -1);
	
	// Get all available categories from database
	TypedArray<ItemCategory> all_categories = database->get_item_categories();
	
	for (int i = 0; i < all_categories.size(); i++) {
		Ref<ItemCategory> category = all_categories[i];
		if (!category.is_valid()) continue;
		
		// Check if this category is already assigned to the item
		bool already_assigned = false;
		for (int j = 0; j < item_categories.size(); j++) {
			Ref<ItemCategory> item_cat = item_categories[j];
			if (item_cat.is_valid() && item_cat->get_id() == category->get_id()) {
				already_assigned = true;
				break;
			}
		}
		
		// Only add to option button if not already assigned
		if (!already_assigned) {
			available_categories_option->add_icon_item(category->get_icon(), category->get_name(), i);
			// Store the category reference in the metadata
			available_categories_option->set_item_metadata(available_categories_option->get_item_count() - 1, category);
			available_categories_option->get_popup()->set_item_icon_max_width(available_categories_option->get_item_count() - 1, 16);
		}
	}
	
	// Disable add button if no categories available to add
	if (add_button) {
		add_button->set_disabled(available_categories_option->get_item_count() <= 1);
	}
}

void CategoriesInItemEditor::_on_category_selected(int index) {
	// Enable/disable remove button based on selection
	if (remove_button) {
		remove_button->set_disabled(index < 0);
	}
}

void CategoriesInItemEditor::_on_add_button_pressed() {
	if (!available_categories_option || !item.is_valid()) return;
	
	int selected_index = available_categories_option->get_selected();
	if (selected_index <= 0) return; // First item is placeholder
	
	// Get the category from metadata
	Variant metadata = available_categories_option->get_item_metadata(selected_index);
	Ref<ItemCategory> category = metadata;
	
	if (category.is_valid()) {
		// Add the category to the item
		item_categories.append(category);
		item->set_categories(item_categories.duplicate());
		
		// Update both lists
		_update_item_categories_list();
		_update_available_categories_option();
		
		// Reset selection
		available_categories_option->select(0);
		
		// Emit changed signal
		emit_signal("changed");
	}
}

void CategoriesInItemEditor::_on_remove_button_pressed() {
	if (!categories_list || !item.is_valid()) return;
	
	PackedInt32Array selected_items = categories_list->get_selected_items();
	if (selected_items.size() == 0) return;
	
	int selected_index = selected_items[0];
	if (selected_index < 0 || selected_index >= item_categories.size()) return;
	
	// Remove the category from the item
	item_categories.remove_at(selected_index);
	item->set_categories(item_categories.duplicate());
	
	// Update both lists
	_update_item_categories_list();
	_update_available_categories_option();
	
	// Clear selection
	categories_list->deselect_all();
	if (remove_button) {
		remove_button->set_disabled(true);
	}
	
	// Emit changed signal
	emit_signal("changed");
}

void CategoriesInItemEditor::_apply_theme() {
	if (!add_button || !remove_button) {
		return;
	}

	// Set icons for add and remove buttons, similar to custom properties editor
	Ref<Texture2D> add_icon = get_theme_icon("Add", "EditorIcons");
	if (add_icon.is_valid()) {
		add_button->set_button_icon(add_icon);
	}

	Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
	if (remove_icon.is_valid()) {
		remove_button->set_button_icon(remove_icon);
	}
}

#endif // TOOLS_ENABLED