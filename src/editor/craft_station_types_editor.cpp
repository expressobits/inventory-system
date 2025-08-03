/**
 * craft_station_types_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "craft_station_types_editor.h"

#include "../base/inventory_database.h"
#include "../base/craft_station_type.h"

#include <godot_cpp/classes/editor_inspector.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void CraftStationTypesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_item_selected", "item", "index"), &CraftStationTypesEditor::_on_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_popup_menu_requested", "at_position"), &CraftStationTypesEditor::_on_item_popup_menu_requested);
	
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_id_changed", "text"), &CraftStationTypesEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_id_focus_exited"), &CraftStationTypesEditor::_on_id_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_name_changed", "text"), &CraftStationTypesEditor::_on_name_changed);
	ClassDB::bind_method(D_METHOD("_on_name_focus_exited"), &CraftStationTypesEditor::_on_name_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_icon_button_pressed"), &CraftStationTypesEditor::_on_icon_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_clear_icon_pressed"), &CraftStationTypesEditor::_on_clear_icon_pressed);
	ClassDB::bind_method(D_METHOD("_on_icon_dialog_file_selected", "path"), &CraftStationTypesEditor::_on_icon_dialog_file_selected);

	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType")));
	ADD_SIGNAL(MethodInfo("duplicated", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType")));
}

void CraftStationTypesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

CraftStationTypesEditor::CraftStationTypesEditor() {
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
	icon_dialog->connect("file_selected", callable_mp(this, &CraftStationTypesEditor::_on_icon_dialog_file_selected));
}

CraftStationTypesEditor::~CraftStationTypesEditor() {
}

void CraftStationTypesEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void CraftStationTypesEditor::_create_ui() {
	// Split container (HSplitContainer)
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	// Item list editor (left side) - matches .tscn minimum size
	item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(item_list_editor);
	item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	item_list_editor->connect("item_selected", callable_mp(this, &CraftStationTypesEditor::_on_item_selected));
	item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &CraftStationTypesEditor::_on_item_popup_menu_requested));

	// Details container (right side) - matches .tscn structure
	details_container = memnew(MarginContainer);
	hsplit_container->add_child(details_container);
	details_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	details_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	details_container->set_visible(false); // Initially invisible like in .tscn
	
	// VBoxContainer inside margin container
	details_vbox = memnew(VBoxContainer);
	details_container->add_child(details_vbox);

	// No selection label
	no_selection_label = memnew(Label);
	hsplit_container->add_child(no_selection_label);
	no_selection_label->set_text("Select a craft station type to edit its properties");
	no_selection_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	no_selection_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	no_selection_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	no_selection_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void CraftStationTypesEditor::load_from_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		TypedArray<CraftStationType> items = database->get_stations_type();
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

void CraftStationTypesEditor::select_item(const Ref<CraftStationType> &p_item) {
	if (p_item.is_valid()) {
		item_list_editor->select_item(p_item);
		_update_details(p_item);
	}
}

void CraftStationTypesEditor::_update_details(const Ref<CraftStationType> &p_item) {
	current_item = p_item;
	
	// Clear existing details
	_clear_details();
	
	if (p_item.is_null()) {
		no_selection_label->set_visible(true);
		details_container->set_visible(false);
		return;
	}
	
	no_selection_label->set_visible(false);
	details_container->set_visible(true);
	
	// Create proper editor UI that matches .tscn structure exactly
	
	// ID Editor (HBoxContainer to match CraftStationTypeIDEditor)
	HBoxContainer *id_container = memnew(HBoxContainer);
	details_vbox->add_child(id_container);
	
	Label *id_label = memnew(Label);
	id_container->add_child(id_label);
	id_label->set_text("ID");
	id_label->set_custom_minimum_size(Vector2(160, 0));
	
	LineEdit *id_edit = memnew(LineEdit);
	id_container->add_child(id_edit);
	id_edit->set_text(p_item->get_id());
	id_edit->set_placeholder("Enter unique craft station type ID");
	id_edit->set_custom_minimum_size(Vector2(0, 32));
	id_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	id_edit->connect("text_submitted", callable_mp(this, &CraftStationTypesEditor::_on_id_changed));
	id_edit->connect("focus_exited", callable_mp(this, &CraftStationTypesEditor::_on_id_focus_exited));
	
	// HSeparator to match .tscn
	HSeparator *separator = memnew(HSeparator);
	details_vbox->add_child(separator);
	
	// Name section (HBoxContainer to match .tscn)
	HBoxContainer *name_container = memnew(HBoxContainer);
	details_vbox->add_child(name_container);
	
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
	name_edit->connect("text_submitted", callable_mp(this, &CraftStationTypesEditor::_on_name_changed));
	name_edit->connect("focus_exited", callable_mp(this, &CraftStationTypesEditor::_on_name_focus_exited));
	
	// Icon selector (HBoxContainer to match .tscn)
	HBoxContainer *icon_selector = memnew(HBoxContainer);
	details_vbox->add_child(icon_selector);
	
	Label *icon_label = memnew(Label);
	icon_selector->add_child(icon_label);
	icon_label->set_text("Icon");
	icon_label->set_custom_minimum_size(Vector2(160, 0));
	
	// Icon preview button
	TextureButton *icon_preview = memnew(TextureButton);
	icon_selector->add_child(icon_preview);
	icon_preview->set_custom_minimum_size(Vector2(64, 64));
	icon_preview->set_stretch_mode(TextureButton::STRETCH_KEEP_ASPECT_CENTERED);
	if (p_item->get_icon().is_valid()) {
		icon_preview->set_texture_normal(p_item->get_icon());
	}
	icon_preview->connect("pressed", callable_mp(this, &CraftStationTypesEditor::_on_icon_button_pressed));
	
	VBoxContainer *icon_buttons = memnew(VBoxContainer);
	icon_selector->add_child(icon_buttons);
	
	Button *select_icon_btn = memnew(Button);
	icon_buttons->add_child(select_icon_btn);
	select_icon_btn->set_text("Select Icon");
	select_icon_btn->connect("pressed", callable_mp(this, &CraftStationTypesEditor::_on_icon_button_pressed));
	
	Button *clear_icon_btn = memnew(Button);
	icon_buttons->add_child(clear_icon_btn);
	clear_icon_btn->set_text("Clear Icon");
	clear_icon_btn->connect("pressed", callable_mp(this, &CraftStationTypesEditor::_on_clear_icon_pressed));
}

void CraftStationTypesEditor::_clear_details() {
	// Remove all children from the details VBox
	Array children = details_vbox->get_children();
	for (int i = 0; i < children.size(); i++) {
		Node *child = Object::cast_to<Node>(children[i]);
		if (child) {
			details_vbox->remove_child(child);
			child->queue_free();
		}
	}
}

void CraftStationTypesEditor::_emit_removed(const Ref<CraftStationType> &p_item) {
	emit_signal("removed", p_item);
}

void CraftStationTypesEditor::_emit_duplicated(const Ref<CraftStationType> &p_item) {
	emit_signal("duplicated", p_item);
}

void CraftStationTypesEditor::_on_item_selected(const Variant &p_item, int p_index) {
	Ref<CraftStationType> item = p_item;
	_update_details(item);
}

void CraftStationTypesEditor::_on_item_popup_menu_requested(const Vector2 &p_position) {
	// TODO: Show context menu with options like duplicate, remove, etc.
	print_line("Context menu requested at position: " + p_position);
}

// Property change handlers
void CraftStationTypesEditor::_on_id_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter unique craft station type ID") {
				current_item->set_id(line_edit->get_text());
				break;
			}
		}
	}
}

void CraftStationTypesEditor::_on_id_focus_exited() {
	// Handle focus exit - just trigger the change handler
	_on_id_changed("");
}

void CraftStationTypesEditor::_on_name_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter craft station type name") {
				current_item->set_name(line_edit->get_text());
				break;
			}
		}
	}
}

void CraftStationTypesEditor::_on_name_focus_exited() {
	// Handle focus exit - just trigger the change handler  
	_on_name_changed("");
}

void CraftStationTypesEditor::_on_icon_button_pressed() {
	icon_dialog->popup_centered(Vector2i(800, 600));
}

void CraftStationTypesEditor::_on_clear_icon_pressed() {
	if (current_item.is_valid()) {
		current_item->set_icon(Ref<Texture2D>());
		// Refresh the details to update the icon preview
		_update_details(current_item);
	}
}

void CraftStationTypesEditor::_on_icon_dialog_file_selected(const String &p_path) {
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