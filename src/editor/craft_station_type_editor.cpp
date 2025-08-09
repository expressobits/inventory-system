/**
 * craft_station_type_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "craft_station_type_editor.h"

#include "../base/inventory_database.h"
#include "../base/craft_station_type.h"

#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

void CraftStationTypeEditor::_bind_methods() {
	// Property change handlers
	ClassDB::bind_method(D_METHOD("_on_resource_id_editor_changed", "id"), &CraftStationTypeEditor::_on_resource_id_editor_changed);
	ClassDB::bind_method(D_METHOD("_on_name_changed", "text"), &CraftStationTypeEditor::_on_name_changed);
	ClassDB::bind_method(D_METHOD("_on_name_focus_exited"), &CraftStationTypeEditor::_on_name_focus_exited);
	ClassDB::bind_method(D_METHOD("_on_icon_changed", "icon"), &CraftStationTypeEditor::_on_icon_changed);

	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "craft_station_type", PROPERTY_HINT_RESOURCE_TYPE, "CraftStationType")));
}

void CraftStationTypeEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

CraftStationTypeEditor::CraftStationTypeEditor() {
	database = nullptr;
	editor_plugin = nullptr;
	resource_id_editor = nullptr;
	icon_selector = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

CraftStationTypeEditor::~CraftStationTypeEditor() {
}

void CraftStationTypeEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	editor_plugin = p_editor_plugin;
	if (icon_selector) {
		icon_selector->set_editor_plugin(editor_plugin);
	}
}

void CraftStationTypeEditor::_create_ui() {
	// Main scroll container
	scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	scroll_container->set_horizontal_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
	scroll_container->set_visible(false);

	// Main VBox container
	main_vbox = memnew(VBoxContainer);
	main_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	scroll_container->add_child(main_vbox);

	// Resource ID - matches addon structure
	resource_id_editor = memnew(ResourceIDEditor);
	main_vbox->add_child(resource_id_editor);
	resource_id_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	resource_id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_CRAFT_STATION_TYPE);
	
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
	name_edit->set_placeholder("Place Item Name Here");
	name_edit->set_custom_minimum_size(Vector2(0, 32));
	name_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// Icon selector
	icon_selector = memnew(IconSelector);
	main_vbox->add_child(icon_selector);
	if (editor_plugin) {
		icon_selector->set_editor_plugin(editor_plugin);
	}
	
	_connect_signals();
}

void CraftStationTypeEditor::_connect_signals() {
	if (resource_id_editor) {
		resource_id_editor->connect("changed", callable_mp(this, &CraftStationTypeEditor::_on_resource_id_editor_changed));
	}
	if (name_edit) {
		name_edit->connect("text_submitted", callable_mp(this, &CraftStationTypeEditor::_on_name_changed));
		name_edit->connect("focus_exited", callable_mp(this, &CraftStationTypeEditor::_on_name_focus_exited));
	}
	if (icon_selector) {
		icon_selector->connect("icon_changed", callable_mp(this, &CraftStationTypeEditor::_on_icon_changed));
	}
}

void CraftStationTypeEditor::_disconnect_signals() {
	if (resource_id_editor && resource_id_editor->is_connected("changed", callable_mp(this, &CraftStationTypeEditor::_on_resource_id_editor_changed))) {
		resource_id_editor->disconnect("changed", callable_mp(this, &CraftStationTypeEditor::_on_resource_id_editor_changed));
	}
	if (name_edit && name_edit->is_connected("text_submitted", callable_mp(this, &CraftStationTypeEditor::_on_name_changed))) {
		name_edit->disconnect("text_submitted", callable_mp(this, &CraftStationTypeEditor::_on_name_changed));
		name_edit->disconnect("focus_exited", callable_mp(this, &CraftStationTypeEditor::_on_name_focus_exited));
	}
	if (icon_selector && icon_selector->is_connected("icon_changed", callable_mp(this, &CraftStationTypeEditor::_on_icon_changed))) {
		icon_selector->disconnect("icon_changed", callable_mp(this, &CraftStationTypeEditor::_on_icon_changed));
	}
}

void CraftStationTypeEditor::load_craft_station_type(const Ref<CraftStationType>& p_craft_station_type, InventoryDatabase* p_database) {
	_disconnect_signals();
	
	craft_station_type = p_craft_station_type;
	database = p_database;
	
	if (craft_station_type.is_valid()) {
		scroll_container->set_visible(true);

		// Setup resource_id_editor
		if (resource_id_editor && database) {
			resource_id_editor->setup(database, craft_station_type->get_id());
		}
		if (name_edit) {
			name_edit->set_text(craft_station_type->get_name());
		}
		if (icon_selector) {
			icon_selector->load_icon(craft_station_type->get_icon());
		}
	} else {
		scroll_container->set_visible(false);
	}
	
	_connect_signals();
}

void CraftStationTypeEditor::reload_craft_station_type() {
	load_craft_station_type(craft_station_type, database);
}

void CraftStationTypeEditor::_on_resource_id_editor_changed(const String& id) {
	if (craft_station_type.is_valid()) {
		craft_station_type->set_id(id);
		emit_signal("changed", craft_station_type);
	}
}

void CraftStationTypeEditor::_on_name_changed(const String& text) {
	if (craft_station_type.is_valid()) {
		craft_station_type->set_name(text);
	}
}

void CraftStationTypeEditor::_on_name_focus_exited() {
	if (craft_station_type.is_valid() && name_edit) {
		_on_name_changed(name_edit->get_text());
	}
}

void CraftStationTypeEditor::_on_icon_changed(const Ref<Texture2D>& icon) {
	if (craft_station_type.is_valid()) {
		craft_station_type->set_icon(icon);
	}
}

#endif // TOOLS_ENABLED