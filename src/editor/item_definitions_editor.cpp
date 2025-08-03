/**
 * item_definitions_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "item_definitions_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_definition.h"

#include <godot_cpp/classes/editor_inspector.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void ItemDefinitionsEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_item_selected", "item", "index"), &ItemDefinitionsEditor::_on_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_popup_menu_requested", "at_position"), &ItemDefinitionsEditor::_on_item_popup_menu_requested);
	
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_id_changed", "text"), &ItemDefinitionsEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_id_focus_exited"), &ItemDefinitionsEditor::_on_id_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_name_changed", "text"), &ItemDefinitionsEditor::_on_name_changed);
	ClassDB::bind_method(D_METHOD("_on_name_focus_exited"), &ItemDefinitionsEditor::_on_name_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_description_changed"), &ItemDefinitionsEditor::_on_description_changed);
	ClassDB::bind_method(D_METHOD("_on_can_stack_changed", "pressed"), &ItemDefinitionsEditor::_on_can_stack_changed);
	ClassDB::bind_method(D_METHOD("_on_max_stack_changed", "value"), &ItemDefinitionsEditor::_on_max_stack_changed);
	ClassDB::bind_method(D_METHOD("_on_weight_changed", "value"), &ItemDefinitionsEditor::_on_weight_changed);
	ClassDB::bind_method(D_METHOD("_on_size_width_changed", "value"), &ItemDefinitionsEditor::_on_size_width_changed);
	ClassDB::bind_method(D_METHOD("_on_size_height_changed", "value"), &ItemDefinitionsEditor::_on_size_height_changed);
	ClassDB::bind_method(D_METHOD("_on_icon_button_pressed"), &ItemDefinitionsEditor::_on_icon_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_clear_icon_pressed"), &ItemDefinitionsEditor::_on_clear_icon_pressed);
	ClassDB::bind_method(D_METHOD("_on_icon_dialog_file_selected", "path"), &ItemDefinitionsEditor::_on_icon_dialog_file_selected);

	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")));
	ADD_SIGNAL(MethodInfo("duplicated", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")));
}

void ItemDefinitionsEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

ItemDefinitionsEditor::ItemDefinitionsEditor() {
	editor_plugin = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	// Create icon dialog
	icon_dialog = memnew(FileDialog);
	add_child(icon_dialog);
	icon_dialog->set_access(FileDialog::ACCESS_RESOURCES);
	icon_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_FILE);
	icon_dialog->add_filter("*.png", "PNG Images");
	icon_dialog->add_filter("*.jpg", "JPEG Images");
	icon_dialog->add_filter("*.jpeg", "JPEG Images");
	icon_dialog->add_filter("*.svg", "SVG Images");
	icon_dialog->add_filter("*.webp", "WebP Images");
	icon_dialog->connect("file_selected", callable_mp(this, &ItemDefinitionsEditor::_on_icon_dialog_file_selected));
}

ItemDefinitionsEditor::~ItemDefinitionsEditor() {
}

void ItemDefinitionsEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void ItemDefinitionsEditor::_create_ui() {
	// Split container (HSplitContainer)
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	// Item list editor (left side) - matches .tscn minimum size
	item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(item_list_editor);
	item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	item_list_editor->connect("item_selected", callable_mp(this, &ItemDefinitionsEditor::_on_item_selected));
	item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &ItemDefinitionsEditor::_on_item_popup_menu_requested));

	// Details scroll container (right side) - matches .tscn structure exactly
	details_scroll = memnew(ScrollContainer);
	hsplit_container->add_child(details_scroll);
	details_scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	details_scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	details_scroll->set_visible(false); // Initially invisible like in .tscn
	
	// Margin container inside scroll - matches .tscn margins exactly
	MarginContainer *scroll_margin = memnew(MarginContainer);
	details_scroll->add_child(scroll_margin);
	scroll_margin->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	scroll_margin->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	scroll_margin->add_theme_constant_override("margin_left", 8);
	scroll_margin->add_theme_constant_override("margin_top", 8);
	scroll_margin->add_theme_constant_override("margin_right", 0);
	scroll_margin->add_theme_constant_override("margin_bottom", 8);

	// Details container (VBoxContainer)
	details_container = memnew(VBoxContainer);
	scroll_margin->add_child(details_container);

	// No selection label
	no_selection_label = memnew(Label);
	hsplit_container->add_child(no_selection_label);
	no_selection_label->set_text("Select an item to edit its properties");
	no_selection_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	no_selection_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	no_selection_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	no_selection_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void ItemDefinitionsEditor::load_from_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		TypedArray<ItemDefinition> items = database->get_items();
		Array item_array;
		for (int i = 0; i < items.size(); i++) {
			item_array.append(items[i]);
		}
		item_list_editor->set_items(item_array);
	} else {
		item_list_editor->clear_items();
	}
	
	_clear_details();
}

void ItemDefinitionsEditor::select_item(const Ref<ItemDefinition> &p_item) {
	if (p_item.is_valid()) {
		item_list_editor->select_item(p_item);
		_update_details(p_item);
	}
}

void ItemDefinitionsEditor::_update_details(const Ref<ItemDefinition> &p_item) {
	current_item = p_item;
	
	// Clear existing details
	_clear_details();
	
	if (p_item.is_null()) {
		no_selection_label->set_visible(true);
		details_scroll->set_visible(false);
		return;
	}
	
	no_selection_label->set_visible(false);
	details_scroll->set_visible(true);
	
	// Create proper editor UI that matches .tscn structure exactly
	
	// Main ScrollContainer -> MarginContainer -> VBoxContainer (already created in _create_ui)
	// Now create the content inside
	
	// Top section: HBoxContainer2 with side-by-side layout
	HBoxContainer *top_hbox = memnew(HBoxContainer);
	details_container->add_child(top_hbox);
	
	// Left side: VBoxContainer for ID and Name
	VBoxContainer *left_vbox = memnew(VBoxContainer);
	top_hbox->add_child(left_vbox);
	left_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// ID Editor (HBoxContainer to match .tscn)
	HBoxContainer *id_container = memnew(HBoxContainer);
	left_vbox->add_child(id_container);
	id_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	Label *id_label = memnew(Label);
	id_container->add_child(id_label);
	id_label->set_text("ID");
	id_label->set_custom_minimum_size(Vector2(160, 0));
	
	LineEdit *id_edit = memnew(LineEdit);
	id_container->add_child(id_edit);
	id_edit->set_text(p_item->get_id());
	id_edit->set_placeholder("Enter unique item ID");
	id_edit->set_custom_minimum_size(Vector2(0, 32));
	id_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	id_edit->connect("text_submitted", callable_mp(this, &ItemDefinitionsEditor::_on_id_changed));
	id_edit->connect("focus_exited", callable_mp(this, &ItemDefinitionsEditor::_on_id_focus_exited));
	
	// Name Editor (HBoxContainer to match .tscn)
	HBoxContainer *name_container = memnew(HBoxContainer);
	left_vbox->add_child(name_container);
	name_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	Label *name_label = memnew(Label);
	name_container->add_child(name_label);
	name_label->set_text("Name");
	name_label->set_custom_minimum_size(Vector2(160, 0));
	
	LineEdit *name_edit = memnew(LineEdit);
	name_container->add_child(name_edit);
	name_edit->set_text(p_item->get_name());
	name_edit->set_placeholder("Place Item Name Here");
	name_edit->set_custom_minimum_size(Vector2(0, 32));
	name_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	name_edit->connect("text_submitted", callable_mp(this, &ItemDefinitionsEditor::_on_name_changed));
	name_edit->connect("focus_exited", callable_mp(this, &ItemDefinitionsEditor::_on_name_focus_exited));
	
	// Spacer Control to match .tscn
	Control *spacer = memnew(Control);
	left_vbox->add_child(spacer);
	spacer->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	// VSeparator between left and right sides
	VSeparator *v_sep = memnew(VSeparator);
	top_hbox->add_child(v_sep);
	
	// Right side: Icon selector 
	HBoxContainer *icon_selector = memnew(HBoxContainer);
	top_hbox->add_child(icon_selector);
	icon_selector->set_custom_minimum_size(Vector2(192, 31));
	
	// Icon preview button
	TextureButton *icon_preview = memnew(TextureButton);
	icon_selector->add_child(icon_preview);
	icon_preview->set_custom_minimum_size(Vector2(64, 64));
	icon_preview->set_stretch_mode(TextureButton::STRETCH_KEEP_ASPECT_CENTERED);
	if (p_item->get_icon().is_valid()) {
		icon_preview->set_texture_normal(p_item->get_icon());
	}
	icon_preview->connect("pressed", callable_mp(this, &ItemDefinitionsEditor::_on_icon_button_pressed));
	
	VBoxContainer *icon_buttons = memnew(VBoxContainer);
	icon_selector->add_child(icon_buttons);
	
	Button *select_icon_btn = memnew(Button);
	icon_buttons->add_child(select_icon_btn);
	select_icon_btn->set_text("Select Icon");
	select_icon_btn->connect("pressed", callable_mp(this, &ItemDefinitionsEditor::_on_icon_button_pressed));
	
	Button *clear_icon_btn = memnew(Button);
	icon_buttons->add_child(clear_icon_btn);
	clear_icon_btn->set_text("Clear Icon");
	clear_icon_btn->connect("pressed", callable_mp(this, &ItemDefinitionsEditor::_on_clear_icon_pressed));
	
	// Description section (HBoxContainer to match .tscn)
	HBoxContainer *desc_container = memnew(HBoxContainer);
	details_container->add_child(desc_container);
	
	Label *desc_label = memnew(Label);
	desc_container->add_child(desc_label);
	desc_label->set_text("Description");
	desc_label->set_custom_minimum_size(Vector2(160, 0));
	
	TextEdit *desc_edit = memnew(TextEdit);
	desc_container->add_child(desc_edit);
	desc_edit->set_text(p_item->get_description());
	desc_edit->set_placeholder("Enter item description here");
	desc_edit->set_custom_minimum_size(Vector2(0, 64));
	desc_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	desc_edit->connect("text_changed", callable_mp(this, &ItemDefinitionsEditor::_on_description_changed));
	
	// Can Stack section (HBoxContainer to match .tscn)
	HBoxContainer *can_stack_container = memnew(HBoxContainer);
	details_container->add_child(can_stack_container);
	
	Label *can_stack_label = memnew(Label);
	can_stack_container->add_child(can_stack_label);
	can_stack_label->set_text("Can Stack");
	can_stack_label->set_custom_minimum_size(Vector2(160, 0));
	
	CheckBox *can_stack_check = memnew(CheckBox);
	can_stack_container->add_child(can_stack_check);
	can_stack_check->set_pressed(p_item->get_can_stack());
	can_stack_check->connect("toggled", callable_mp(this, &ItemDefinitionsEditor::_on_can_stack_changed));
	
	// Max Stack section (HBoxContainer to match .tscn)
	HBoxContainer *max_stack_container = memnew(HBoxContainer);
	details_container->add_child(max_stack_container);
	
	Label *max_stack_label = memnew(Label);
	max_stack_container->add_child(max_stack_label);
	max_stack_label->set_text("Max Stack");
	max_stack_label->set_custom_minimum_size(Vector2(160, 0));
	
	SpinBox *max_stack_spin = memnew(SpinBox);
	max_stack_container->add_child(max_stack_spin);
	max_stack_spin->set_custom_minimum_size(Vector2(0, 32));
	max_stack_spin->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	max_stack_spin->set_allow_greater(true);
	max_stack_spin->set_value(p_item->get_max_stack());
	max_stack_spin->connect("value_changed", callable_mp(this, &ItemDefinitionsEditor::_on_max_stack_changed));
	
	// Weight and Size section (HBoxContainer to match .tscn)
	HBoxContainer *weight_size_container = memnew(HBoxContainer);
	details_container->add_child(weight_size_container);
	
	// Weight subsection
	HBoxContainer *weight_container = memnew(HBoxContainer);
	weight_size_container->add_child(weight_container);
	weight_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	Label *weight_label = memnew(Label);
	weight_container->add_child(weight_label);
	weight_label->set_text("Weight");
	weight_label->set_custom_minimum_size(Vector2(160, 0));
	
	SpinBox *weight_spin = memnew(SpinBox);
	weight_container->add_child(weight_spin);
	weight_spin->set_custom_minimum_size(Vector2(0, 32));
	weight_spin->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	weight_spin->set_max(100000.0);
	weight_spin->set_step(0.001);
	weight_spin->set_value(p_item->get_weight());
	weight_spin->set_allow_greater(true);
	weight_spin->set_allow_lesser(true);
	weight_spin->connect("value_changed", callable_mp(this, &ItemDefinitionsEditor::_on_weight_changed));
	
	// VSeparator between weight and size
	VSeparator *weight_size_sep = memnew(VSeparator);
	weight_size_container->add_child(weight_size_sep);
	
	// Size subsection
	HBoxContainer *size_container = memnew(HBoxContainer);
	weight_size_container->add_child(size_container);
	
	Label *grid_size_label = memnew(Label);
	size_container->add_child(grid_size_label);
	grid_size_label->set_text("Grid Size");
	grid_size_label->set_custom_minimum_size(Vector2(160, 0));
	
	Label *x_label = memnew(Label);
	size_container->add_child(x_label);
	x_label->set_text("X");
	x_label->set_custom_minimum_size(Vector2(32, 0));
	
	SpinBox *size_x_spin = memnew(SpinBox);
	size_container->add_child(size_x_spin);
	size_x_spin->set_custom_minimum_size(Vector2(64, 32));
	size_x_spin->set_min(1.0);
	size_x_spin->set_max(100000.0);
	size_x_spin->set_value(p_item->get_size().x);
	size_x_spin->set_allow_greater(true);
	size_x_spin->set_update_on_text_changed(true);
	size_x_spin->connect("value_changed", callable_mp(this, &ItemDefinitionsEditor::_on_size_width_changed));
	
	Label *y_label = memnew(Label);
	size_container->add_child(y_label);
	y_label->set_text("Y");
	y_label->set_custom_minimum_size(Vector2(32, 0));
	
	SpinBox *size_y_spin = memnew(SpinBox);
	size_container->add_child(size_y_spin);
	size_y_spin->set_custom_minimum_size(Vector2(64, 32));
	size_y_spin->set_min(1.0);
	size_y_spin->set_max(100000.0);
	size_y_spin->set_value(p_item->get_size().y);
	size_y_spin->set_allow_greater(true);
	size_y_spin->connect("value_changed", callable_mp(this, &ItemDefinitionsEditor::_on_size_height_changed));
}
}

void ItemDefinitionsEditor::_clear_details() {
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

void ItemDefinitionsEditor::_emit_removed(const Ref<ItemDefinition> &p_item) {
	emit_signal("removed", p_item);
}

void ItemDefinitionsEditor::_emit_duplicated(const Ref<ItemDefinition> &p_item) {
	emit_signal("duplicated", p_item);
}

void ItemDefinitionsEditor::_on_item_selected(const Variant &p_item, int p_index) {
	Ref<ItemDefinition> item = p_item;
	_update_details(item);
}

void ItemDefinitionsEditor::_on_item_popup_menu_requested(const Vector2 &p_position) {
	// TODO: Show context menu with options like duplicate, remove, etc.
	print_line("Context menu requested at position: " + p_position);
}

// Property change handlers
void ItemDefinitionsEditor::_on_id_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter unique item ID") {
				current_item->set_id(line_edit->get_text());
				break;
			}
		}
	}
}

void ItemDefinitionsEditor::_on_id_focus_exited() {
	// Handle focus exit - just trigger the change handler
	_on_id_changed("");
}

void ItemDefinitionsEditor::_on_name_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter item name") {
				current_item->set_name(line_edit->get_text());
				break;
			}
		}
	}
}

void ItemDefinitionsEditor::_on_name_focus_exited() {
	// Handle focus exit - just trigger the change handler  
	_on_name_changed("");
}

void ItemDefinitionsEditor::_on_description_changed() {
	if (current_item.is_valid()) {
		// Get the TextEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			TextEdit *text_edit = Object::cast_to<TextEdit>(children[i]);
			if (text_edit) {
				current_item->set_description(text_edit->get_text());
				break;
			}
		}
	}
}

void ItemDefinitionsEditor::_on_can_stack_changed(bool p_pressed) {
	if (current_item.is_valid()) {
		current_item->set_can_stack(p_pressed);
	}
}

void ItemDefinitionsEditor::_on_max_stack_changed(double p_value) {
	if (current_item.is_valid()) {
		current_item->set_max_stack((int)p_value);
	}
}

void ItemDefinitionsEditor::_on_weight_changed(double p_value) {
	if (current_item.is_valid()) {
		current_item->set_weight((float)p_value);
	}
}

void ItemDefinitionsEditor::_on_size_width_changed(double p_value) {
	if (current_item.is_valid()) {
		Vector2i current_size = current_item->get_size();
		current_item->set_size(Vector2i((int)p_value, current_size.y));
	}
}

void ItemDefinitionsEditor::_on_size_height_changed(double p_value) {
	if (current_item.is_valid()) {
		Vector2i current_size = current_item->get_size();
		current_item->set_size(Vector2i(current_size.x, (int)p_value));
	}
}

void ItemDefinitionsEditor::_on_icon_button_pressed() {
	icon_dialog->popup_centered(Vector2i(800, 600));
}

void ItemDefinitionsEditor::_on_clear_icon_pressed() {
	if (current_item.is_valid()) {
		current_item->set_icon(Ref<Texture2D>());
		// Refresh the details to update the icon preview
		_update_details(current_item);
	}
}

void ItemDefinitionsEditor::_on_icon_dialog_file_selected(const String &p_path) {
	if (current_item.is_valid()) {
		Ref<Resource> resource = ResourceLoader::get_singleton()->load(p_path);
		Ref<Texture2D> texture = resource;
		if (texture.is_valid()) {
			current_item->set_icon(texture);
			// Refresh the details to update the icon preview
			_update_details(current_item);
		}
	}
}

#endif // TOOLS_ENABLED