/**
 * loot_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef LOOT_EDITOR_H
#define LOOT_EDITOR_H

#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include "base_resource_editor.h"
#include "resource_id_editor.h"
#include "loot_item_editor.h"

using namespace godot;

class InventoryDatabase;
class Loot;
class LootItem;

class LootEditor : public BaseResourceEditor {
	GDCLASS(LootEditor, BaseResourceEditor);

private:
	// Data
	Ref<Loot> current_loot;
	InventoryDatabase *database;
	EditorPlugin *editor_plugin;

	// UI Components
	HBoxContainer *header_hbox;
	ResourceIDEditor *id_editor;
	LineEdit *name_line_edit;
	Label *total_weight_label;
	SpinBox *min_rolls_spinbox;
	SpinBox *max_rolls_spinbox;
	SpinBox *none_weight_spinbox;
	
	// Items section with horizontal split
	HSplitContainer *items_hsplit;
	VBoxContainer *items_left_vbox;
	VBoxContainer *items_right_vbox;
	
	// Loot items section (left side)
	Label *items_label;
	Button *add_item_button;
	Button *remove_item_button;
	ItemList *items_list;
	
	// Item details section (right side)
	LootItemEditor *loot_item_editor;
	
	PopupMenu *context_menu;

	void _create_ui();
	void _apply_theme();
	void _update_ui();
	void _update_items_list();
	void _update_total_weight();
	void _update_item_id_options();

	// Signal handlers
	void _on_id_changed(const String &p_id);
	void _on_name_text_changed(const String &p_text);
	void _on_min_rolls_value_changed(double p_value);
	void _on_max_rolls_value_changed(double p_value);
	void _on_none_weight_value_changed(double p_value);
	void _on_add_item_button_pressed();
	void _on_remove_item_button_pressed();
	void _on_items_list_item_selected(int p_index);
	void _on_items_list_item_clicked(int p_index, const Vector2 &p_position, int p_mouse_button_index);
	void _on_items_list_item_rmb_selected(int p_index, const Vector2 &p_position);
	void _on_context_menu_id_pressed(int p_id);
	void _on_loot_item_changed(const Ref<LootItem> &p_loot_item);

	Ref<LootItem> _get_selected_loot_item();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:

	void load_loot(const Ref<Loot> &p_loot, InventoryDatabase *p_database);
	void set_editor_plugin(EditorPlugin *p_plugin);
	void clear();

	LootEditor();
	~LootEditor();
};

#endif // LOOT_EDITOR_H

#endif // TOOLS_ENABLED