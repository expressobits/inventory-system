/**
 * item_category_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "item_category_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_category.h"
#include "custom_properties_editor.h"

#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

void ItemCategoryEditor::_bind_methods() {
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_resource_id_editor_changed", "id"), &ItemCategoryEditor::_on_resource_id_editor_changed);
	ClassDB::bind_method(D_METHOD("_on_name_changed", "text"), &ItemCategoryEditor::_on_name_changed);
	ClassDB::bind_method(D_METHOD("_on_name_focus_exited"), &ItemCategoryEditor::_on_name_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_color_changed", "color"), &ItemCategoryEditor::_on_color_changed);
	ClassDB::bind_method(D_METHOD("_on_code_changed", "value"), &ItemCategoryEditor::_on_code_changed);
	ClassDB::bind_method(D_METHOD("_on_icon_changed", "icon"), &ItemCategoryEditor::_on_icon_changed);
	ClassDB::bind_method(D_METHOD("_on_custom_properties_changed"), &ItemCategoryEditor::_on_custom_properties_changed);

	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "item_category", PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")));
}

ItemCategoryEditor::ItemCategoryEditor() : BaseResourceEditor() {
	database = nullptr;
	editor_plugin = nullptr;
	resource_id_editor = nullptr;
	icon_selector = nullptr;
	color_picker = nullptr;
	code_spin_box = nullptr;
	custom_properties = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

ItemCategoryEditor::~ItemCategoryEditor() {
}

void ItemCategoryEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	editor_plugin = p_editor_plugin;
	if (icon_selector) {
		icon_selector->set_editor_plugin(editor_plugin);
	}
}

void ItemCategoryEditor::_create_ui() {
	// Call base class to create ScrollContainer and main_vbox
	BaseResourceEditor::_create_ui();
	
	// Apply specific settings for item category editor
	scroll_container->set_horizontal_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
	scroll_container->set_visible(false);

	// Resource ID - matches addon structure
	resource_id_editor = memnew(ResourceIDEditor);
	main_vbox->add_child(resource_id_editor);
	resource_id_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	resource_id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_ITEM_CATEGORY);
	
	// HSeparator to match addon
	HSeparator *separator = memnew(HSeparator);
	main_vbox->add_child(separator);
	
	// Name section (HBoxContainer to match addon)
	HBoxContainer *name_container = memnew(HBoxContainer);
	main_vbox->add_child(name_container);
	
	Label *name_label = memnew(Label);
	name_container->add_child(name_label);
	name_label->set_text("Name");
	name_label->set_custom_minimum_size(Vector2(160, 0));
	
	name_edit = memnew(LineEdit);
	name_container->add_child(name_edit);
	name_edit->set_placeholder("Enter category name");
	name_edit->set_custom_minimum_size(Vector2(0, 32));
	name_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// Color section (HBoxContainer to match addon)
	HBoxContainer *color_container = memnew(HBoxContainer);
	main_vbox->add_child(color_container);
	
	Label *color_label = memnew(Label);
	color_container->add_child(color_label);
	color_label->set_text("Color");
	color_label->set_custom_minimum_size(Vector2(160, 0));
	
	color_picker = memnew(ColorPickerButton);
	color_container->add_child(color_picker);

	color_picker->set_custom_minimum_size(Vector2(100, 32));
	// Code section (HBoxContainer to match addon)
	HBoxContainer *code_container = memnew(HBoxContainer);
	main_vbox->add_child(code_container);
	color_picker->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// Label *code_label = memnew(Label);
	// code_container->add_child(code_label);
	// code_label->set_text("Code");
	// code_label->set_custom_minimum_size(Vector2(160, 0));
	
	// code_spin_box = memnew(SpinBox);
	// code_container->add_child(code_spin_box);
	// code_spin_box->set_min(0);
	// code_spin_box->set_max(999999);
	// code_spin_box->set_step(1);
	// code_spin_box->set_value(0);
	// code_spin_box->set_custom_minimum_size(Vector2(100, 32));
	
	// Icon selector
	icon_selector = memnew(IconSelector);
	main_vbox->add_child(icon_selector);
	if (editor_plugin) {
		icon_selector->set_editor_plugin(editor_plugin);
	}
	
	// HSeparator before custom properties
	HSeparator *custom_props_separator = memnew(HSeparator);
	main_vbox->add_child(custom_props_separator);
	
	// Custom Properties section
	custom_properties = memnew(CustomPropertiesEditor);
	custom_properties->set_resource_type(CustomPropertiesEditor::RESOURCE_TYPE_ITEM_CATEGORY);
	custom_properties->set_title("Item Properties");
	main_vbox->add_child(custom_properties);
	
	_connect_signals();
}

void ItemCategoryEditor::_connect_signals() {
	if (resource_id_editor) {
		resource_id_editor->connect("changed", callable_mp(this, &ItemCategoryEditor::_on_resource_id_editor_changed));
	}
	if (name_edit) {
		name_edit->connect("text_submitted", callable_mp(this, &ItemCategoryEditor::_on_name_changed));
		name_edit->connect("focus_exited", callable_mp(this, &ItemCategoryEditor::_on_name_focus_exited));
	}
	if (color_picker) {
		color_picker->connect("color_changed", callable_mp(this, &ItemCategoryEditor::_on_color_changed));
	}
	if (code_spin_box) {
		code_spin_box->connect("value_changed", callable_mp(this, &ItemCategoryEditor::_on_code_changed));
	}
	if (icon_selector) {
		icon_selector->connect("icon_changed", callable_mp(this, &ItemCategoryEditor::_on_icon_changed));
	}
	if (custom_properties) {
		custom_properties->connect("changed", callable_mp(this, &ItemCategoryEditor::_on_custom_properties_changed));
	}
}

void ItemCategoryEditor::_disconnect_signals() {
	if (resource_id_editor && resource_id_editor->is_connected("changed", callable_mp(this, &ItemCategoryEditor::_on_resource_id_editor_changed))) {
		resource_id_editor->disconnect("changed", callable_mp(this, &ItemCategoryEditor::_on_resource_id_editor_changed));
	}
	if (name_edit && name_edit->is_connected("text_submitted", callable_mp(this, &ItemCategoryEditor::_on_name_changed))) {
		name_edit->disconnect("text_submitted", callable_mp(this, &ItemCategoryEditor::_on_name_changed));
		name_edit->disconnect("focus_exited", callable_mp(this, &ItemCategoryEditor::_on_name_focus_exited));
	}
	if (color_picker && color_picker->is_connected("color_changed", callable_mp(this, &ItemCategoryEditor::_on_color_changed))) {
		color_picker->disconnect("color_changed", callable_mp(this, &ItemCategoryEditor::_on_color_changed));
	}
	if (code_spin_box && code_spin_box->is_connected("value_changed", callable_mp(this, &ItemCategoryEditor::_on_code_changed))) {
		code_spin_box->disconnect("value_changed", callable_mp(this, &ItemCategoryEditor::_on_code_changed));
	}
	if (icon_selector && icon_selector->is_connected("icon_changed", callable_mp(this, &ItemCategoryEditor::_on_icon_changed))) {
		icon_selector->disconnect("icon_changed", callable_mp(this, &ItemCategoryEditor::_on_icon_changed));
	}
	if (custom_properties && custom_properties->is_connected("changed", callable_mp(this, &ItemCategoryEditor::_on_custom_properties_changed))) {
		custom_properties->disconnect("changed", callable_mp(this, &ItemCategoryEditor::_on_custom_properties_changed));
	}
}

void ItemCategoryEditor::load_item_category(const Ref<ItemCategory>& p_item_category, InventoryDatabase* p_database) {
	_disconnect_signals();
	
	item_category = p_item_category;
	database = p_database;
	
	if (item_category.is_valid()) {
		scroll_container->set_visible(true);
		
		// Setup resource_id_editor
		if (resource_id_editor && database) {
			resource_id_editor->setup(database, item_category->get_id());
		}
		if (name_edit) {
			name_edit->set_text(item_category->get_name());
		}
		if (color_picker) {
			color_picker->set_pick_color(item_category->get_color());
		}
		if (code_spin_box) {
			code_spin_box->set_value(item_category->get_code());
		}
		if (icon_selector) {
			icon_selector->load_icon(item_category->get_icon());
		}
		if (custom_properties) {
			custom_properties->load_resource(database, item_category);
		}
	} else {
		scroll_container->set_visible(false);
	}
	
	_connect_signals();
}

void ItemCategoryEditor::reload_item_category() {
	load_item_category(item_category, database);
}

void ItemCategoryEditor::_on_resource_id_editor_changed(const String& id) {
	if (item_category.is_valid()) {
		item_category->set_id(id);
		emit_signal("changed", item_category);
	}
}

void ItemCategoryEditor::_on_name_changed(const String& text) {
	if (item_category.is_valid()) {
		item_category->set_name(text);
		emit_signal("changed", item_category);
	}
}

void ItemCategoryEditor::_on_name_focus_exited() {
	if (item_category.is_valid() && name_edit) {
		_on_name_changed(name_edit->get_text());
	}
}

void ItemCategoryEditor::_on_color_changed(const Color& color) {
	if (item_category.is_valid()) {
		item_category->set_color(color);
		emit_signal("changed", item_category);
	}
}

void ItemCategoryEditor::_on_code_changed(double value) {
	if (item_category.is_valid()) {
		item_category->set_code((int)value);
		emit_signal("changed", item_category);
	}
}

void ItemCategoryEditor::_on_icon_changed(const Ref<Texture2D>& icon) {
	if (item_category.is_valid()) {
		item_category->set_icon(icon);
		emit_signal("changed", item_category);
	}
}

void ItemCategoryEditor::_on_custom_properties_changed() {
	if (item_category.is_valid()) {
		emit_signal("changed", item_category);
	}
}

#endif // TOOLS_ENABLED