#ifdef TOOLS_ENABLED

#include "custom_properties_editor.h"
#include "../base/inventory_database.h"
#include "../base/item_category.h"
#include "../base/item_definition.h"
#include "property_type_icons.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void CustomPropertiesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_resource_type", "type"), &CustomPropertiesEditor::set_resource_type);
	ClassDB::bind_method(D_METHOD("set_title", "title"), &CustomPropertiesEditor::set_title);
	ClassDB::bind_method(D_METHOD("load_resource", "database", "resource"), &CustomPropertiesEditor::load_resource);

	// New signal handlers
	ClassDB::bind_method(D_METHOD("_on_add_property_button_pressed"), &CustomPropertiesEditor::_on_add_property_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_property_button_pressed"), &CustomPropertiesEditor::_on_remove_property_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_properties_list_item_selected", "index"), &CustomPropertiesEditor::_on_properties_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_property_name_text_changed", "text"), &CustomPropertiesEditor::_on_property_name_text_changed);
	ClassDB::bind_method(D_METHOD("_on_property_type_item_selected", "index"), &CustomPropertiesEditor::_on_property_type_item_selected);
	ClassDB::bind_method(D_METHOD("_on_string_value_text_changed", "text"), &CustomPropertiesEditor::_on_string_value_text_changed);
	ClassDB::bind_method(D_METHOD("_on_int_value_changed", "value"), &CustomPropertiesEditor::_on_int_value_changed);
	ClassDB::bind_method(D_METHOD("_on_float_value_changed", "value"), &CustomPropertiesEditor::_on_float_value_changed);
	ClassDB::bind_method(D_METHOD("_on_bool_value_toggled", "pressed"), &CustomPropertiesEditor::_on_bool_value_toggled);
	ClassDB::bind_method(D_METHOD("_on_color_value_changed", "color"), &CustomPropertiesEditor::_on_color_value_changed);
	ClassDB::bind_method(D_METHOD("_on_resource_value_changed", "resource"), &CustomPropertiesEditor::_on_resource_value_changed);
	ClassDB::bind_method(D_METHOD("_on_dynamic_property_toggled", "pressed"), &CustomPropertiesEditor::_on_dynamic_property_toggled);

	BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_DEFINITION);
	BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_CATEGORY);
}

CustomPropertiesEditor::CustomPropertiesEditor() {
	resource_type = RESOURCE_TYPE_ITEM_DEFINITION;
	database = nullptr;
	v_box_container = nullptr;
	title_text = "Custom Properties";

	// Initialize new UI pointers
	properties_label = nullptr;
	add_property_button = nullptr;
	remove_property_button = nullptr;
	properties_list = nullptr;
	property_details_vbox = nullptr;
	property_name_line_edit = nullptr;
	property_type_option = nullptr;
	property_value_container = nullptr;
	string_value_edit = nullptr;
	int_value_spinbox = nullptr;
	float_value_spinbox = nullptr;
	bool_value_checkbox = nullptr;
	color_value_picker = nullptr;
	resource_value_picker = nullptr;
	dynamic_property_checkbox = nullptr;
	selected_property_name = "";
}

CustomPropertiesEditor::~CustomPropertiesEditor() {
}

void CustomPropertiesEditor::_ready() {
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Title label
	Label *title_label = memnew(Label);
	title_label->set_text(title_text);
	title_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	add_child(title_label);

	// Create horizontal split container for properties list and details
	HSplitContainer *properties_split = memnew(HSplitContainer);
	add_child(properties_split);
	properties_split->set_split_offset(-128);
	properties_split->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Create VBoxContainer for left side (properties header + list)
	VBoxContainer *properties_left_container = memnew(VBoxContainer);
	properties_split->add_child(properties_left_container);
	properties_left_container->set_custom_minimum_size(Vector2(256, 0));
	properties_left_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Properties section header - inside the left container
	HBoxContainer *properties_header = memnew(HBoxContainer);
	properties_left_container->add_child(properties_header);

	properties_label = memnew(Label);
	properties_header->add_child(properties_label);
	properties_label->set_text("Properties:");
	properties_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	add_property_button = memnew(Button);
	properties_header->add_child(add_property_button);
	add_property_button->set_tooltip_text("Add Property");
	add_property_button->set_custom_minimum_size(Vector2(32, 32));
	add_property_button->set_flat(true);
	add_property_button->connect("pressed", callable_mp(this, &CustomPropertiesEditor::_on_add_property_button_pressed));

	remove_property_button = memnew(Button);
	properties_header->add_child(remove_property_button);
	remove_property_button->set_tooltip_text("Remove Property");
	remove_property_button->set_custom_minimum_size(Vector2(32, 32));
	remove_property_button->set_flat(true);
	remove_property_button->set_disabled(true);
	remove_property_button->connect("pressed", callable_mp(this, &CustomPropertiesEditor::_on_remove_property_button_pressed));

	// Properties list - inside the left container
	properties_list = memnew(ItemList);
	properties_left_container->add_child(properties_list);
	properties_list->set_custom_minimum_size(Vector2(200, 100));
	properties_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	properties_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	properties_list->connect("item_selected", callable_mp(this, &CustomPropertiesEditor::_on_properties_list_item_selected));

	// Create persistent details container - right side (never hidden to maintain size)
	VBoxContainer *details_container = memnew(VBoxContainer);
	properties_split->add_child(details_container);
	details_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	details_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	details_container->set_custom_minimum_size(Vector2(300, 0));

	// Property details section inside the persistent container
	property_details_vbox = memnew(VBoxContainer);
	details_container->add_child(property_details_vbox);
	property_details_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	property_details_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	property_details_vbox->set_visible(false);

	Label *details_label = memnew(Label);
	property_details_vbox->add_child(details_label);
	details_label->set_text("Property Details:");

	// Property name field
	HBoxContainer *name_hbox = memnew(HBoxContainer);
	property_details_vbox->add_child(name_hbox);

	Label *name_label = memnew(Label);
	name_hbox->add_child(name_label);
	name_label->set_text("Property Name:");
	name_label->set_custom_minimum_size(Vector2(160, 0));

	property_name_line_edit = memnew(LineEdit);
	name_hbox->add_child(property_name_line_edit);
	property_name_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	property_name_line_edit->set_placeholder("Enter property name");
	property_name_line_edit->connect("text_changed", callable_mp(this, &CustomPropertiesEditor::_on_property_name_text_changed));

	// Property type field
	HBoxContainer *type_hbox = memnew(HBoxContainer);
	property_details_vbox->add_child(type_hbox);

	Label *type_label = memnew(Label);
	type_hbox->add_child(type_label);
	type_label->set_text("Value Type:");
	type_label->set_custom_minimum_size(Vector2(160, 0));

	property_type_option = memnew(OptionButton);
	type_hbox->add_child(property_type_option);
	property_type_option->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	property_type_option->connect("item_selected", callable_mp(this, &CustomPropertiesEditor::_on_property_type_item_selected));

	// Dynamic property checkbox
	HBoxContainer *dynamic_hbox = memnew(HBoxContainer);
	property_details_vbox->add_child(dynamic_hbox);

	Label *dynamic_label = memnew(Label);
	dynamic_hbox->add_child(dynamic_label);
	dynamic_label->set_text("Dynamic Property:");
	dynamic_label->set_custom_minimum_size(Vector2(160, 0));

	dynamic_property_checkbox = memnew(CheckBox);
	dynamic_hbox->add_child(dynamic_property_checkbox);
	dynamic_property_checkbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	dynamic_property_checkbox->connect("toggled", callable_mp(this, &CustomPropertiesEditor::_on_dynamic_property_toggled));

	// Property value container - will hold different controls based on type
	property_value_container = memnew(Control);
	property_details_vbox->add_child(property_value_container);
	property_value_container->set_custom_minimum_size(Vector2(0, 32));

	// Create all value controls (initially hidden)
	_create_value_controls();

	build_type_options();
	_apply_theme();
}

void CustomPropertiesEditor::set_resource_type(ResourceType p_type) {
	resource_type = p_type;
}

void CustomPropertiesEditor::set_title(const String &p_title) {
	title_text = p_title;
}

void CustomPropertiesEditor::load_resource(InventoryDatabase *p_database, const Ref<Resource> &p_resource) {
	database = p_database;
	current_resource = p_resource;
	_update_properties_list();
}

Dictionary CustomPropertiesEditor::get_properties_from_resource() {
	if (!current_resource.is_valid()) {
		return Dictionary();
	}

	if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
		Ref<ItemDefinition> item = current_resource;
		if (item.is_valid()) {
			return item->get_properties();
		}
	} else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
		Ref<ItemCategory> category = current_resource;
		if (category.is_valid()) {
			return category->get_item_properties();
		}
	}

	return Dictionary();
}

void CustomPropertiesEditor::set_properties_to_resource(const Dictionary &properties) {
	if (!current_resource.is_valid()) {
		return;
	}

	if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
		Ref<ItemDefinition> item = current_resource;
		if (item.is_valid()) {
			item->set_properties(properties);
		}
	} else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
		Ref<ItemCategory> category = current_resource;
		if (category.is_valid()) {
			category->set_item_properties(properties);
		}
	}
}

TypedArray<String> CustomPropertiesEditor::get_dynamic_properties_from_resource() {
	if (!current_resource.is_valid()) {
		return TypedArray<String>();
	}

	if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
		Ref<ItemDefinition> item = current_resource;
		if (item.is_valid()) {
			return item->get_dynamic_properties();
		}
	} else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
		Ref<ItemCategory> category = current_resource;
		if (category.is_valid()) {
			return category->get_item_dynamic_properties();
		}
	}

	return TypedArray<String>();
}

void CustomPropertiesEditor::set_dynamic_properties_to_resource(const TypedArray<String> &dynamic_properties) {
	if (!current_resource.is_valid()) {
		return;
	}

	if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
		Ref<ItemDefinition> item = current_resource;
		if (item.is_valid()) {
			item->set_dynamic_properties(dynamic_properties);
		}
	} else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
		Ref<ItemCategory> category = current_resource;
		if (category.is_valid()) {
			category->set_item_dynamic_properties(dynamic_properties);
		}
	}
}

Dictionary CustomPropertiesEditor::make_dictionary_unique(const Dictionary &input_dictionary) {
	Dictionary unique_dictionary;
	Array keys = input_dictionary.keys();
	for (int i = 0; i < keys.size(); i++) {
		Variant key = keys[i];
		unique_dictionary[key] = input_dictionary[key];
	}
	return unique_dictionary;
}

void CustomPropertiesEditor::_create_value_controls() {
	if (!property_value_container)
		return;

	// String value control
	HBoxContainer *string_hbox = memnew(HBoxContainer);
	property_value_container->add_child(string_hbox);
	string_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	string_hbox->set_visible(false);

	Label *string_label = memnew(Label);
	string_hbox->add_child(string_label);
	string_label->set_text("String Value:");
	string_label->set_custom_minimum_size(Vector2(160, 0));

	string_value_edit = memnew(LineEdit);
	string_hbox->add_child(string_value_edit);
	string_value_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	string_value_edit->connect("text_changed", callable_mp(this, &CustomPropertiesEditor::_on_string_value_text_changed));

	// Integer value control
	HBoxContainer *int_hbox = memnew(HBoxContainer);
	property_value_container->add_child(int_hbox);
	int_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	int_hbox->set_visible(false);

	Label *int_label = memnew(Label);
	int_hbox->add_child(int_label);
	int_label->set_text("Integer Value:");
	int_label->set_custom_minimum_size(Vector2(160, 0));

	int_value_spinbox = memnew(SpinBox);
	int_hbox->add_child(int_value_spinbox);
	int_value_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	int_value_spinbox->set_min(-999999);
	int_value_spinbox->set_max(999999);
	int_value_spinbox->set_step(1);
	int_value_spinbox->connect("value_changed", callable_mp(this, &CustomPropertiesEditor::_on_int_value_changed));

	// Float value control
	HBoxContainer *float_hbox = memnew(HBoxContainer);
	property_value_container->add_child(float_hbox);
	float_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	float_hbox->set_visible(false);

	Label *float_label = memnew(Label);
	float_hbox->add_child(float_label);
	float_label->set_text("Float Value:");
	float_label->set_custom_minimum_size(Vector2(160, 0));

	float_value_spinbox = memnew(SpinBox);
	float_hbox->add_child(float_value_spinbox);
	float_value_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	float_value_spinbox->set_min(-999999);
	float_value_spinbox->set_max(999999);
	float_value_spinbox->set_step(0.01);
	float_value_spinbox->connect("value_changed", callable_mp(this, &CustomPropertiesEditor::_on_float_value_changed));

	// Boolean value control
	HBoxContainer *bool_hbox = memnew(HBoxContainer);
	property_value_container->add_child(bool_hbox);
	bool_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	bool_hbox->set_visible(false);

	Label *bool_label = memnew(Label);
	bool_hbox->add_child(bool_label);
	bool_label->set_text("Boolean Value:");
	bool_label->set_custom_minimum_size(Vector2(160, 0));

	bool_value_checkbox = memnew(CheckBox);
	bool_hbox->add_child(bool_value_checkbox);
	bool_value_checkbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	bool_value_checkbox->connect("toggled", callable_mp(this, &CustomPropertiesEditor::_on_bool_value_toggled));

	// Color value control
	HBoxContainer *color_hbox = memnew(HBoxContainer);
	property_value_container->add_child(color_hbox);
	color_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	color_hbox->set_visible(false);

	Label *color_label = memnew(Label);
	color_hbox->add_child(color_label);
	color_label->set_text("Color Value:");
	color_label->set_custom_minimum_size(Vector2(160, 0));

	color_value_picker = memnew(ColorPickerButton);
	color_hbox->add_child(color_value_picker);
	color_value_picker->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	color_value_picker->connect("color_changed", callable_mp(this, &CustomPropertiesEditor::_on_color_value_changed));

	// Resource value control
	HBoxContainer *resource_hbox = memnew(HBoxContainer);
	property_value_container->add_child(resource_hbox);
	resource_hbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	resource_hbox->set_visible(false);

	Label *resource_label = memnew(Label);
	resource_hbox->add_child(resource_label);
	resource_label->set_text("Resource Value:");
	resource_label->set_custom_minimum_size(Vector2(160, 0));

	resource_value_picker = memnew(EditorResourcePicker);
	resource_hbox->add_child(resource_value_picker);
	resource_value_picker->set_base_type("Resource");
	resource_value_picker->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	resource_value_picker->connect("resource_changed", callable_mp(this, &CustomPropertiesEditor::_on_resource_value_changed));
}

void CustomPropertiesEditor::_apply_theme() {
	if (!add_property_button || !remove_property_button) {
		return;
	}

	// Set icons for add and remove buttons
	Ref<Texture2D> add_icon = get_theme_icon("Add", "EditorIcons");
	if (add_icon.is_valid()) {
		add_property_button->set_button_icon(add_icon);
		add_property_button->set_text(""); // Remove text when icon is available
	}

	Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
	if (remove_icon.is_valid()) {
		remove_property_button->set_button_icon(remove_icon);
		remove_property_button->set_text(""); // Remove text when icon is available
	}
}

void CustomPropertiesEditor::_update_properties_list() {
	if (!properties_list) {
		return;
	}

	// Clear existing items
	properties_list->clear();

	if (!current_resource.is_valid()) {
		remove_property_button->set_disabled(true);
		property_details_vbox->set_visible(false);
		return;
	}

	Dictionary properties = get_properties_from_resource();
	Array keys = properties.keys();

	for (int i = 0; i < keys.size(); i++) {
		if (keys[i].get_type() == Variant::STRING) {
			String property_name = keys[i];
			Variant property_value = properties[property_name];
			Variant::Type property_type = property_value.get_type();

			// Add item with icon based on property type
			properties_list->add_item(property_name);
			Ref<Texture2D> icon = get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(property_type), "EditorIcons");
			properties_list->set_item_icon(i, icon);

			// Restore selection if this was the previously selected property
			if (property_name == selected_property_name) {
				properties_list->select(i);
			}
		}
	}

	// Update button states
	remove_property_button->set_disabled(properties_list->get_item_count() == 0);

	// If no selection but items exist, hide details
	if (properties_list->get_selected_items().is_empty() && properties_list->get_item_count() > 0) {
		property_details_vbox->set_visible(false);
	} else {
		_update_property_details();
	}
}

void CustomPropertiesEditor::_update_property_details() {
	if (!property_details_vbox || !properties_list) {
		return;
	}

	Array selected = properties_list->get_selected_items();
	if (selected.is_empty()) {
		property_details_vbox->set_visible(false);
		return;
	}

	int index = selected[0];
	if (index < 0 || index >= properties_list->get_item_count()) {
		return;
	}

	String property_name = properties_list->get_item_text(index);
	selected_property_name = property_name;

	Dictionary properties = get_properties_from_resource();
	if (!properties.has(property_name)) {
		return;
	}

	property_details_vbox->set_visible(true);

	// Update property name field
	property_name_line_edit->set_text(property_name);

	// Update property value and type
	Variant value = properties[property_name];
	int type = value.get_type();

	// Set the type option
	for (int i = 0; i < property_type_option->get_item_count(); i++) {
		if (property_type_option->get_item_id(i) == type) {
			property_type_option->select(i);
			break;
		}
	}

	// Show appropriate value control and set its value
	_show_property_value_control(type);

	switch (type) {
		case Variant::STRING:
			string_value_edit->set_text(value);
			break;
		case Variant::INT:
			int_value_spinbox->set_value(value);
			break;
		case Variant::FLOAT:
			float_value_spinbox->set_value(value);
			break;
		case Variant::BOOL:
			bool_value_checkbox->set_pressed(value);
			break;
		case Variant::COLOR:
			color_value_picker->set_pick_color(value);
			break;
		case Variant::OBJECT:
			{
				Ref<Resource> resource = value;
				resource_value_picker->set_edited_resource(resource);
			}
			break;
	}

	// Update dynamic property checkbox
	TypedArray<String> dynamic_props = get_dynamic_properties_from_resource();
	bool is_dynamic = dynamic_props.find(property_name) != -1;
	dynamic_property_checkbox->set_pressed(is_dynamic);
}

void CustomPropertiesEditor::_show_property_value_control(int type) {
	if (!property_value_container)
		return;

	// Hide all value controls first
	for (int i = 0; i < property_value_container->get_child_count(); i++) {
		Control *child = Object::cast_to<Control>(property_value_container->get_child(i));
		if (child) {
			child->set_visible(false);
		}
	}

	// Show the appropriate control based on type
	int control_index = -1;
	switch (type) {
		case Variant::STRING:
			control_index = 0;
			break;
		case Variant::INT:
			control_index = 1;
			break;
		case Variant::FLOAT:
			control_index = 2;
			break;
		case Variant::BOOL:
			control_index = 3;
			break;
		case Variant::COLOR:
			control_index = 4;
			break;
		case Variant::OBJECT:
			control_index = 5; // Resource picker
			break;
	}

	if (control_index >= 0 && control_index < property_value_container->get_child_count()) {
		Control *child = Object::cast_to<Control>(property_value_container->get_child(control_index));
		if (child) {
			child->set_visible(true);
		}
	}
}

void CustomPropertiesEditor::build_type_options() {
	if (!property_type_option)
		return;

	property_type_option->clear();
	// Use proper icon names for Godot editor icons
	property_type_option->add_icon_item(get_theme_icon("bool", "EditorIcons"), "Boolean", Variant::BOOL);
	property_type_option->add_icon_item(get_theme_icon("int", "EditorIcons"), "Integer", Variant::INT);
	property_type_option->add_icon_item(get_theme_icon("float", "EditorIcons"), "Float", Variant::FLOAT);
	property_type_option->add_icon_item(get_theme_icon("String", "EditorIcons"), "String", Variant::STRING);
	property_type_option->add_icon_item(get_theme_icon("Color", "EditorIcons"), "Color", Variant::COLOR);
	property_type_option->add_icon_item(get_theme_icon("Object", "EditorIcons"), "Resource", Variant::OBJECT);

	// Select the first item after adding all items
	if (property_type_option->get_item_count() > 0) {
		property_type_option->select(0);
	}
}

// New signal handlers for list-based UI
void CustomPropertiesEditor::_on_add_property_button_pressed() {
	if (!current_resource.is_valid()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	String new_prop_name = "new_property_" + String::num_int64(properties.size());

	// Set default value as string
	properties[new_prop_name] = String();
	set_properties_to_resource(properties);

	_update_properties_list();

	// Select the newly added property
	for (int i = 0; i < properties_list->get_item_count(); i++) {
		if (properties_list->get_item_text(i) == new_prop_name) {
			properties_list->select(i);
			_update_property_details();
			break;
		}
	}
}

void CustomPropertiesEditor::_on_remove_property_button_pressed() {
	if (!current_resource.is_valid() || !properties_list) {
		return;
	}

	Array selected = properties_list->get_selected_items();
	if (selected.is_empty()) {
		return;
	}

	int index = selected[0];
	if (index < 0 || index >= properties_list->get_item_count()) {
		return;
	}

	String property_name = properties_list->get_item_text(index);

	Dictionary properties = get_properties_from_resource();
	properties.erase(property_name);
	set_properties_to_resource(properties);

	// Also remove from dynamic properties if it exists
	TypedArray<String> dynamic_properties = get_dynamic_properties_from_resource();
	int dynamic_index = dynamic_properties.find(property_name);
	if (dynamic_index != -1) {
		dynamic_properties.remove_at(dynamic_index);
		set_dynamic_properties_to_resource(dynamic_properties);
	}

	selected_property_name = "";
	_update_properties_list();
	property_details_vbox->set_visible(false);
}

void CustomPropertiesEditor::_on_properties_list_item_selected(int index) {
	_update_property_details();
}

void CustomPropertiesEditor::_on_property_name_text_changed(const String &text) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	if (text == selected_property_name) {
		return; // No change
	}

	Dictionary properties = get_properties_from_resource();
	if (!properties.has(selected_property_name)) {
		return;
	}

	// Rename the property
	Variant value = properties[selected_property_name];
	properties.erase(selected_property_name);
	properties[text] = value;
	set_properties_to_resource(properties);

	// Handle dynamic properties renaming
	TypedArray<String> dynamic_properties = get_dynamic_properties_from_resource();
	int dynamic_index = dynamic_properties.find(selected_property_name);
	if (dynamic_index != -1) {
		dynamic_properties[dynamic_index] = text;
		set_dynamic_properties_to_resource(dynamic_properties);
	}

	selected_property_name = text;
	_update_properties_list();

	// Reselect the renamed property
	for (int i = 0; i < properties_list->get_item_count(); i++) {
		if (properties_list->get_item_text(i) == text) {
			properties_list->select(i);
			break;
		}
	}
}

void CustomPropertiesEditor::_on_property_type_item_selected(int index) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	int type = property_type_option->get_item_id(index);
	Dictionary properties = get_properties_from_resource();

	// Convert current value to new type with appropriate default
	Variant new_value;
	switch (type) {
		case Variant::STRING:
			new_value = String();
			break;
		case Variant::INT:
			new_value = 0;
			break;
		case Variant::FLOAT:
			new_value = 0.0;
			break;
		case Variant::BOOL:
			new_value = false;
			break;
		case Variant::COLOR:
			new_value = Color();
			break;
		case Variant::OBJECT:
			new_value = Ref<Resource>();
			break;
		default:
			new_value = String();
			break;
	}

	properties[selected_property_name] = new_value;
	set_properties_to_resource(properties);

	_show_property_value_control(type);
	_update_property_details(); // Refresh the value display
}

void CustomPropertiesEditor::_on_string_value_text_changed(const String &text) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = text;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_int_value_changed(double value) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = (int)value;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_float_value_changed(double value) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = value;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_bool_value_toggled(bool pressed) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = pressed;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_color_value_changed(const Color &color) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = color;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_resource_value_changed(const Ref<Resource> &resource) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	Dictionary properties = get_properties_from_resource();
	properties[selected_property_name] = resource;
	set_properties_to_resource(properties);
}

void CustomPropertiesEditor::_on_dynamic_property_toggled(bool pressed) {
	if (!current_resource.is_valid() || selected_property_name.is_empty()) {
		return;
	}

	TypedArray<String> dynamic_properties = get_dynamic_properties_from_resource();
	int index = dynamic_properties.find(selected_property_name);
	bool currently_dynamic = index != -1;

	if (currently_dynamic != pressed) {
		if (pressed) {
			dynamic_properties.append(selected_property_name);
		} else {
			dynamic_properties.remove_at(index);
		}
		set_dynamic_properties_to_resource(dynamic_properties);
	}
}

#endif // TOOLS_ENABLED