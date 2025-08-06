/**
 * loots_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef LOOTS_EDITOR_H
#define LOOTS_EDITOR_H

#include "base_inventory_editor.h"
#include "inventory_item_list_editor.h"
#include "loot_editor.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

using namespace godot;

class InventoryDatabase;
class Loot;

class LootsEditor : public BaseInventoryEditor {
	GDCLASS(LootsEditor, BaseInventoryEditor);

private:
	// UI Components matching addon structure exactly
	HSplitContainer* hsplit_container;
	LootEditor* loot_editor;
	InventoryItemListEditor* inventory_item_list_editor;
	PopupMenu* items_popup_menu;

	void _create_ui();

	// Signal handlers - matching addon method names exactly
	void _on_inventory_item_list_item_selected(const Variant& item, int index);
	void _on_inventory_item_list_item_popup_menu_requested(const Vector2& at_position);
	void _on_items_popup_menu_id_pressed(int id);
	void _on_loot_editor_changed(const Ref<Loot>& loot);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	// Public methods matching addon interface exactly
	void set_editor_plugin(EditorPlugin* p_editor_plugin) override;
	void on_load_database() override;
	void load_loots();
	bool remove_current_data() override;
	void select(const Ref<Loot>& loot);

	LootsEditor();
	~LootsEditor();
};

#endif // LOOTS_EDITOR_H

#endif // TOOLS_ENABLED