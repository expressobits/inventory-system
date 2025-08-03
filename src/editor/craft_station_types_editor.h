/**
 * craft_station_types_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef CRAFT_STATION_TYPES_EDITOR_H
#define CRAFT_STATION_TYPES_EDITOR_H

#include "inventory_item_list_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>

using namespace godot;

class InventoryDatabase;
class CraftStationType;

class CraftStationTypesEditor : public Control {
	GDCLASS(CraftStationTypesEditor, Control);

private:
	EditorPlugin *editor_plugin;
	Ref<InventoryDatabase> database;

	// UI Components
	HSplitContainer *hsplit_container;
	InventoryItemListEditor *item_list_editor;
	MarginContainer *details_container;
	VBoxContainer *details_vbox;
	Label *no_selection_label;

	Ref<CraftStationType> current_item;
	FileDialog *icon_dialog;

	void _create_ui();
	void _update_details(const Ref<CraftStationType> &p_item);
	void _clear_details();

	void _on_item_selected(const Variant &p_item, int p_index);
	void _on_item_popup_menu_requested(const Vector2 &p_position);
	
	// Property change handlers
	void _on_id_changed(const String &p_text);
	void _on_id_focus_exited();
	void _on_name_changed(const String &p_text);
	void _on_name_focus_exited();
	void _on_icon_button_pressed();
	void _on_clear_icon_pressed();
	void _on_icon_dialog_file_selected(const String &p_path);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void load_from_database(const Ref<InventoryDatabase> &p_database);
	void select_item(const Ref<CraftStationType> &p_item);

	// Signal emissions
	void _emit_removed(const Ref<CraftStationType> &p_item);
	void _emit_duplicated(const Ref<CraftStationType> &p_item);

	CraftStationTypesEditor();
	~CraftStationTypesEditor();
};

#endif // CRAFT_STATION_TYPES_EDITOR_H

#endif // TOOLS_ENABLED