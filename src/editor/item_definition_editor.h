/**
 * item_definition_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef ITEM_DEFINITION_EDITOR_H
#define ITEM_DEFINITION_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include "base/resource_id_editor.h"
#include "base/icon_selector.h"
#include "base/custom_properties_editor.h"
#include "base/categories_in_item_editor.h"

using namespace godot;

class InventoryDatabase;
class ItemDefinition;

class ItemDefinitionEditor : public Control {
	GDCLASS(ItemDefinitionEditor, Control);

private:
	Ref<ItemDefinition> item;
	InventoryDatabase* database;
	EditorPlugin* editor_plugin;

	// UI Components matching addon structure
	ScrollContainer* scroll_container;
	MarginContainer* margin_container;
	VBoxContainer* main_vbox;
	
	// Form components
	ResourceIDEditor* resource_id_editor;
	LineEdit* item_name_text_edit;
	SpinBox* item_max_stack_spin_box;
	IconSelector* icon_selector;
	SpinBox* weight_spin_box;
	CheckBox* can_stack_check_box;
	HBoxContainer* max_stack_container;
	SpinBox* size_x_spin_box;
	SpinBox* size_y_spin_box;
	TextEdit* item_description_text_edit;
	
	// Custom Properties section
	CustomPropertiesEditor* custom_properties;
	
	// Categories section
	CategoriesInItemEditor* categories_in_item;

	void _create_ui();
	void _connect_signals();
	void _disconnect_signals();

	// Signal handlers - matching addon method names exactly
	void _on_max_stack_spin_box_value_changed(double value);
	void _on_text_edit_text_changed(const String& new_text);
	void _on_icon_changed(const Ref<Texture2D>& icon);
	void _on_resource_id_editor_changed(const String& id);
	void _on_weight_spin_box_value_changed(double value);
	void _can_stack_check_box_toggled(bool value);
	void _on_size_x_spin_box_value_changed(double value);
	void _on_size_y_spin_box_value_changed(double value);
	void _on_item_description_text_edit_text_changed();
	void _on_categories_in_item_changed();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin* p_editor_plugin);
	void load_item(const Ref<ItemDefinition>& p_item, InventoryDatabase* p_database);
	void reload_item();

	ItemDefinitionEditor();
	~ItemDefinitionEditor();
};

#endif // ITEM_DEFINITION_EDITOR_H

#endif // TOOLS_ENABLED