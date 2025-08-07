/**
 * loots_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "loots_editor.h"

#include "../base/inventory_database.h"
#include "../base/loot.h"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

using namespace godot;

void LootsEditor::_bind_methods() {
	// Bind signal handlers - matching addon method names exactly
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_selected", "item", "index"), &LootsEditor::_on_inventory_item_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_inventory_item_list_item_popup_menu_requested", "at_position"), &LootsEditor::_on_inventory_item_list_item_popup_menu_requested);
	ClassDB::bind_method(D_METHOD("_on_items_popup_menu_id_pressed", "id"), &LootsEditor::_on_items_popup_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_loot_editor_changed", "loot"), &LootsEditor::_on_loot_editor_changed);

	// Public methods
	ClassDB::bind_method(D_METHOD("load_loots"), &LootsEditor::load_loots);
	ClassDB::bind_method(D_METHOD("select", "loot"), &LootsEditor::select);
}

void LootsEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Call super._ready() like in addon by calling parent implementation
			BaseInventoryEditor::_notification(p_what);
			_create_ui();
			_apply_theme();
		} break;
	}
}

LootsEditor::LootsEditor() {
	hsplit_container = nullptr;
	loot_editor = nullptr;
	inventory_item_list_editor = nullptr;
	items_popup_menu = nullptr;
}

LootsEditor::~LootsEditor() {
}

void LootsEditor::_create_ui() {
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
	inventory_item_list_editor->connect("item_selected", callable_mp(this, &LootsEditor::_on_inventory_item_list_item_selected));
	inventory_item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &LootsEditor::_on_inventory_item_list_item_popup_menu_requested));

	// LootEditor (right side)
	loot_editor = memnew(LootEditor);
	hsplit_container->add_child(loot_editor);
	loot_editor->connect("changed", callable_mp(this, &LootsEditor::_on_loot_editor_changed));

	// PopupMenu for context actions
	items_popup_menu = memnew(PopupMenu);
	add_child(items_popup_menu);
	items_popup_menu->set_size(Vector2i(100, 24));
	items_popup_menu->connect("id_pressed", callable_mp(this, &LootsEditor::_on_items_popup_menu_id_pressed));
}

void LootsEditor::set_editor_plugin(EditorPlugin* p_editor_plugin) {
	BaseInventoryEditor::set_editor_plugin(p_editor_plugin);
	
	if (loot_editor) {
		loot_editor->set_editor_plugin(p_editor_plugin);
	}
	_apply_theme();
}

void LootsEditor::on_load_database() {
	if (loot_editor && get_database()) {
		loot_editor->load_loot(Ref<Loot>(), get_database());
	}
	if (inventory_item_list_editor) {
		load_loots();
	}
}

void LootsEditor::load_loots() {
	if (inventory_item_list_editor && get_database()) {
		TypedArray<Loot> loots = get_database()->get_loots();
		Array loot_array;
		for (int i = 0; i < loots.size(); i++) {
			loot_array.append(loots[i]);
		}
		inventory_item_list_editor->set_items(loot_array);
	}
}

bool LootsEditor::remove_current_data() {
	bool removed = BaseInventoryEditor::remove_current_data();
	if (removed) {
		load_loots();
		emit_signal("data_changed");
		if (loot_editor && get_database()) {
			loot_editor->load_loot(Ref<Loot>(), get_database());
		}
	}
	return removed;
}

void LootsEditor::select(const Ref<Loot>& loot) {
	if (loot_editor && get_database()) {
		loot_editor->load_loot(loot, get_database());
	}
	if (inventory_item_list_editor) {
		inventory_item_list_editor->select_item(loot);
	}
}

void LootsEditor::_on_inventory_item_list_item_selected(const Variant& item, int index) {
	Ref<Loot> loot = item;
	set_current_data(loot);
	if (loot_editor && get_database()) {
		loot_editor->load_loot(loot, get_database());
	}
}

void LootsEditor::_on_inventory_item_list_item_popup_menu_requested(const Vector2& at_position) {
	if (!items_popup_menu || !get_editor_plugin()) {
		return;
	}
	
	items_popup_menu->clear();

	Ref<Texture2D> remove_icon;
	if (get_editor_plugin()->get_editor_interface()) {
		EditorInterface* ei = get_editor_plugin()->get_editor_interface();
		remove_icon = get_theme_icon("Remove", "EditorIcons");
	}
	items_popup_menu->add_icon_item(remove_icon, "Remove", ITEM_REMOVE);
	
	Ref<Texture2D> duplicate_icon;
	if (get_editor_plugin()->get_editor_interface()) {
		EditorInterface* ei = get_editor_plugin()->get_editor_interface();
		duplicate_icon = get_theme_icon("Duplicate", "EditorIcons");
	}
	items_popup_menu->add_icon_item(duplicate_icon, "Duplicate", ITEM_DUPLICATE);

	if (inventory_item_list_editor) {
		Vector2 global_pos = inventory_item_list_editor->get_global_mouse_position();
		Variant v = get_viewport()->get("position");
		items_popup_menu->set_position(((Vector2)v) + global_pos);
		items_popup_menu->popup();
	}
}

void LootsEditor::_on_items_popup_menu_id_pressed(int id) {
	switch (id) {
		case ITEM_REMOVE: {
			if (get_current_data().is_null()) {
				return;
			}
			
			// Get remove confirmation dialog from base class
			if (remove_confirmation_dialog) {
				remove_confirmation_dialog->popup_centered();
				String loot_name = "Unknown";
				Ref<Loot> loot = get_current_data();
				if (loot.is_valid()) {
					loot_name = loot->get_name();
				}
				remove_confirmation_dialog->set_text("Remove Loot \"" + loot_name + "\"?");
			}
		} break;
		
		case ITEM_DUPLICATE: {
			BaseInventoryEditor::duplicate_current_data();
		} break;
	}
}

void LootsEditor::_on_loot_editor_changed(const Ref<Loot>& loot) {
	if (inventory_item_list_editor) {
		int index = inventory_item_list_editor->get_index_of_item_id(loot->get_id());
		if (index > -1) {
			inventory_item_list_editor->update_item(index);
			emit_signal("data_changed");
		}
	}
}

#endif // TOOLS_ENABLED