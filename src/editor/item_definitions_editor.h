/**
 * item_definitions_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef ITEM_DEFINITIONS_EDITOR_H
#define ITEM_DEFINITIONS_EDITOR_H

#include "inventory_item_list_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>

using namespace godot;

class InventoryDatabase;
class ItemDefinition;

class ItemDefinitionsEditor : public Control {
	GDCLASS(ItemDefinitionsEditor, Control);

private:
	EditorPlugin *editor_plugin;
	Ref<InventoryDatabase> database;

	// UI Components
	HSplitContainer *hsplit_container;
	InventoryItemListEditor *item_list_editor;
	ScrollContainer *details_scroll;
	VBoxContainer *details_container;
	Label *no_selection_label;

	Ref<ItemDefinition> current_item;
	FileDialog *icon_dialog;

	void _create_ui();
	void _update_details(const Ref<ItemDefinition> &p_item);
	void _clear_details();

	void _on_item_selected(const Variant &p_item, int p_index);
	void _on_item_popup_menu_requested(const Vector2 &p_position);
	
	// Property change handlers
	void _on_id_changed(const String &p_text);
	void _on_id_focus_exited();
	void _on_name_changed(const String &p_text);
	void _on_name_focus_exited();
	void _on_description_changed();
	void _on_can_stack_changed(bool p_pressed);
	void _on_max_stack_changed(double p_value);
	void _on_weight_changed(double p_value);
	void _on_size_width_changed(double p_value);
	void _on_size_height_changed(double p_value);
	void _on_icon_button_pressed();
	void _on_clear_icon_pressed();
	void _on_icon_dialog_file_selected(const String &p_path);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void load_from_database(const Ref<InventoryDatabase> &p_database);
	void select_item(const Ref<ItemDefinition> &p_item);

	// Signal emissions
	void _emit_removed(const Ref<ItemDefinition> &p_item);
	void _emit_duplicated(const Ref<ItemDefinition> &p_item);

	ItemDefinitionsEditor();
	~ItemDefinitionsEditor();
};

#endif // ITEM_DEFINITIONS_EDITOR_H

#endif // TOOLS_ENABLED