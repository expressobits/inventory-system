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

#include "base_inventory_editor.h"
#include "inventory_item_list_editor.h"
#include "item_category_editor.h"

#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

using namespace godot;

class InventoryDatabase;
class ItemCategory;

class ItemCategoriesEditor : public BaseInventoryEditor {
	GDCLASS(ItemCategoriesEditor, BaseInventoryEditor);

private:
	// UI Components matching addon structure exactly
	HSplitContainer* hsplit_container;
	ItemCategoryEditor* item_category_editor;
	InventoryItemListEditor* inventory_item_list_editor;
	PopupMenu* items_popup_menu;

	void _create_ui();
	void _apply_theme() override;

	// Signal handlers - matching addon method names exactly
	void _on_theme_changed();
	void _on_inventory_item_list_item_selected(const Variant& item, int index);
	void _on_inventory_item_list_item_popup_menu_requested(const Vector2& at_position);
	void _on_items_popup_menu_id_pressed(int id);
	void _on_item_category_editor_changed(const Ref<ItemCategory>& item_category);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	// Public methods matching addon interface exactly
	void set_editor_plugin(EditorPlugin* p_editor_plugin) override;
	void on_load_database() override;
	void load_item_categories();
	bool remove_current_data() override;
	void select(const Ref<ItemCategory>& item_category);

	ItemCategoriesEditor();
	~ItemCategoriesEditor();
};

#endif // ITEM_CATEGORIES_EDITOR_H

#endif // TOOLS_ENABLED