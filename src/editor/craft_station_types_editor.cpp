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

#include <godot_cpp/classes/popup_menu.hpp>

using namespace godot;

void CraftStationTypesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_selected", "item", "index"), &CraftStationTypesEditor::_on_inventory_item_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_popup_menu_requested", "at_position"), &CraftStationTypesEditor::_on_inventory_item_list_item_popup_menu_requested);
	ClassDB::bind_method(D_METHOD("_on_items_popup_menu_id_pressed", "id"), &CraftStationTypesEditor::_on_items_popup_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_craft_station_type_editor_changed", "id"), &CraftStationTypesEditor::_on_craft_station_type_editor_changed);
}

void CraftStationTypesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

CraftStationTypesEditor::CraftStationTypesEditor() {
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

CraftStationTypesEditor::~CraftStationTypesEditor() {
}

void CraftStationTypesEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	BaseInventoryEditor::set_editor_plugin(p_editor_plugin);
	if (craft_station_type_editor) {
		craft_station_type_editor->set_editor_plugin(p_editor_plugin);
	}
	_apply_theme();
}

void CraftStationTypesEditor::_create_ui() {
	// HSplitContainer matching addon .tscn exactly
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	hsplit_container->set_offset(SIDE_LEFT, -4);
	hsplit_container->set_offset(SIDE_RIGHT, 4);
	hsplit_container->set_split_offset(-128); // Match addon split offset

	// InventoryItemListEditor (left side) - matches .tscn minimum size
	inventory_item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(inventory_item_list_editor);
	inventory_item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	inventory_item_list_editor->connect("item_selected", callable_mp(this, &CraftStationTypesEditor::_on_inventory_item_list_item_selected));
	inventory_item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &CraftStationTypesEditor::_on_inventory_item_list_item_popup_menu_requested));

	// CraftStationTypeEditor (right side)
	craft_station_type_editor = memnew(CraftStationTypeEditor);
	hsplit_container->add_child(craft_station_type_editor);
	craft_station_type_editor->connect("changed", callable_mp(this, &CraftStationTypesEditor::_on_craft_station_type_editor_changed));

	// PopupMenu for context actions
	items_popup_menu = memnew(PopupMenu);
	add_child(items_popup_menu);
	items_popup_menu->set_size(Vector2i(100, 24));
	items_popup_menu->connect("id_pressed", callable_mp(this, &CraftStationTypesEditor::_on_items_popup_menu_id_pressed));
}

void CraftStationTypesEditor::on_load_database() {
	load_craft_station_types();
}

void CraftStationTypesEditor::load_craft_station_types() {
	if (get_database()) {
		TypedArray<CraftStationType> craft_station_types = get_database()->get_stations_type();
		Array item_array;
		for (int i = 0; i < craft_station_types.size(); i++) {
			item_array.append(craft_station_types[i]);
		}
		inventory_item_list_editor->set_items(item_array);
	} else {
		inventory_item_list_editor->clear_items();
	}
	
	// Reset craft station type editor
	if (craft_station_type_editor) {
		craft_station_type_editor->load_craft_station_type(Ref<CraftStationType>(), get_database());
	}
}

bool CraftStationTypesEditor::remove_current_data() {
	// Call super.remove_current_data() like in addon
	bool removed = BaseInventoryEditor::remove_current_data();
	if (removed) {
		load_craft_station_types();
		emit_signal("data_changed");
		if (craft_station_type_editor && get_database()) {
			craft_station_type_editor->load_craft_station_type(Ref<CraftStationType>(), get_database());
		}
	}
	return removed;
}

void CraftStationTypesEditor::select(const Ref<CraftStationType>& craft_station_type) {
	if (craft_station_type.is_valid()) {
		inventory_item_list_editor->select_item(craft_station_type);
		if (craft_station_type_editor) {
			craft_station_type_editor->load_craft_station_type(craft_station_type, get_database());
		}
	}
}

void CraftStationTypesEditor::_apply_theme() {
	// Apply theme to components
}

void CraftStationTypesEditor::_on_theme_changed() {
	_apply_theme();
}

void CraftStationTypesEditor::_on_inventory_item_list_item_selected(const Variant& item, int index) {
	Ref<CraftStationType> craft_station_type = item;
	set_current_data(craft_station_type);
	if (craft_station_type_editor) {
		craft_station_type_editor->load_craft_station_type(craft_station_type, get_database());
	}
}

void CraftStationTypesEditor::_on_inventory_item_list_item_popup_menu_requested(const Vector2& at_position) {
	if (!items_popup_menu || !get_editor_plugin()) {
		return;
	}
	
	items_popup_menu->clear();

	// Add Remove option with editor icon
	Ref<Texture2D> remove_icon;
	if (get_editor_plugin()->get_editor_interface()) {
		EditorInterface* ei = get_editor_plugin()->get_editor_interface();
		// Note: In production this would get the actual theme icon
		remove_icon = get_theme_icon("Remove", "EditorIcons");
	}
	items_popup_menu->add_icon_item(remove_icon, "Remove", ITEM_REMOVE);
	
	// Add Duplicate option with editor icon
	Ref<Texture2D> duplicate_icon;
	if (get_editor_plugin()->get_editor_interface()) {
		EditorInterface* ei = get_editor_plugin()->get_editor_interface();
		// Note: In production this would get the actual theme icon
		duplicate_icon = get_theme_icon("Duplicate", "EditorIcons");
	}
	items_popup_menu->add_icon_item(duplicate_icon, "Duplicate", ITEM_DUPLICATE);

	// Position popup at mouse position like in addon
	if (inventory_item_list_editor) {
		Vector2 global_pos = inventory_item_list_editor->get_global_mouse_position();
		Variant v = get_viewport()->get("position");
		items_popup_menu->set_position(((Vector2)v) + global_pos);
		items_popup_menu->popup();
	}
}

void CraftStationTypesEditor::_on_items_popup_menu_id_pressed(int id) {
	switch (id) {
		case ITEM_REMOVE: {
			if (get_current_data().is_null()) {
				return;
			}
			
			// Get remove confirmation dialog from base class
			if (remove_confirmation_dialog) {
				remove_confirmation_dialog->popup_centered();
				String item_name = "Unknown";
				Ref<CraftStationType> craft_station_type = get_current_data();
				if (craft_station_type.is_valid()) {
					item_name = craft_station_type->get_name();
				}
				remove_confirmation_dialog->set_text("Remove Craft Station Type \"" + item_name + "\"?");
			}
		} break;
		
		case ITEM_DUPLICATE: {
			// Call super.duplicate_current_data() like in addon
			BaseInventoryEditor::duplicate_current_data();
		} break;
	}
}

void CraftStationTypesEditor::_on_craft_station_type_editor_changed(const Ref<CraftStationType>& craft_station_type) {
	// Update the list when craft station type changes
	if (inventory_item_list_editor) {
		int current_index = inventory_item_list_editor->get_index_of_item_id(craft_station_type->get_id());
		if (current_index >= 0) {
			inventory_item_list_editor->update_item(current_index);
		}
	}
}

#endif // TOOLS_ENABLED