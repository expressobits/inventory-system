/**
 * item_categories_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "item_categories_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_category.h"

#include <godot_cpp/classes/editor_inspector.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/color_picker.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/h_separator.hpp>

using namespace godot;

void ItemCategoriesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_item_selected", "item", "index"), &ItemCategoriesEditor::_on_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_popup_menu_requested", "at_position"), &ItemCategoriesEditor::_on_item_popup_menu_requested);
	
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_id_changed", "text"), &ItemCategoriesEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_id_focus_exited"), &ItemCategoriesEditor::_on_id_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_name_changed", "text"), &ItemCategoriesEditor::_on_name_changed);
	ClassDB::bind_method(D_METHOD("_on_name_focus_exited"), &ItemCategoriesEditor::_on_name_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_color_changed", "color"), &ItemCategoriesEditor::_on_color_changed);
	ClassDB::bind_method(D_METHOD("_on_code_changed", "value"), &ItemCategoriesEditor::_on_code_changed);
	ClassDB::bind_method(D_METHOD("_on_icon_button_pressed"), &ItemCategoriesEditor::_on_icon_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_clear_icon_pressed"), &ItemCategoriesEditor::_on_clear_icon_pressed);
	ClassDB::bind_method(D_METHOD("_on_icon_dialog_file_selected", "path"), &ItemCategoriesEditor::_on_icon_dialog_file_selected);

	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")));
	ADD_SIGNAL(MethodInfo("duplicated", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")));
}

void ItemCategoriesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

ItemCategoriesEditor::ItemCategoriesEditor() {
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
	icon_dialog->connect("file_selected", callable_mp(this, &ItemCategoriesEditor::_on_icon_dialog_file_selected));
}

ItemCategoriesEditor::~ItemCategoriesEditor() {
}

void ItemCategoriesEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void ItemCategoriesEditor::_create_ui() {
	// Split container
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	// Item list editor
	item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(item_list_editor);
	item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	item_list_editor->connect("item_selected", callable_mp(this, &ItemCategoriesEditor::_on_item_selected));
	item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &ItemCategoriesEditor::_on_item_popup_menu_requested));

	// Details panel
	details_scroll = memnew(ScrollContainer);
	hsplit_container->add_child(details_scroll);
	details_scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	details_container = memnew(VBoxContainer);
	details_scroll->add_child(details_container);
	details_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// No selection label
	no_selection_label = memnew(Label);
	details_container->add_child(no_selection_label);
	no_selection_label->set_text("Select an item category to edit its properties");
	no_selection_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	no_selection_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	no_selection_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	no_selection_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void ItemCategoriesEditor::load_from_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		TypedArray<ItemCategory> items = database->get_categories();
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

void ItemCategoriesEditor::select_item(const Ref<ItemCategory> &p_item) {
	if (p_item.is_valid()) {
		item_list_editor->select_item(p_item);
		_update_details(p_item);
	}
}

void ItemCategoriesEditor::_update_details(const Ref<ItemCategory> &p_item) {
	current_item = p_item;
	
	// Clear existing details
	_clear_details();
	
	if (p_item.is_null()) {
		no_selection_label->set_visible(true);
		return;
	}
	
	no_selection_label->set_visible(false);
	
	// Create proper editor UI components
	
	// ID field
	Label *id_label = memnew(Label);
	details_container->add_child(id_label);
	id_label->set_text("ID:");
	
	LineEdit *id_edit = memnew(LineEdit);
	details_container->add_child(id_edit);
	id_edit->set_text(p_item->get_id());
	id_edit->set_placeholder("Enter unique item category ID");
	id_edit->connect("text_submitted", callable_mp(this, &ItemCategoriesEditor::_on_id_changed));
	id_edit->connect("focus_exited", callable_mp(this, &ItemCategoriesEditor::_on_id_focus_exited));
	
	// Name field
	Label *name_label = memnew(Label);
	details_container->add_child(name_label);
	name_label->set_text("Name:");
	
	LineEdit *name_edit = memnew(LineEdit);
	details_container->add_child(name_edit);
	name_edit->set_text(p_item->get_name());
	name_edit->set_placeholder("Enter item category name");
	name_edit->connect("text_submitted", callable_mp(this, &ItemCategoriesEditor::_on_name_changed));
	name_edit->connect("focus_exited", callable_mp(this, &ItemCategoriesEditor::_on_name_focus_exited));
	
	// Category properties
	HSeparator *separator1 = memnew(HSeparator);
	details_container->add_child(separator1);
	
	Label *properties_label = memnew(Label);
	details_container->add_child(properties_label);
	properties_label->set_text("Category Properties:");
	properties_label->add_theme_font_size_override("font_size", 14);
	
	// Color field
	Label *color_label = memnew(Label);
	details_container->add_child(color_label);
	color_label->set_text("Color:");
	
	ColorPicker *color_picker = memnew(ColorPicker);
	details_container->add_child(color_picker);
	color_picker->set_color(p_item->get_color());
	color_picker->set_custom_minimum_size(Vector2(200, 150));
	color_picker->connect("color_changed", callable_mp(this, &ItemCategoriesEditor::_on_color_changed));
	
	// Code field
	Label *code_label = memnew(Label);
	details_container->add_child(code_label);
	code_label->set_text("Code:");
	
	SpinBox *code_spin = memnew(SpinBox);
	details_container->add_child(code_spin);
	code_spin->set_min(0);
	code_spin->set_max(999999);
	code_spin->set_value(p_item->get_code());
	code_spin->connect("value_changed", callable_mp(this, &ItemCategoriesEditor::_on_code_changed));
	
	// Visual properties
	HSeparator *separator2 = memnew(HSeparator);
	details_container->add_child(separator2);
	
	Label *visual_label = memnew(Label);
	details_container->add_child(visual_label);
	visual_label->set_text("Visual Properties:");
	visual_label->add_theme_font_size_override("font_size", 14);
	
	Label *icon_label = memnew(Label);
	details_container->add_child(icon_label);
	icon_label->set_text("Icon:");
	
	HBoxContainer *icon_container = memnew(HBoxContainer);
	details_container->add_child(icon_container);
	
	TextureButton *icon_preview = memnew(TextureButton);
	icon_container->add_child(icon_preview);
	icon_preview->set_custom_minimum_size(Vector2(64, 64));
	icon_preview->set_expand_mode(TextureButton::EXPAND_FIT_WIDTH_PROPORTIONAL);
	icon_preview->set_stretch_mode(TextureButton::STRETCH_KEEP_ASPECT_CENTERED);
	if (p_item->get_icon().is_valid()) {
		icon_preview->set_texture_normal(p_item->get_icon());
	}
	icon_preview->connect("pressed", callable_mp(this, &ItemCategoriesEditor::_on_icon_button_pressed));
	
	VBoxContainer *icon_buttons = memnew(VBoxContainer);
	icon_container->add_child(icon_buttons);
	
	Button *select_icon_btn = memnew(Button);
	icon_buttons->add_child(select_icon_btn);
	select_icon_btn->set_text("Select Icon");
	select_icon_btn->connect("pressed", callable_mp(this, &ItemCategoriesEditor::_on_icon_button_pressed));
	
	Button *clear_icon_btn = memnew(Button);
	icon_buttons->add_child(clear_icon_btn);
	clear_icon_btn->set_text("Clear Icon");
	clear_icon_btn->connect("pressed", callable_mp(this, &ItemCategoriesEditor::_on_clear_icon_pressed));
}

void ItemCategoriesEditor::_clear_details() {
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

void ItemCategoriesEditor::_emit_removed(const Ref<ItemCategory> &p_item) {
	emit_signal("removed", p_item);
}

void ItemCategoriesEditor::_emit_duplicated(const Ref<ItemCategory> &p_item) {
	emit_signal("duplicated", p_item);
}

void ItemCategoriesEditor::_on_item_selected(const Variant &p_item, int p_index) {
	Ref<ItemCategory> item = p_item;
	_update_details(item);
}

void ItemCategoriesEditor::_on_item_popup_menu_requested(const Vector2 &p_position) {
	// TODO: Show context menu with options like duplicate, remove, etc.
	print_line("Context menu requested at position: " + p_position);
}

// Property change handlers
void ItemCategoriesEditor::_on_id_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter unique item category ID") {
				current_item->set_id(line_edit->get_text());
				break;
			}
		}
	}
}

void ItemCategoriesEditor::_on_id_focus_exited() {
	// Handle focus exit - just trigger the change handler
	_on_id_changed("");
}

void ItemCategoriesEditor::_on_name_changed(const String &p_text) {
	if (current_item.is_valid()) {
		// Get the LineEdit widget from the current details container
		Array children = details_container->get_children();
		for (int i = 0; i < children.size(); i++) {
			LineEdit *line_edit = Object::cast_to<LineEdit>(children[i]);
			if (line_edit && line_edit->get_placeholder() == "Enter item category name") {
				current_item->set_name(line_edit->get_text());
				break;
			}
		}
	}
}

void ItemCategoriesEditor::_on_name_focus_exited() {
	// Handle focus exit - just trigger the change handler  
	_on_name_changed("");
}

void ItemCategoriesEditor::_on_color_changed(const Color &p_color) {
	if (current_item.is_valid()) {
		current_item->set_color(p_color);
	}
}

void ItemCategoriesEditor::_on_code_changed(double p_value) {
	if (current_item.is_valid()) {
		current_item->set_code((int)p_value);
	}
}

void ItemCategoriesEditor::_on_icon_button_pressed() {
	icon_dialog->popup_centered(Vector2i(800, 600));
}

void ItemCategoriesEditor::_on_clear_icon_pressed() {
	if (current_item.is_valid()) {
		current_item->set_icon(Ref<Texture2D>());
		// Refresh the details to update the icon preview
		_update_details(current_item);
	}
}

void ItemCategoriesEditor::_on_icon_dialog_file_selected(const String &p_path) {
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