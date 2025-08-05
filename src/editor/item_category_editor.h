/**
 * item_category_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef ITEM_CATEGORY_EDITOR_H
#define ITEM_CATEGORY_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include "base/icon_selector.h"
#include "base/custom_properties_editor.h"
#include "base/resource_id_editor.h"

using namespace godot;

class InventoryDatabase;
class ItemCategory;
class CustomPropertiesEditor;

class ItemCategoryEditor : public Control {
	GDCLASS(ItemCategoryEditor, Control);

private:
	Ref<ItemCategory> item_category;
	InventoryDatabase* database;
	EditorPlugin* editor_plugin;

	// UI Components matching addon structure
	ScrollContainer* scroll_container;
	MarginContainer* margin_container;
	VBoxContainer* main_vbox;
	
	// Form components
	ResourceIDEditor* resource_id_editor;
	LineEdit* name_edit;
	ColorPickerButton* color_picker;
	SpinBox* code_spin_box;
	IconSelector* icon_selector;
	
	// Custom Properties section
	CustomPropertiesEditor* custom_properties;

	void _create_ui();
	void _connect_signals();
	void _disconnect_signals();

	// Signal handlers - matching addon method names exactly
	void _on_resource_id_editor_changed(const String& id);
	void _on_name_changed(const String& text);
	void _on_name_focus_exited();
	void _on_color_changed(const Color& color);
	void _on_code_changed(double value);
	void _on_icon_changed(const Ref<Texture2D>& icon);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin* p_editor_plugin);
	void load_item_category(const Ref<ItemCategory>& p_item_category, InventoryDatabase* p_database);
	void reload_item_category();

	ItemCategoryEditor();
	~ItemCategoryEditor();
};

#endif // ITEM_CATEGORY_EDITOR_H

#endif // TOOLS_ENABLED