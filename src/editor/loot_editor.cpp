/**
 * loot_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "loot_editor.h"
#include "property_type_icons.h"
#include "../base/inventory_database.h"
#include "../base/loot.h"
#include "../base/loot_item.h"
#include "../base/item_definition.h"
#include "../base/item_stack.h"
#include "item_stack_selector.h"

#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/h_separator.hpp>

using namespace godot;

void LootEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_id_changed", "id"), &LootEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_name_text_changed", "text"), &LootEditor::_on_name_text_changed);
	ClassDB::bind_method(D_METHOD("_on_min_rolls_value_changed", "value"), &LootEditor::_on_min_rolls_value_changed);
	ClassDB::bind_method(D_METHOD("_on_max_rolls_value_changed", "value"), &LootEditor::_on_max_rolls_value_changed);
	ClassDB::bind_method(D_METHOD("_on_add_item_button_pressed"), &LootEditor::_on_add_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_item_button_pressed"), &LootEditor::_on_remove_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_items_list_item_selected", "index"), &LootEditor::_on_items_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_items_list_item_clicked", "index", "position", "mouse_button_index"), &LootEditor::_on_items_list_item_clicked);
	ClassDB::bind_method(D_METHOD("_on_context_menu_id_pressed", "id"), &LootEditor::_on_context_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_stack_changed", "item_stack"), &LootEditor::_on_item_stack_changed);
	ClassDB::bind_method(D_METHOD("_on_weight_value_changed", "value"), &LootEditor::_on_weight_value_changed);
	ClassDB::bind_method(D_METHOD("_on_min_amount_value_changed", "value"), &LootEditor::_on_min_amount_value_changed);
	ClassDB::bind_method(D_METHOD("_on_max_amount_value_changed", "value"), &LootEditor::_on_max_amount_value_changed);
	ClassDB::bind_method(D_METHOD("_on_add_property_range_button_pressed"), &LootEditor::_on_add_property_range_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_property_range_button_pressed"), &LootEditor::_on_remove_property_range_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_property_ranges_list_item_selected", "index"), &LootEditor::_on_property_ranges_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_property_name_text_changed", "text"), &LootEditor::_on_property_name_text_changed);
	ClassDB::bind_method(D_METHOD("_on_property_type_item_selected", "index"), &LootEditor::_on_property_type_item_selected);
	ClassDB::bind_method(D_METHOD("_on_min_value_value_changed", "value"), &LootEditor::_on_min_value_value_changed);
	ClassDB::bind_method(D_METHOD("_on_max_value_value_changed", "value"), &LootEditor::_on_max_value_value_changed);
	ClassDB::bind_method(D_METHOD("_on_bool_value_toggled", "pressed"), &LootEditor::_on_bool_value_toggled);

	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "loot", PROPERTY_HINT_RESOURCE_TYPE, "Loot")));
}

void LootEditor::_notification(int p_what) {
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

LootEditor::LootEditor() : BaseResourceEditor() {
	database = nullptr;
	editor_plugin = nullptr;
	item_stack_selector = nullptr;
	items_hsplit = nullptr;
	items_left_vbox = nullptr;
	items_right_vbox = nullptr;
	min_rolls_spinbox = nullptr;
	max_rolls_spinbox = nullptr;
	property_ranges_list = nullptr;
	property_range_details_vbox = nullptr;
	property_name_line_edit = nullptr;
	property_type_option = nullptr;
	min_value_spinbox = nullptr;
	max_value_spinbox = nullptr;
	bool_value_checkbox = nullptr;
}

LootEditor::~LootEditor() {
}

void LootEditor::_create_ui() {
	// Call base class to create ScrollContainer and main_vbox
	BaseResourceEditor::_create_ui();

	// Header section - using pattern from item_definition_editor
	header_hbox = memnew(HBoxContainer);
	main_vbox->add_child(header_hbox);

	// Left side: VBoxContainer for ID and Name
	VBoxContainer *left_vbox = memnew(VBoxContainer);
	header_hbox->add_child(left_vbox);
	left_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// ResourceIDEditor - ID comes first
	id_editor = memnew(ResourceIDEditor);
	left_vbox->add_child(id_editor);
	id_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_LOOT);
	id_editor->connect("changed", callable_mp(this, &LootEditor::_on_id_changed));

	// Name field - HBoxContainer following item_definition_editor pattern
	HBoxContainer *name_hbox = memnew(HBoxContainer);
	left_vbox->add_child(name_hbox);
	name_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label *name_label = memnew(Label);
	name_hbox->add_child(name_label);
	name_label->set_text("Name");
	name_label->set_custom_minimum_size(Vector2(160, 0));

	name_line_edit = memnew(LineEdit);
	name_hbox->add_child(name_line_edit);
	name_line_edit->set_custom_minimum_size(Vector2(0, 32));
	name_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	name_line_edit->set_placeholder("Enter loot name here");
	name_line_edit->connect("text_changed", callable_mp(this, &LootEditor::_on_name_text_changed));

	// Total weight field - HBoxContainer following item_definition_editor pattern
	HBoxContainer *weight_hbox = memnew(HBoxContainer);
	left_vbox->add_child(weight_hbox);
	weight_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label *weight_info_label = memnew(Label);
	weight_hbox->add_child(weight_info_label);
	weight_info_label->set_text("Total Weight");
	weight_info_label->set_custom_minimum_size(Vector2(160, 0));

	total_weight_label = memnew(Label);
	weight_hbox->add_child(total_weight_label);
	total_weight_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Min and Max rolls - combined in one horizontal container
	HBoxContainer *rolls_hbox = memnew(HBoxContainer);
	left_vbox->add_child(rolls_hbox);
	rolls_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label *min_rolls_label = memnew(Label);
	rolls_hbox->add_child(min_rolls_label);
	min_rolls_label->set_text("Min Rolls");
	min_rolls_label->set_custom_minimum_size(Vector2(160, 0));

	min_rolls_spinbox = memnew(SpinBox);
	rolls_hbox->add_child(min_rolls_spinbox);
	min_rolls_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	min_rolls_spinbox->set_min(0);
	min_rolls_spinbox->set_max(999);
	min_rolls_spinbox->set_step(1);
	min_rolls_spinbox->set_value(1);
	min_rolls_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_min_rolls_value_changed));

	Label *max_rolls_label = memnew(Label);
	rolls_hbox->add_child(max_rolls_label);
	max_rolls_label->set_text("Max Rolls");
	max_rolls_label->set_custom_minimum_size(Vector2(160, 0));

	max_rolls_spinbox = memnew(SpinBox);
	rolls_hbox->add_child(max_rolls_spinbox);
	max_rolls_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	max_rolls_spinbox->set_min(0);
	max_rolls_spinbox->set_max(999);
	max_rolls_spinbox->set_step(1);
	max_rolls_spinbox->set_value(1);
	max_rolls_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_max_rolls_value_changed));

	// Spacer Control matching item_definition_editor
	Control *spacer = memnew(Control);
	left_vbox->add_child(spacer);
	spacer->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Separator
	HSeparator *sep1 = memnew(HSeparator);
	main_vbox->add_child(sep1);

	// Items section with horizontal split container
	items_hsplit = memnew(HSplitContainer);
	main_vbox->add_child(items_hsplit);
	items_hsplit->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	items_hsplit->set_split_offset(-300); // Default split position

	// Left side: Items list
	items_left_vbox = memnew(VBoxContainer);
	items_hsplit->add_child(items_left_vbox);
	items_left_vbox->set_custom_minimum_size(Vector2(200, 0));

	// Items section header with title and buttons
	HBoxContainer *items_header = memnew(HBoxContainer);
	items_left_vbox->add_child(items_header);
	
	items_label = memnew(Label);
	items_header->add_child(items_label);
	items_label->set_text("Loot Items:");
	items_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	add_item_button = memnew(Button);
	items_header->add_child(add_item_button);
	add_item_button->set_tooltip_text("Add Item");
	add_item_button->set_custom_minimum_size(Vector2(32, 32));
	add_item_button->set_flat(true); // Make button flat
	add_item_button->connect("pressed", callable_mp(this, &LootEditor::_on_add_item_button_pressed));

	remove_item_button = memnew(Button);
	items_header->add_child(remove_item_button);
	remove_item_button->set_tooltip_text("Remove Item");
	remove_item_button->set_custom_minimum_size(Vector2(32, 32));
	remove_item_button->set_flat(true); // Make button flat
	remove_item_button->set_disabled(true);
	remove_item_button->connect("pressed", callable_mp(this, &LootEditor::_on_remove_item_button_pressed));

	// Items list - set to expand and fill available space
	items_list = memnew(ItemList);
	items_left_vbox->add_child(items_list);
	items_list->set_custom_minimum_size(Vector2(0, 150));
	items_list->set_v_size_flags(Control::SIZE_EXPAND_FILL); // Fill available vertical space
	items_list->set_allow_rmb_select(true);
	items_list->set_fixed_icon_size(Vector2i(16, 16)); // Match inventory_item_list icon size
	items_list->connect("item_selected", callable_mp(this, &LootEditor::_on_items_list_item_selected));
	items_list->connect("item_clicked", callable_mp(this, &LootEditor::_on_items_list_item_clicked));

	// Right side: Item details section
	items_right_vbox = memnew(VBoxContainer);
	items_hsplit->add_child(items_right_vbox);
	items_right_vbox->set_custom_minimum_size(Vector2(300, 0));

	item_details_vbox = memnew(VBoxContainer);
	items_right_vbox->add_child(item_details_vbox);
	item_details_vbox->set_v_size_flags(Control::SIZE_SHRINK_END); // Don't expand, stay compact

	item_details_label = memnew(Label);
	item_details_vbox->add_child(item_details_label);
	item_details_label->set_text("Item Details:");

	// Item selection using ItemStackSelector - no label needed
	item_stack_selector = memnew(ItemStackSelector);
	item_details_vbox->add_child(item_stack_selector);
	item_stack_selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	item_stack_selector->set_show_quantity(false); // Hide quantity for loot editor
	item_stack_selector->connect("changed", callable_mp(this, &LootEditor::_on_item_stack_changed));

	// Weight
	weight_hbox = memnew(HBoxContainer);
	item_details_vbox->add_child(weight_hbox);

	weight_label = memnew(Label);
	weight_hbox->add_child(weight_label);
	weight_label->set_text("Weight:");
	weight_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	weight_spinbox = memnew(SpinBox);
	weight_hbox->add_child(weight_spinbox);
	weight_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	weight_spinbox->set_min(0.0);
	weight_spinbox->set_max(999999.0);
	weight_spinbox->set_step(0.01);
	weight_spinbox->set_value(1.0);
	weight_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_weight_value_changed));

	// Min and Max amount - combined in one horizontal container
	HBoxContainer *amount_hbox = memnew(HBoxContainer);
	item_details_vbox->add_child(amount_hbox);

	min_amount_label = memnew(Label);
	amount_hbox->add_child(min_amount_label);
	min_amount_label->set_text("Min Amount:");
	min_amount_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	min_amount_spinbox = memnew(SpinBox);
	amount_hbox->add_child(min_amount_spinbox);
	min_amount_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	min_amount_spinbox->set_min(0);
	min_amount_spinbox->set_max(999999);
	min_amount_spinbox->set_step(1);
	min_amount_spinbox->set_value(1);
	min_amount_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_min_amount_value_changed));

	max_amount_label = memnew(Label);
	amount_hbox->add_child(max_amount_label);
	max_amount_label->set_text("Max Amount:");
	max_amount_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	max_amount_spinbox = memnew(SpinBox);
	amount_hbox->add_child(max_amount_spinbox);
	max_amount_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	max_amount_spinbox->set_min(0);
	max_amount_spinbox->set_max(999999);
	max_amount_spinbox->set_step(1);
	max_amount_spinbox->set_value(1);
	max_amount_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_max_amount_value_changed));

	// Separator for property ranges section
	HSeparator *prop_sep = memnew(HSeparator);
	item_details_vbox->add_child(prop_sep);

	// Property ranges section header
	HBoxContainer *property_ranges_header = memnew(HBoxContainer);
	item_details_vbox->add_child(property_ranges_header);
	
	property_ranges_label = memnew(Label);
	property_ranges_header->add_child(property_ranges_label);
	property_ranges_label->set_text("Property Ranges:");
	property_ranges_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	add_property_range_button = memnew(Button);
	property_ranges_header->add_child(add_property_range_button);
	add_property_range_button->set_tooltip_text("Add Property Range");
	add_property_range_button->set_custom_minimum_size(Vector2(32, 32));
	add_property_range_button->set_flat(true);
	add_property_range_button->connect("pressed", callable_mp(this, &LootEditor::_on_add_property_range_button_pressed));

	remove_property_range_button = memnew(Button);
	property_ranges_header->add_child(remove_property_range_button);
	remove_property_range_button->set_tooltip_text("Remove Property Range");
	remove_property_range_button->set_custom_minimum_size(Vector2(32, 32));
	remove_property_range_button->set_flat(true);
	remove_property_range_button->set_disabled(true);
	remove_property_range_button->connect("pressed", callable_mp(this, &LootEditor::_on_remove_property_range_button_pressed));

	// Property ranges list
	property_ranges_list = memnew(ItemList);
	item_details_vbox->add_child(property_ranges_list);
	property_ranges_list->set_custom_minimum_size(Vector2(0, 100));
	property_ranges_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	property_ranges_list->connect("item_selected", callable_mp(this, &LootEditor::_on_property_ranges_list_item_selected));

	// Property range details section
	property_range_details_vbox = memnew(VBoxContainer);
	item_details_vbox->add_child(property_range_details_vbox);
	property_range_details_vbox->set_visible(false);

	property_range_details_label = memnew(Label);
	property_range_details_vbox->add_child(property_range_details_label);
	property_range_details_label->set_text("Property Range Details:");

	// Property name field
	HBoxContainer *prop_name_hbox = memnew(HBoxContainer);
	property_range_details_vbox->add_child(prop_name_hbox);

	Label *prop_name_label = memnew(Label);
	prop_name_hbox->add_child(prop_name_label);
	prop_name_label->set_text("Property Name:");
	prop_name_label->set_custom_minimum_size(Vector2(160, 0));

	property_name_line_edit = memnew(LineEdit);
	prop_name_hbox->add_child(property_name_line_edit);
	property_name_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	property_name_line_edit->set_placeholder("Enter property name (e.g., durability)");
	property_name_line_edit->connect("text_changed", callable_mp(this, &LootEditor::_on_property_name_text_changed));

	// Property type field
	HBoxContainer *prop_type_hbox = memnew(HBoxContainer);
	property_range_details_vbox->add_child(prop_type_hbox);

	Label *prop_type_label = memnew(Label);
	prop_type_hbox->add_child(prop_type_label);
	prop_type_label->set_text("Value Type:");
	prop_type_label->set_custom_minimum_size(Vector2(160, 0));

	property_type_option = memnew(OptionButton);
	prop_type_hbox->add_child(property_type_option);
	property_type_option->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	property_type_option->connect("item_selected", callable_mp(this, &LootEditor::_on_property_type_item_selected));

	// Range values (for int/float)
	range_values_hbox = memnew(HBoxContainer);
	property_range_details_vbox->add_child(range_values_hbox);

	min_value_label = memnew(Label);
	range_values_hbox->add_child(min_value_label);
	min_value_label->set_text("Min Value:");
	min_value_label->set_custom_minimum_size(Vector2(160, 0));

	min_value_spinbox = memnew(SpinBox);
	range_values_hbox->add_child(min_value_spinbox);
	min_value_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	min_value_spinbox->set_min(-999999);
	min_value_spinbox->set_max(999999);
	min_value_spinbox->set_step(1);
	min_value_spinbox->set_value(0);
	min_value_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_min_value_value_changed));

	max_value_label = memnew(Label);
	range_values_hbox->add_child(max_value_label);
	max_value_label->set_text("Max Value:");
	max_value_label->set_custom_minimum_size(Vector2(160, 0));

	max_value_spinbox = memnew(SpinBox);
	range_values_hbox->add_child(max_value_spinbox);
	max_value_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	max_value_spinbox->set_min(-999999);
	max_value_spinbox->set_max(999999);
	max_value_spinbox->set_step(1);
	max_value_spinbox->set_value(100);
	max_value_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_max_value_value_changed));

	// Boolean value (for bool type)
	HBoxContainer *bool_value_hbox = memnew(HBoxContainer);
	property_range_details_vbox->add_child(bool_value_hbox);

	bool_value_label = memnew(Label);
	bool_value_hbox->add_child(bool_value_label);
	bool_value_label->set_text("Boolean Value:");
	bool_value_label->set_custom_minimum_size(Vector2(160, 0));

	bool_value_checkbox = memnew(CheckBox);
	bool_value_hbox->add_child(bool_value_checkbox);
	bool_value_checkbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	bool_value_checkbox->connect("toggled", callable_mp(this, &LootEditor::_on_bool_value_toggled));

	// Initially hide bool controls
	bool_value_hbox->set_visible(false);

	// Context menu
	context_menu = memnew(PopupMenu);
	add_child(context_menu);
	context_menu->connect("id_pressed", callable_mp(this, &LootEditor::_on_context_menu_id_pressed));

	// Disable item details by default
	item_details_vbox->set_visible(false);
}

void LootEditor::_apply_theme() {
	if (!editor_plugin) {
		return;
	}
	
	// Set icons for add and remove buttons using Godot editor icons
	if (add_item_button) {
		Ref<Texture2D> add_icon = get_theme_icon("Add", "EditorIcons");
		if (add_icon.is_valid()) {
			add_item_button->set_button_icon(add_icon);
			add_item_button->set_text(""); // Remove text when icon is available
		}
	}
	
	if (remove_item_button) {
		Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
		if (remove_icon.is_valid()) {
			remove_item_button->set_button_icon(remove_icon);
			remove_item_button->set_text(""); // Remove text when icon is available
		}
	}
	
	// Set icons for property range add and remove buttons
	if (add_property_range_button) {
		Ref<Texture2D> add_icon = get_theme_icon("Add", "EditorIcons");
		if (add_icon.is_valid()) {
			add_property_range_button->set_button_icon(add_icon);
			add_property_range_button->set_text(""); // Remove text when icon is available
		}
	}
	
	if (remove_property_range_button) {
		Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
		if (remove_icon.is_valid()) {
			remove_property_range_button->set_button_icon(remove_icon);
			remove_property_range_button->set_text(""); // Remove text when icon is available
		}
	}
	
	// Setup property type options with icons matching custom_properties_editor
	if (property_type_option) {
		property_type_option->clear();
		property_type_option->add_icon_item(get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(Variant::INT), "EditorIcons"), "Integer Range", 0);
		property_type_option->add_icon_item(get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(Variant::FLOAT), "EditorIcons"), "Float Range", 1);
		property_type_option->add_icon_item(get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(Variant::BOOL), "EditorIcons"), "Boolean Value", 2);
	}
	
	// Setup context menu with proper icons
	if (context_menu) {
		context_menu->clear();
		
		// Add Remove option with editor icon
		Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
		context_menu->add_icon_item(remove_icon, "Remove", BaseInventoryEditor::ITEM_REMOVE);
		
		// Add Duplicate option with editor icon
		Ref<Texture2D> duplicate_icon = get_theme_icon("Duplicate", "EditorIcons");
		context_menu->add_icon_item(duplicate_icon, "Duplicate", BaseInventoryEditor::ITEM_DUPLICATE);
	}
}

void LootEditor::load_loot(const Ref<Loot> &p_loot, InventoryDatabase *p_database) {
	current_loot = p_loot;
	database = p_database;
	_update_ui();
}

void LootEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
	_apply_theme();
}

void LootEditor::clear() {
	current_loot = Ref<Loot>();
	database = nullptr;
	_update_ui();
}

void LootEditor::_update_ui() {
	if (current_loot.is_null()) {
		if (id_editor) {
			id_editor->setup(database, "");
		}
		name_line_edit->set_text("");
		if (min_rolls_spinbox) {
			min_rolls_spinbox->set_value(1);
		}
		if (max_rolls_spinbox) {
			max_rolls_spinbox->set_value(1);
		}
		items_list->clear();
		item_details_vbox->set_visible(false);
		_update_total_weight();
		return;
	}

	if (id_editor) {
		id_editor->setup(database, current_loot->get_id());
	}
	name_line_edit->set_text(current_loot->get_name());
	if (min_rolls_spinbox) {
		min_rolls_spinbox->set_value(current_loot->get_min_rolls());
	}
	if (max_rolls_spinbox) {
		max_rolls_spinbox->set_value(current_loot->get_max_rolls());
	}
	_update_items_list();
	_update_total_weight();
}

void LootEditor::_update_items_list() {
	if (!items_list || current_loot.is_null()) {
		return;
	}

	items_list->clear();
	
	TypedArray<LootItem> items = current_loot->get_items();
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			String item_id = item->get_item_id();
			String display_text = "No Item";
			Ref<Texture2D> item_icon;
			
			// Get item definition to show name and icon
			if (!item_id.is_empty() && database) {
				Ref<ItemDefinition> item_def = database->get_item(item_id);
				if (item_def.is_valid()) {
					display_text = item_def->get_name();
					item_icon = item_def->get_icon();
				} else {
					display_text = item_id; // Fallback to ID if definition not found
				}
			}
			
			// Show weight and min-max amount range
			display_text += " (weight: " + String::num_real(item->get_weight()) + ") ";
			if (item->get_min_amount() == item->get_max_amount()) {
				display_text += "(" + String::num_int64(item->get_min_amount()) + ")";
			} else {
				display_text += "(" + String::num_int64(item->get_min_amount()) + "-" + String::num_int64(item->get_max_amount()) + ")";
			}
			
			items_list->add_item(display_text);
			if (item_icon.is_valid()) {
				items_list->set_item_icon(i, item_icon);
			}
			items_list->set_item_metadata(i, item);
		}
	}
	if (remove_item_button) {
		remove_item_button->set_disabled(items.size() == 0);
	}
}

void LootEditor::_update_item_details() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	
	if (selected_item.is_null()) {
		item_details_vbox->set_visible(false);
		return;
	}

	item_details_vbox->set_visible(true);
	
	// Update item stack selector 
	if (item_stack_selector && database) {
		Ref<ItemStack> item_stack = memnew(ItemStack);
		item_stack->set_item_id(selected_item->get_item_id());
		item_stack->set_amount(selected_item->get_min_amount()); // Use min_amount as default
		item_stack_selector->setup(item_stack, database);
	}
	
	weight_spinbox->set_value(selected_item->get_weight());
	min_amount_spinbox->set_value(selected_item->get_min_amount());
	max_amount_spinbox->set_value(selected_item->get_max_amount());
	
	// Update property ranges
	_update_property_ranges_list();
	property_range_details_vbox->set_visible(false); // Hide details until a property is selected
}

void LootEditor::_update_total_weight() {
	float total = 0.0;
	if (current_loot.is_valid()) {
		total = current_loot->get_total_weight();
	}
	total_weight_label->set_text(String::num_real(total));
}

Ref<LootItem> LootEditor::_get_selected_loot_item() {
	if (!items_list || items_list->get_selected_items().size() == 0) {
		return Ref<LootItem>();
	}

	int selected = items_list->get_selected_items()[0];
	return items_list->get_item_metadata(selected);
}

void LootEditor::_on_id_changed(const String &p_id) {
	if (current_loot.is_valid() && id_editor) {
		current_loot->set_id(id_editor->get_id());
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_name_text_changed(const String &p_text) {
	if (current_loot.is_valid()) {
		current_loot->set_name(p_text);
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_min_rolls_value_changed(double p_value) {
	if (current_loot.is_valid()) {
		current_loot->set_min_rolls((int)p_value);
		
		// Ensure max rolls is not less than min rolls
		if (current_loot->get_max_rolls() < (int)p_value) {
			current_loot->set_max_rolls((int)p_value);
			if (max_rolls_spinbox) {
				max_rolls_spinbox->set_value((int)p_value);
			}
		}
		
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_max_rolls_value_changed(double p_value) {
	if (current_loot.is_valid()) {
		current_loot->set_max_rolls((int)p_value);
		
		// Ensure min rolls is not greater than max rolls
		if (current_loot->get_min_rolls() > (int)p_value) {
			current_loot->set_min_rolls((int)p_value);
			if (min_rolls_spinbox) {
				min_rolls_spinbox->set_value((int)p_value);
			}
		}
		
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_add_item_button_pressed() {
	if (current_loot.is_null()) {
		return;
	}

	Ref<LootItem> new_item = memnew(LootItem);
	new_item->set_item_id("");
	new_item->set_weight(1.0);
	new_item->set_min_amount(1);
	new_item->set_max_amount(1);

	TypedArray<LootItem> items = current_loot->get_items();
	items.append(new_item);
	current_loot->set_items(items);

	_update_items_list();
	_update_total_weight();
	
	// Select the new item
	if (items_list->get_item_count() > 0) {
		items_list->select(items_list->get_item_count() - 1);
		_on_items_list_item_selected(items_list->get_item_count() - 1);
	}
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_remove_item_button_pressed() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || current_loot.is_null()) {
		return;
	}

	TypedArray<LootItem> items = current_loot->get_items();
	int index = items.find(selected_item);
	if (index >= 0) {
		items.remove_at(index);
		current_loot->set_items(items);
		_update_items_list();
		_update_total_weight();
		item_details_vbox->set_visible(false);
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_items_list_item_selected(int p_index) {
	_update_item_details();
}

void LootEditor::_on_items_list_item_clicked(int p_index, const Vector2 &p_position, int p_mouse_button_index) {
	if (p_mouse_button_index == MOUSE_BUTTON_LEFT) {
		// Left click - select the item and update details
		items_list->select(p_index);
		_update_item_details();
	} else if (p_mouse_button_index == MOUSE_BUTTON_RIGHT) {
		// Right click - show context menu with proper positioning
		if (!context_menu) {
			return;
		}

		items_list->select(p_index);
		_update_item_details();
		
		// Position popup at mouse position using the same pattern as other editors
		Vector2 global_pos = get_global_mouse_position();
		Variant v = get_viewport()->get("position");
		context_menu->set_position(((Vector2)v) + global_pos);
		context_menu->popup();
	}
}


void LootEditor::_on_context_menu_id_pressed(int p_id) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || current_loot.is_null()) {
		return;
	}

	switch (p_id) {
		case BaseInventoryEditor::ITEM_REMOVE:
			_on_remove_item_button_pressed();
			break;
		case BaseInventoryEditor::ITEM_DUPLICATE: {
			Ref<LootItem> new_item = memnew(LootItem);
			new_item->set_item_id(selected_item->get_item_id());
			new_item->set_weight(selected_item->get_weight());
			new_item->set_min_amount(selected_item->get_min_amount());
			new_item->set_max_amount(selected_item->get_max_amount());

			TypedArray<LootItem> items = current_loot->get_items();
			items.append(new_item);
			current_loot->set_items(items);

			_update_items_list();
			_update_total_weight();
			emit_signal("changed", current_loot);
		} break;
	}
}

void LootEditor::_on_item_stack_changed(const Ref<ItemStack> &p_item_stack) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || p_item_stack.is_null()) {
		return;
	}

	selected_item->set_item_id(p_item_stack->get_item_id());
	_update_items_list();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_weight_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_weight(p_value);
	_update_items_list();
	_update_total_weight();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_min_amount_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_min_amount((int)p_value);
	
	// Ensure max amount is not less than min amount
	if (selected_item->get_max_amount() < (int)p_value) {
		selected_item->set_max_amount((int)p_value);
		max_amount_spinbox->set_value((int)p_value);
	}
	
	_update_items_list();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_max_amount_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_max_amount((int)p_value);
	
	// Ensure min amount is not greater than max amount
	if (selected_item->get_min_amount() > (int)p_value) {
		selected_item->set_min_amount((int)p_value);
		min_amount_spinbox->set_value((int)p_value);
	}
	
	_update_items_list();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_add_property_range_button_pressed() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Dictionary property_ranges = selected_item->get_property_ranges();
	String new_prop_name = "new_property_" + String::num_int64(property_ranges.size());
	
	Dictionary new_range;
	new_range["min"] = 0;
	new_range["max"] = 100;
	
	property_ranges[new_prop_name] = new_range;
	selected_item->set_property_ranges(property_ranges);
	
	_update_property_ranges_list();
	
	// Select the newly added property
	int new_index = property_ranges_list->get_item_count() - 1;
	if (new_index >= 0) {
		property_ranges_list->select(new_index);
		_update_property_range_details();
	}
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_remove_property_range_button_pressed() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0]; // Get property name before the colon

	Dictionary property_ranges = selected_item->get_property_ranges();
	property_ranges.erase(property_name);
	selected_item->set_property_ranges(property_ranges);
	
	_update_property_ranges_list();
	property_range_details_vbox->set_visible(false);
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_property_ranges_list_item_selected(int p_index) {
	_update_property_range_details();
}

void LootEditor::_on_property_name_text_changed(const String &p_text) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String old_item_text = property_ranges_list->get_item_text(selected_index);
	String old_property_name = old_item_text.split(":")[0];

	if (p_text == old_property_name) {
		return; // No change
	}

	Dictionary property_ranges = selected_item->get_property_ranges();
	
	// Check if new name already exists
	if (property_ranges.has(p_text) && p_text != old_property_name) {
		// Reset to old name if new name conflicts
		property_name_line_edit->set_text(old_property_name);
		return;
	}

	// Move the property to the new name
	Dictionary range_config = property_ranges[old_property_name];
	property_ranges.erase(old_property_name);
	property_ranges[p_text] = range_config;
	
	selected_item->set_property_ranges(property_ranges);
	_update_property_ranges_list();
	
	// Reselect the renamed property
	for (int i = 0; i < property_ranges_list->get_item_count(); i++) {
		String item_text = property_ranges_list->get_item_text(i);
		if (item_text.begins_with(p_text + String(":"))) {
			property_ranges_list->select(i);
			break;
		}
	}
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_property_type_item_selected(int p_index) {
	// Update the visibility of different value input controls
	if (p_index == 2) { // Boolean type
		range_values_hbox->set_visible(false);
		Object::cast_to<Control>(bool_value_checkbox->get_parent())->set_visible(true);
	} else { // Integer or Float type
		range_values_hbox->set_visible(true);
		Object::cast_to<Control>(bool_value_checkbox->get_parent())->set_visible(false);
		
		// Update step for float vs int
		if (p_index == 1) { // Float type
			min_value_spinbox->set_step(0.1);
			max_value_spinbox->set_step(0.1);
		} else { // Integer type
			min_value_spinbox->set_step(1);
			max_value_spinbox->set_step(1);
		}
	}
	
	// Update the selected property range configuration based on new type
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0];

	Dictionary property_ranges = selected_item->get_property_ranges();
	Dictionary old_range_config = property_ranges[property_name];
	Dictionary range_config;
	
	if (p_index == 2) { // Boolean type
		// Convert to boolean type
		bool default_value = false;
		if (old_range_config.has("value")) {
			default_value = old_range_config["value"];
		} else if (old_range_config.has("min")) {
			// Convert numeric min value to boolean
			default_value = (double)old_range_config["min"] > 0;
		}
		range_config["value"] = default_value;
		bool_value_checkbox->set_pressed(default_value);
	} else { // Integer or Float type
		// Convert to range type
		double min_value = 0.0;
		double max_value = 1.0;
		
		if (old_range_config.has("min") && old_range_config.has("max")) {
			min_value = old_range_config["min"];
			max_value = old_range_config["max"];
		} else if (old_range_config.has("value")) {
			// Convert boolean value to numeric range
			bool bool_val = old_range_config["value"];
			min_value = bool_val ? 1.0 : 0.0;
			max_value = bool_val ? 1.0 : 0.0;
		}
		
		// Convert to proper type (int or float)
		if (p_index == 0) { // Integer type
			min_value = Math::round(min_value);
			max_value = Math::round(max_value);
			range_config["min"] = (int)min_value;
			range_config["max"] = (int)max_value;
		} else { // Float type
			range_config["min"] = min_value;
			range_config["max"] = max_value;
		}
		
		min_value_spinbox->set_value(min_value);
		max_value_spinbox->set_value(max_value);
	}
	
	property_ranges[property_name] = range_config;
	selected_item->set_property_ranges(property_ranges);
	_update_property_ranges_list();
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_min_value_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0];

	Dictionary property_ranges = selected_item->get_property_ranges();
	Dictionary range_config = property_ranges[property_name];
	
	// Store value with correct type based on current property type selection
	if (property_type_option->get_selected() == 0) { // Integer type
		range_config["min"] = (int)p_value;
	} else { // Float type
		range_config["min"] = p_value;
	}
	
	// Ensure max is not less than min
	if (range_config.has("max")) {
		double max_value = range_config["max"];
		if (max_value < p_value) {
			if (property_type_option->get_selected() == 0) { // Integer type
				range_config["max"] = (int)p_value;
			} else { // Float type
				range_config["max"] = p_value;
			}
			max_value_spinbox->set_value(p_value);
		}
	}
	
	property_ranges[property_name] = range_config;
	selected_item->set_property_ranges(property_ranges);
	_update_property_ranges_list();
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_max_value_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0];

	Dictionary property_ranges = selected_item->get_property_ranges();
	Dictionary range_config = property_ranges[property_name];
	
	// Store value with correct type based on current property type selection
	if (property_type_option->get_selected() == 0) { // Integer type
		range_config["max"] = (int)p_value;
	} else { // Float type
		range_config["max"] = p_value;
	}
	
	// Ensure min is not greater than max
	if (range_config.has("min")) {
		double min_value = range_config["min"];
		if (min_value > p_value) {
			if (property_type_option->get_selected() == 0) { // Integer type
				range_config["min"] = (int)p_value;
			} else { // Float type
				range_config["min"] = p_value;
			}
			min_value_spinbox->set_value(p_value);
		}
	}
	
	property_ranges[property_name] = range_config;
	selected_item->set_property_ranges(property_ranges);
	_update_property_ranges_list();
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_bool_value_toggled(bool p_pressed) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0];

	Dictionary property_ranges = selected_item->get_property_ranges();
	Dictionary range_config;
	range_config["value"] = p_pressed;
	
	property_ranges[property_name] = range_config;
	selected_item->set_property_ranges(property_ranges);
	_update_property_ranges_list();
	
	emit_signal("changed", current_loot);
}

void LootEditor::_update_property_ranges_list() {
	if (!property_ranges_list) {
		return;
	}

	// Store currently selected property name to restore selection after update
	String selected_property_name;
	Array selected_items = property_ranges_list->get_selected_items();
	if (!selected_items.is_empty()) {
		int selected_index = selected_items[0];
		String item_text = property_ranges_list->get_item_text(selected_index);
		selected_property_name = item_text.split(":")[0];
	}

	property_ranges_list->clear();
	
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	Dictionary property_ranges = selected_item->get_property_ranges();
	Array property_keys = property_ranges.keys();
	
	for (int i = 0; i < property_keys.size(); i++) {
		String property_name = property_keys[i];
		Dictionary range_config = property_ranges[property_name];
		
		String display_text = property_name + ": ";
		Variant::Type property_type = Variant::NIL;
		
		if (range_config.has("min") && range_config.has("max")) {
			display_text += String::num_real(range_config["min"]) + " - " + String::num_real(range_config["max"]);
			// Determine if it's int or float based on the values
			Variant min_val = range_config["min"];
			Variant max_val = range_config["max"];
			if (min_val.get_type() == Variant::INT && max_val.get_type() == Variant::INT) {
				property_type = Variant::INT;
			} else {
				property_type = Variant::FLOAT;
			}
		} else if (range_config.has("value")) {
			display_text += (range_config["value"] ? "true" : "false");
			property_type = Variant::BOOL;
		}
		
		Ref<Texture2D> icon = get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(property_type), "EditorIcons");
		property_ranges_list->add_item(display_text);
		property_ranges_list->set_item_icon(property_ranges_list->get_item_count() - 1, icon);
		
		// Restore selection if this was the previously selected property
		if (property_name == selected_property_name) {
			property_ranges_list->select(i);
		}
	}
	
	// Update button states
	remove_property_range_button->set_disabled(property_ranges_list->get_item_count() == 0);
}

void LootEditor::_update_property_range_details() {
	if (!property_range_details_vbox) {
		return;
	}

	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		property_range_details_vbox->set_visible(false);
		return;
	}

	Array selected_items = property_ranges_list->get_selected_items();
	if (selected_items.is_empty()) {
		property_range_details_vbox->set_visible(false);
		return;
	}

	int selected_index = selected_items[0];
	String item_text = property_ranges_list->get_item_text(selected_index);
	String property_name = item_text.split(":")[0];

	Dictionary property_ranges = selected_item->get_property_ranges();
	if (!property_ranges.has(property_name)) {
		property_range_details_vbox->set_visible(false);
		return;
	}

	Dictionary range_config = property_ranges[property_name];
	
	// Update property name
	property_name_line_edit->set_text(property_name);
	
	// Update property type and values
	if (range_config.has("value")) {
		// Boolean type
		property_type_option->select(2);
		bool_value_checkbox->set_pressed(range_config["value"]);
		range_values_hbox->set_visible(false);
		Object::cast_to<Control>(bool_value_checkbox->get_parent())->set_visible(true);
	} else if (range_config.has("min") && range_config.has("max")) {
		// Check if values are integers or floats
		Variant min_val = range_config["min"];
		Variant max_val = range_config["max"];
		
		if (min_val.get_type() == Variant::INT && max_val.get_type() == Variant::INT) {
			property_type_option->select(0); // Integer
			min_value_spinbox->set_step(1);
			max_value_spinbox->set_step(1);
		} else {
			property_type_option->select(1); // Float
			min_value_spinbox->set_step(0.1);
			max_value_spinbox->set_step(0.1);
		}
		
		min_value_spinbox->set_value(min_val);
		max_value_spinbox->set_value(max_val);
		range_values_hbox->set_visible(true);
		Object::cast_to<Control>(bool_value_checkbox->get_parent())->set_visible(false);
	}
	
	property_range_details_vbox->set_visible(true);
}

#endif // TOOLS_ENABLED