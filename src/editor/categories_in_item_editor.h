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
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>
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
	TypedArray<ItemCategory> item_categories; // Categories assigned to the item

	// UI Components 
	Label* categories_label;
	VBoxContainer* categories_container;
	ItemList* categories_list;       // Shows only assigned categories
	HBoxContainer* add_category_container;
	OptionButton* available_categories_option;  // Shows unassigned categories
	Button* add_button;
	Button* remove_button;

	void _create_ui();
	void _clear_categories();
	void _update_item_categories_list();
	void _update_available_categories_option();
	void _apply_theme();
	void _on_category_selected(int index);
	void _on_add_button_pressed();
	void _on_remove_button_pressed();

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