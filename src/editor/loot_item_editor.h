/**
 * loot_item_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef LOOT_ITEM_EDITOR_H
#define LOOT_ITEM_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include "item_stack_selector.h"

using namespace godot;

class InventoryDatabase;
class LootItem;
class ItemStack;

class LootItemEditor : public Control {
	GDCLASS(LootItemEditor, Control);

private:
	// Data
	Ref<LootItem> current_item;
	InventoryDatabase *database;
	EditorPlugin *editor_plugin;

	// Main container
	VBoxContainer *main_vbox;
	
	// Item details section
	Label *item_details_label;
	ItemStackSelector *item_stack_selector;
	HBoxContainer *weight_hbox;
	Label *weight_label;
	SpinBox *weight_spinbox;
	Label *min_amount_label;
	SpinBox *min_amount_spinbox;
	Label *max_amount_label;
	SpinBox *max_amount_spinbox;
	
	// Property ranges section
	Label *property_ranges_label;
	Button *add_property_range_button;
	Button *remove_property_range_button;
	ItemList *property_ranges_list;
	
	// Property range details
	VBoxContainer *property_range_details_vbox;
	Label *property_range_details_label;
	LineEdit *property_name_line_edit;
	OptionButton *property_type_option;
	HBoxContainer *range_values_hbox;
	Label *min_value_label;
	SpinBox *min_value_spinbox;
	Label *max_value_label;
	SpinBox *max_value_spinbox;
	Label *bool_value_label;
	CheckBox *bool_value_checkbox;

	void _create_ui();
	void _apply_theme();
	void _update_ui();
	void _update_property_ranges_list();
	void _update_property_range_details();

	// Signal handlers
	void _on_item_stack_changed(const Ref<ItemStack> &p_item_stack);
	void _on_weight_value_changed(double p_value);
	void _on_min_amount_value_changed(double p_value);
	void _on_max_amount_value_changed(double p_value);
	void _on_add_property_range_button_pressed();
	void _on_remove_property_range_button_pressed();
	void _on_property_ranges_list_item_selected(int p_index);
	void _on_property_name_text_changed(const String &p_text);
	void _on_property_type_item_selected(int p_index);
	void _on_min_value_value_changed(double p_value);
	void _on_max_value_value_changed(double p_value);
	void _on_bool_value_toggled(bool p_pressed);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void load_item(const Ref<LootItem> &p_item, InventoryDatabase *p_database);
	void set_editor_plugin(EditorPlugin *p_plugin);
	void clear();

	LootItemEditor();
	~LootItemEditor();
};

#endif // LOOT_ITEM_EDITOR_H

#endif // TOOLS_ENABLED