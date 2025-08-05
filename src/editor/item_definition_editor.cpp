/**
 * item_definition_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "item_definition_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_definition.h"
#include "resource_id_editor.h"
#include "icon_selector.h"
#include "item_definition_property_editor.h"
#include "categories_in_item_editor.h"

#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ItemDefinitionEditor::_bind_methods() {
	// Bind signal handlers - matching addon method names exactly
	ClassDB::bind_method(D_METHOD("_on_max_stack_spin_box_value_changed", "value"), &ItemDefinitionEditor::_on_max_stack_spin_box_value_changed);
	ClassDB::bind_method(D_METHOD("_on_text_edit_text_changed", "new_text"), &ItemDefinitionEditor::_on_text_edit_text_changed);
	ClassDB::bind_method(D_METHOD("_on_icon_changed", "icon"), &ItemDefinitionEditor::_on_icon_changed);
	ClassDB::bind_method(D_METHOD("_on_resource_id_editor_changed", "id"), &ItemDefinitionEditor::_on_resource_id_editor_changed);
	ClassDB::bind_method(D_METHOD("_on_weight_spin_box_value_changed", "value"), &ItemDefinitionEditor::_on_weight_spin_box_value_changed);
	ClassDB::bind_method(D_METHOD("_can_stack_check_box_toggled", "value"), &ItemDefinitionEditor::_can_stack_check_box_toggled);
	ClassDB::bind_method(D_METHOD("_on_size_x_spin_box_value_changed", "value"), &ItemDefinitionEditor::_on_size_x_spin_box_value_changed);
	ClassDB::bind_method(D_METHOD("_on_size_y_spin_box_value_changed", "value"), &ItemDefinitionEditor::_on_size_y_spin_box_value_changed);
	ClassDB::bind_method(D_METHOD("_on_item_description_text_edit_text_changed"), &ItemDefinitionEditor::_on_item_description_text_edit_text_changed);
	ClassDB::bind_method(D_METHOD("_on_categories_in_item_changed"), &ItemDefinitionEditor::_on_categories_in_item_changed);

	// Public methods
	ClassDB::bind_method(D_METHOD("set_editor_plugin", "editor_plugin"), &ItemDefinitionEditor::set_editor_plugin);
	ClassDB::bind_method(D_METHOD("load_item", "item", "database"), &ItemDefinitionEditor::load_item);
	ClassDB::bind_method(D_METHOD("reload_item"), &ItemDefinitionEditor::reload_item);

	// Signal matching addon
	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "item_definition", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")));
}

void ItemDefinitionEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

ItemDefinitionEditor::ItemDefinitionEditor() {
	database = nullptr;
	editor_plugin = nullptr;
	
	// Initialize UI pointers
	scroll_container = nullptr;
	margin_container = nullptr;
	main_vbox = nullptr;
	resource_id_editor = nullptr;
	item_name_text_edit = nullptr;
	item_max_stack_spin_box = nullptr;
	icon_selector = nullptr;
	weight_spin_box = nullptr;
	can_stack_check_box = nullptr;
	max_stack_container = nullptr;
	size_x_spin_box = nullptr;
	size_y_spin_box = nullptr;
	item_description_text_edit = nullptr;
	custom_properties = nullptr;
	categories_in_item = nullptr;

	set_custom_minimum_size(Vector2(256, 0));
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
}

ItemDefinitionEditor::~ItemDefinitionEditor() {
}

void ItemDefinitionEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	editor_plugin = p_editor_plugin;
}

void ItemDefinitionEditor::_create_ui() {
	// Match .tscn structure exactly: ScrollContainer -> MarginContainer -> VBoxContainer
	scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	scroll_container->set_visible(false);

	margin_container = memnew(MarginContainer);
	scroll_container->add_child(margin_container);
	margin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	margin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	// Match addon margin constants - ItemDefinitionEditor uses 8,8,0,8 
	margin_container->add_theme_constant_override("margin_left", 8);
	margin_container->add_theme_constant_override("margin_top", 8);
	margin_container->add_theme_constant_override("margin_right", 0);
	margin_container->add_theme_constant_override("margin_bottom", 8);

	main_vbox = memnew(VBoxContainer);
	margin_container->add_child(main_vbox);

	// Create top section: HBoxContainer2 with side-by-side layout
	HBoxContainer* top_hbox = memnew(HBoxContainer);
	main_vbox->add_child(top_hbox);

	// Left side: VBoxContainer for ID and Name
	VBoxContainer* left_vbox = memnew(VBoxContainer);
	top_hbox->add_child(left_vbox);
	left_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// ResourceIDEditor - matches addon structure
	resource_id_editor = memnew(ResourceIDEditor);
	left_vbox->add_child(resource_id_editor);
	resource_id_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	resource_id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_ITEM_DEFINITION);

	// Name field - HBoxContainer matching addon
	HBoxContainer* name_hbox = memnew(HBoxContainer);
	left_vbox->add_child(name_hbox);
	name_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label* name_label = memnew(Label);
	name_hbox->add_child(name_label);
	name_label->set_text("Name");
	name_label->set_custom_minimum_size(Vector2(160, 0));

	item_name_text_edit = memnew(LineEdit);
	name_hbox->add_child(item_name_text_edit);
	item_name_text_edit->set_custom_minimum_size(Vector2(0, 32));
	item_name_text_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	item_name_text_edit->set_placeholder("Place Item Name Here");

	// Spacer Control matching addon
	Control* spacer = memnew(Control);
	left_vbox->add_child(spacer);
	spacer->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// VSeparator between left and right sides
	VSeparator* v_sep = memnew(VSeparator);
	top_hbox->add_child(v_sep);

	// Right side: Icon selector
	icon_selector = memnew(IconSelector);
	top_hbox->add_child(icon_selector);
	icon_selector->set_custom_minimum_size(Vector2(288, 31));

	// Description section
	HBoxContainer* desc_hbox = memnew(HBoxContainer);
	main_vbox->add_child(desc_hbox);

	Label* desc_label = memnew(Label);
	desc_hbox->add_child(desc_label);
	desc_label->set_text("Description");
	desc_label->set_custom_minimum_size(Vector2(160, 0));

	item_description_text_edit = memnew(TextEdit);
	desc_hbox->add_child(item_description_text_edit);
	item_description_text_edit->set_custom_minimum_size(Vector2(0, 64));
	item_description_text_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	item_description_text_edit->set_placeholder("Enter item description here");

	// Can Stack and Max Stack section
	HBoxContainer* can_stack_and_max_stack_hbox = memnew(HBoxContainer);
	main_vbox->add_child(can_stack_and_max_stack_hbox);
	can_stack_and_max_stack_hbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Can Stack section
	HBoxContainer* can_stack_hbox = memnew(HBoxContainer);
	can_stack_and_max_stack_hbox->add_child(can_stack_hbox);
	can_stack_hbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	Label* can_stack_label = memnew(Label);
	can_stack_hbox->add_child(can_stack_label);
	can_stack_label->set_text("Can Stack");
	can_stack_label->set_custom_minimum_size(Vector2(160, 0));

	can_stack_check_box = memnew(CheckBox);
	can_stack_hbox->add_child(can_stack_check_box);

	// VSeparator between can_stack and max_stack
	VSeparator* can_stack_max_stack_sep = memnew(VSeparator);
	can_stack_and_max_stack_hbox->add_child(can_stack_max_stack_sep);

	// Max Stack section
	max_stack_container = memnew(HBoxContainer);
	can_stack_and_max_stack_hbox->add_child(max_stack_container);
	max_stack_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	Label* max_stack_label = memnew(Label);
	max_stack_container->add_child(max_stack_label);
	max_stack_label->set_text("Max Stack");
	max_stack_label->set_custom_minimum_size(Vector2(160, 0));

	item_max_stack_spin_box = memnew(SpinBox);
	max_stack_container->add_child(item_max_stack_spin_box);
	item_max_stack_spin_box->set_custom_minimum_size(Vector2(0, 32));
	item_max_stack_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	item_max_stack_spin_box->set_allow_greater(true);

	// Weight and Size section
	HBoxContainer* weight_size_hbox = memnew(HBoxContainer);
	main_vbox->add_child(weight_size_hbox);

	// Weight subsection
	HBoxContainer* weight_hbox = memnew(HBoxContainer);
	weight_size_hbox->add_child(weight_hbox);
	weight_hbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	Label* weight_label = memnew(Label);
	weight_hbox->add_child(weight_label);
	weight_label->set_text("Weight");
	weight_label->set_custom_minimum_size(Vector2(160, 0));

	weight_spin_box = memnew(SpinBox);
	weight_hbox->add_child(weight_spin_box);
	weight_spin_box->set_custom_minimum_size(Vector2(0, 32));
	weight_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	weight_spin_box->set_max(100000.0);
	weight_spin_box->set_step(0.001);
	weight_spin_box->set_value(0.1);
	weight_spin_box->set_allow_greater(true);
	weight_spin_box->set_allow_lesser(true);

	// VSeparator between weight and size
	VSeparator* weight_size_sep = memnew(VSeparator);
	weight_size_hbox->add_child(weight_size_sep);

	// Size subsection
	HBoxContainer* size_hbox = memnew(HBoxContainer);
	weight_size_hbox->add_child(size_hbox);
	size_hbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	Label* grid_size_label = memnew(Label);
	size_hbox->add_child(grid_size_label);
	grid_size_label->set_text("Grid Size");
	grid_size_label->set_custom_minimum_size(Vector2(160, 0));

	Label* x_label = memnew(Label);
	size_hbox->add_child(x_label);
	x_label->set_text("X");
	x_label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_RIGHT);
	x_label->set_custom_minimum_size(Vector2(32, 0));

	size_x_spin_box = memnew(SpinBox);
	size_hbox->add_child(size_x_spin_box);
	size_x_spin_box->set_custom_minimum_size(Vector2(64, 32));
	size_x_spin_box->set_min(1.0);
	size_x_spin_box->set_max(100000.0);
	size_x_spin_box->set_value(1.0);
	size_x_spin_box->set_allow_greater(true);
	size_x_spin_box->set_update_on_text_changed(true);

	Label* y_label = memnew(Label);
	size_hbox->add_child(y_label);
	y_label->set_text("Y");
	y_label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_RIGHT);
	y_label->set_custom_minimum_size(Vector2(32, 0));

	size_y_spin_box = memnew(SpinBox);
	size_hbox->add_child(size_y_spin_box);
	size_y_spin_box->set_custom_minimum_size(Vector2(64, 32));
	size_y_spin_box->set_min(1.0);
	size_y_spin_box->set_max(100000.0);
	size_y_spin_box->set_value(1.0);
	size_y_spin_box->set_allow_greater(true);

	// Add separator before categories
	HSeparator* categories_separator = memnew(HSeparator);
	main_vbox->add_child(categories_separator);
	
	// Categories section
	categories_in_item = memnew(CategoriesInItemEditor);
	main_vbox->add_child(categories_in_item);

	// Add separator before custom properties
	HSeparator* separator = memnew(HSeparator);
	main_vbox->add_child(separator);
	
	// Custom Properties section
	custom_properties = memnew(CustomPropertiesEditor);
	custom_properties->set_resource_type(CustomPropertiesEditor::RESOURCE_TYPE_ITEM_DEFINITION);
	custom_properties->set_title("Custom Properties");
	main_vbox->add_child(custom_properties);

	_connect_signals();
}

void ItemDefinitionEditor::_connect_signals() {
	if (can_stack_check_box) {
		can_stack_check_box->connect("toggled", callable_mp(this, &ItemDefinitionEditor::_can_stack_check_box_toggled));
	}
	if (icon_selector) {
		icon_selector->connect("icon_changed", callable_mp(this, &ItemDefinitionEditor::_on_icon_changed));
	}
	if (item_description_text_edit) {
		item_description_text_edit->connect("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_item_description_text_edit_text_changed));
	}
	if (item_name_text_edit) {
		item_name_text_edit->connect("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_text_edit_text_changed));
	}
	if (item_max_stack_spin_box) {
		item_max_stack_spin_box->connect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_max_stack_spin_box_value_changed));
	}
	if (weight_spin_box) {
		weight_spin_box->connect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_weight_spin_box_value_changed));
	}
	if (resource_id_editor) {
		resource_id_editor->connect("changed", callable_mp(this, &ItemDefinitionEditor::_on_resource_id_editor_changed));
	}
	if (categories_in_item) {
		categories_in_item->connect("changed", callable_mp(this, &ItemDefinitionEditor::_on_categories_in_item_changed));
	}
	if (size_x_spin_box) {
		size_x_spin_box->connect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_x_spin_box_value_changed));
	}
	if (size_y_spin_box) {
		size_y_spin_box->connect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_y_spin_box_value_changed));
	}
}

void ItemDefinitionEditor::_disconnect_signals() {
	if (can_stack_check_box && can_stack_check_box->is_connected("toggled", callable_mp(this, &ItemDefinitionEditor::_can_stack_check_box_toggled))) {
		can_stack_check_box->disconnect("toggled", callable_mp(this, &ItemDefinitionEditor::_can_stack_check_box_toggled));
	}
	if (icon_selector && icon_selector->is_connected("icon_changed", callable_mp(this, &ItemDefinitionEditor::_on_icon_changed))) {
		icon_selector->disconnect("icon_changed", callable_mp(this, &ItemDefinitionEditor::_on_icon_changed));
	}
	if (item_description_text_edit && item_description_text_edit->is_connected("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_item_description_text_edit_text_changed))) {
		item_description_text_edit->disconnect("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_item_description_text_edit_text_changed));
	}
	if (item_name_text_edit && item_name_text_edit->is_connected("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_text_edit_text_changed))) {
		item_name_text_edit->disconnect("text_changed", callable_mp(this, &ItemDefinitionEditor::_on_text_edit_text_changed));
	}
	if (item_max_stack_spin_box && item_max_stack_spin_box->is_connected("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_max_stack_spin_box_value_changed))) {
		item_max_stack_spin_box->disconnect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_max_stack_spin_box_value_changed));
	}
	if (weight_spin_box && weight_spin_box->is_connected("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_weight_spin_box_value_changed))) {
		weight_spin_box->disconnect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_weight_spin_box_value_changed));
	}
	if (resource_id_editor && resource_id_editor->is_connected("changed", callable_mp(this, &ItemDefinitionEditor::_on_resource_id_editor_changed))) {
		resource_id_editor->disconnect("changed", callable_mp(this, &ItemDefinitionEditor::_on_resource_id_editor_changed));
	}
	if (categories_in_item && categories_in_item->is_connected("changed", callable_mp(this, &ItemDefinitionEditor::_on_categories_in_item_changed))) {
		categories_in_item->disconnect("changed", callable_mp(this, &ItemDefinitionEditor::_on_categories_in_item_changed));
	}
	if (size_x_spin_box && size_x_spin_box->is_connected("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_x_spin_box_value_changed))) {
		size_x_spin_box->disconnect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_x_spin_box_value_changed));
	}
	if (size_y_spin_box && size_y_spin_box->is_connected("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_y_spin_box_value_changed))) {
		size_y_spin_box->disconnect("value_changed", callable_mp(this, &ItemDefinitionEditor::_on_size_y_spin_box_value_changed));
	}
}

void ItemDefinitionEditor::load_item(const Ref<ItemDefinition>& p_item, InventoryDatabase* p_database) {
	// Disconnect signals to prevent unwanted updates during loading
	_disconnect_signals();

	item = p_item;
	database = p_database;
	
	if (item.is_valid()) {
		scroll_container->set_visible(true);
		
		// Setup resource_id_editor
		if (resource_id_editor && database) {
			resource_id_editor->setup(database, item->get_id());
		}

		item_name_text_edit->set_text(item->get_name());
		item_max_stack_spin_box->set_value(item->get_max_stack());
		weight_spin_box->set_value(item->get_weight());
		can_stack_check_box->set_pressed(item->get_can_stack());
		
		if (icon_selector) {
			icon_selector->load_icon(item->get_icon());
		}
		
		max_stack_container->set_visible(item->get_can_stack());
		size_x_spin_box->set_value(item->get_size().x);
		size_y_spin_box->set_value(item->get_size().y);
		item_description_text_edit->set_text(item->get_description());
		
		// Load custom properties
		if (custom_properties) {
			custom_properties->load_resource(database, item);
		}
		
		// Load categories
		if (categories_in_item) {
			categories_in_item->load_item(database, item);
		}
		
		size_x_spin_box->set_editable(true);
	} else {
		scroll_container->set_visible(false);
	}

	_connect_signals();
}

void ItemDefinitionEditor::reload_item() {
	load_item(item, database);
}

// Signal handlers - matching addon method names exactly
void ItemDefinitionEditor::_on_max_stack_spin_box_value_changed(double value) {
	if (item.is_valid()) {
		item->set_max_stack((int)item_max_stack_spin_box->get_value());
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_text_edit_text_changed(const String& new_text) {
	if (item.is_valid()) {
		item->set_name(item_name_text_edit->get_text());
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_icon_changed(const Ref<Texture2D>& icon) {
	if (item.is_valid()) {
		item->set_icon(icon);
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_resource_id_editor_changed(const String& id) {
	if (item.is_valid()) {
		item->set_id(id);
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_weight_spin_box_value_changed(double value) {
	if (item.is_valid()) {
		item->set_weight(weight_spin_box->get_value());
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_can_stack_check_box_toggled(bool value) {
	if (item.is_valid()) {
		item->set_can_stack(value);
		max_stack_container->set_visible(item->get_can_stack());
		emit_signal("changed", item);
		if (custom_properties) {
			custom_properties->loading_properties(); // Reload custom properties when can_stack changes
		}
	}
}

void ItemDefinitionEditor::_on_size_x_spin_box_value_changed(double value) {
	if (item.is_valid()) {
		Vector2i current_size = item->get_size();
		item->set_size(Vector2i((int)value, current_size.y));
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_size_y_spin_box_value_changed(double value) {
	if (item.is_valid()) {
		Vector2i current_size = item->get_size();
		item->set_size(Vector2i(current_size.x, (int)value));
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_item_description_text_edit_text_changed() {
	if (item.is_valid()) {
		item->set_description(item_description_text_edit->get_text());
		emit_signal("changed", item);
	}
}

void ItemDefinitionEditor::_on_categories_in_item_changed() {
	if (item.is_valid()) {
		emit_signal("changed", item);
	}
}

#endif // TOOLS_ENABLED
