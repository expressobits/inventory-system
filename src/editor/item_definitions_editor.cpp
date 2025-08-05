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

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

using namespace godot;

void ItemDefinitionsEditor::_bind_methods() {
	// Bind signal handlers - matching addon method names exactly
	ClassDB::bind_method(D_METHOD("_on_theme_changed"), &ItemDefinitionsEditor::_on_theme_changed);
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_selected", "item", "index"), &ItemDefinitionsEditor::_on_inventory_item_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_popup_menu_requested", "at_position"), &ItemDefinitionsEditor::_on_inventory_item_list_item_popup_menu_requested);
	ClassDB::bind_method(D_METHOD("_on_items_popup_menu_id_pressed", "id"), &ItemDefinitionsEditor::_on_items_popup_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_editor_changed", "id"), &ItemDefinitionsEditor::_on_item_editor_changed);

	// Public methods
	ClassDB::bind_method(D_METHOD("load_items"), &ItemDefinitionsEditor::load_items);
	ClassDB::bind_method(D_METHOD("select", "item"), &ItemDefinitionsEditor::select);
}

void ItemDefinitionsEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Call super._ready() like in addon by calling parent implementation
			BaseInventoryEditor::_notification(p_what);
			_create_ui();
			_apply_theme();
		} break;
	}
}

ItemDefinitionsEditor::ItemDefinitionsEditor() {
	hsplit_container = nullptr;
	item_editor = nullptr;
	inventory_item_list_editor = nullptr;
	items_popup_menu = nullptr;
}

ItemDefinitionsEditor::~ItemDefinitionsEditor() {
}

void ItemDefinitionsEditor::_create_ui() {
	// Create UI matching .tscn structure exactly: HSplitContainer with two children + PopupMenu
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	hsplit_container->set_offset(SIDE_LEFT, -4);
	hsplit_container->set_offset(SIDE_RIGHT, 4);
	hsplit_container->set_split_offset(-128); // Match addon split offset

	// InventoryItemListEditor (left side)
	inventory_item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(inventory_item_list_editor);
	inventory_item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	inventory_item_list_editor->connect("item_selected", callable_mp(this, &ItemDefinitionsEditor::_on_inventory_item_list_item_selected));
	inventory_item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &ItemDefinitionsEditor::_on_inventory_item_list_item_popup_menu_requested));

	// ItemDefinitionEditor (right side)
	item_editor = memnew(ItemDefinitionEditor);
	hsplit_container->add_child(item_editor);
	item_editor->connect("changed", callable_mp(this, &ItemDefinitionsEditor::_on_item_editor_changed));

	// PopupMenu for context actions
	items_popup_menu = memnew(PopupMenu);
	add_child(items_popup_menu);
	items_popup_menu->set_size(Vector2i(100, 24));
	items_popup_menu->connect("id_pressed", callable_mp(this, &ItemDefinitionsEditor::_on_items_popup_menu_id_pressed));
}

void ItemDefinitionsEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	// Call super.set_editor_plugin() like in addon
	BaseInventoryEditor::set_editor_plugin(p_editor_plugin);
	
	if (item_editor) {
		item_editor->set_editor_plugin(p_editor_plugin);
	}
	_apply_theme();
}

void ItemDefinitionsEditor::on_load_database() {
	// Match addon logic exactly
	if (item_editor && get_database()) {
		item_editor->load_item(Ref<ItemDefinition>(), get_database());
	}
	load_items();
}

void ItemDefinitionsEditor::load_items() {
	if (inventory_item_list_editor && get_database()) {
		TypedArray<ItemDefinition> items = get_database()->get_items();
		Array item_array;
		for (int i = 0; i < items.size(); i++) {
			item_array.append(items[i]);
		}
		inventory_item_list_editor->set_items(item_array);
	}
}

bool ItemDefinitionsEditor::remove_current_data() {
	// Call super.remove_current_data() like in addon
	bool removed = BaseInventoryEditor::remove_current_data();
	if (removed) {
		load_items();
		emit_signal("data_changed");
		if (item_editor && get_database()) {
			item_editor->load_item(Ref<ItemDefinition>(), get_database());
		}
	}
	return removed;
}

void ItemDefinitionsEditor::select(const Ref<ItemDefinition>& item) {
	if (item_editor && get_database()) {
		item_editor->load_item(item, get_database());
	}
	if (inventory_item_list_editor) {
		inventory_item_list_editor->select_item(item);
	}
}

void ItemDefinitionsEditor::_apply_theme() {
	// Call super._apply_theme() like in addon
	BaseInventoryEditor::_apply_theme();
	
	// Apply search icon from editor theme like in addon
	if (get_editor_plugin() && inventory_item_list_editor) {
		// TODO: Apply search icon when InventoryItemListEditor supports it
		// inventory_item_list_editor.search_icon.texture = get_theme_icon("Search", "EditorIcons")
	}
}

void ItemDefinitionsEditor::_on_theme_changed() {
	_apply_theme();
}

void ItemDefinitionsEditor::_on_inventory_item_list_item_selected(const Variant& item, int index) {
	Ref<ItemDefinition> item_def = item;
	set_current_data(item_def);
	if (item_editor && get_database()) {
		item_editor->load_item(item_def, get_database());
	}
}

void ItemDefinitionsEditor::_on_inventory_item_list_item_popup_menu_requested(const Vector2& at_position) {
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

void ItemDefinitionsEditor::_on_items_popup_menu_id_pressed(int id) {
	switch (id) {
		case ITEM_REMOVE: {
			if (get_current_data().is_null()) {
				return;
			}
			
			// Get remove confirmation dialog from base class
			if (remove_confirmation_dialog) {
				remove_confirmation_dialog->popup_centered();
				String item_name = "Unknown";
				Ref<ItemDefinition> item_def = get_current_data();
				if (item_def.is_valid()) {
					item_name = item_def->get_name();
				}
				remove_confirmation_dialog->set_text("Remove Item Definition \"" + item_name + "\"?");
			}
		} break;
		
		case ITEM_DUPLICATE: {
			// Call super.duplicate_current_data() like in addon
			BaseInventoryEditor::duplicate_current_data();
		} break;
	}
}

void ItemDefinitionsEditor::_on_item_editor_changed(const Ref<ItemDefinition>& item_definition) {
	if (inventory_item_list_editor) {
		int index = inventory_item_list_editor->get_index_of_item_id(item_definition->get_id());
		if (index > -1) {
			inventory_item_list_editor->update_item(index);
			emit_signal("data_changed");
		}
	}
}

#endif // TOOLS_ENABLED