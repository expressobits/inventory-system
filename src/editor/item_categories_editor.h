/**
 * item_categories_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef ITEM_CATEGORIES_EDITOR_H
#define ITEM_CATEGORIES_EDITOR_H

#include "inventory_item_list_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/color_picker.hpp>
#include <godot_cpp/classes/spin_box.hpp>

using namespace godot;

class InventoryDatabase;
class ItemCategory;

class ItemCategoriesEditor : public Control {
	GDCLASS(ItemCategoriesEditor, Control);

private:
	EditorPlugin *editor_plugin;
	Ref<InventoryDatabase> database;

	// UI Components
	HSplitContainer *hsplit_container;
	InventoryItemListEditor *item_list_editor;
	ScrollContainer *details_scroll;
	VBoxContainer *details_container;
	Label *no_selection_label;

	Ref<ItemCategory> current_item;
	FileDialog *icon_dialog;

	void _create_ui();
	void _update_details(const Ref<ItemCategory> &p_item);
	void _clear_details();

	void _on_item_selected(const Variant &p_item, int p_index);
	void _on_item_popup_menu_requested(const Vector2 &p_position);
	
	// Property change handlers
	void _on_id_changed(const String &p_text);
	void _on_id_focus_exited();
	void _on_name_changed(const String &p_text);
	void _on_name_focus_exited();
	void _on_color_changed(const Color &p_color);
	void _on_code_changed(double p_value);
	void _on_icon_button_pressed();
	void _on_clear_icon_pressed();
	void _on_icon_dialog_file_selected(const String &p_path);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void load_from_database(const Ref<InventoryDatabase> &p_database);
	void select_item(const Ref<ItemCategory> &p_item);

	// Signal emissions
	void _emit_removed(const Ref<ItemCategory> &p_item);
	void _emit_duplicated(const Ref<ItemCategory> &p_item);

	ItemCategoriesEditor();
	~ItemCategoriesEditor();
};

#endif // ITEM_CATEGORIES_EDITOR_H

#endif // TOOLS_ENABLED