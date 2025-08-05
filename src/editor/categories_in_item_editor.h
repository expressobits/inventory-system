/**
 * categories_in_item_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef CATEGORIES_IN_ITEM_EDITOR_H
#define CATEGORIES_IN_ITEM_EDITOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/h_flow_container.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;

class InventoryDatabase;
class ItemDefinition;
class ItemCategory;

class CategoriesInItemEditor : public HBoxContainer {
	GDCLASS(CategoriesInItemEditor, HBoxContainer);

private:
	InventoryDatabase* database;
	Ref<ItemDefinition> item;
	TypedArray<CheckBox> category_checkboxes;

	// UI Components matching addon structure
	Label* categories_label;
	ScrollContainer* scroll_container;
	HFlowContainer* h_flow_container;

	void _create_ui();
	void _clear_categories();
	void _on_category_toggled(bool toggled, const Ref<ItemCategory>& category);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void load_item(InventoryDatabase* p_database, const Ref<ItemDefinition>& p_item);
	void loading_categories();

	CategoriesInItemEditor();
	~CategoriesInItemEditor();
};

#endif // CATEGORIES_IN_ITEM_EDITOR_H

#endif // TOOLS_ENABLED