/**
 * inventory_item_list_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef INVENTORY_ITEM_LIST_EDITOR_H
#define INVENTORY_ITEM_LIST_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class InventoryItemListEditor : public VBoxContainer {
	GDCLASS(InventoryItemListEditor, VBoxContainer);

private:
	Array items;
	Array item_list_handler; // Filtered items for display
	Dictionary item_map; // Map of item id -> item object
	String filter;

	// UI Components
	Control *search_items;
	LineEdit *search_line_edit;
	TextureRect *search_icon;
	ItemList *item_list;

	void _create_ui();
	void _update_item_map();
	void _update_item_list(const Array &p_items);
	void _update_item(int p_index);
	void _apply_filter();

	void _on_search_text_changed(const String &p_text);
	void _on_item_list_item_selected(int p_index);
	void _on_item_list_item_clicked(int p_index, const Vector2 &p_at_position, int p_button_index);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_items(const Array &p_items);
	Array get_items() const { return items; }
	void set_filter(const String &p_filter);
	String get_filter() const { return filter; }

	void load_items(const Array &p_items);
	void add_item(const Variant &p_item);
	void clear_items();
	int get_index_of_item_id(const String &p_id);
	void select_item(const Variant &p_item);
	void update_item(int p_index);

	// Signals
	void _emit_item_selected(const Variant &p_item, int p_index);
	void _emit_item_popup_menu_requested(const Vector2 &p_position);

	InventoryItemListEditor();
	~InventoryItemListEditor();
};

#endif // INVENTORY_ITEM_LIST_EDITOR_H

#endif // TOOLS_ENABLED