/**
 * item_definitions_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "item_definitions_editor.h"

#include "../base/inventory_database.h"
#include "../base/item_definition.h"

#include <godot_cpp/classes/editor_inspector.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

using namespace godot;

void ItemDefinitionsEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_item_selected", "item", "index"), &ItemDefinitionsEditor::_on_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_popup_menu_requested", "at_position"), &ItemDefinitionsEditor::_on_item_popup_menu_requested);

	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")));
	ADD_SIGNAL(MethodInfo("duplicated", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition")));
}

void ItemDefinitionsEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

ItemDefinitionsEditor::ItemDefinitionsEditor() {
	editor_plugin = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

ItemDefinitionsEditor::~ItemDefinitionsEditor() {
}

void ItemDefinitionsEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void ItemDefinitionsEditor::_create_ui() {
	// Split container
	hsplit_container = memnew(HSplitContainer);
	add_child(hsplit_container);
	hsplit_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	// Item list editor
	item_list_editor = memnew(InventoryItemListEditor);
	hsplit_container->add_child(item_list_editor);
	item_list_editor->set_custom_minimum_size(Vector2(256, 0));
	item_list_editor->connect("item_selected", callable_mp(this, &ItemDefinitionsEditor::_on_item_selected));
	item_list_editor->connect("item_popup_menu_requested", callable_mp(this, &ItemDefinitionsEditor::_on_item_popup_menu_requested));

	// Details scroll container
	details_scroll = memnew(ScrollContainer);
	hsplit_container->add_child(details_scroll);
	details_scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Details container
	details_container = memnew(VBoxContainer);
	details_scroll->add_child(details_container);

	// No selection label
	no_selection_label = memnew(Label);
	details_container->add_child(no_selection_label);
	no_selection_label->set_text("Select an item to edit its properties");
	no_selection_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	no_selection_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	no_selection_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	no_selection_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

void ItemDefinitionsEditor::load_from_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		TypedArray<ItemDefinition> items = database->get_items();
		Array item_array;
		for (int i = 0; i < items.size(); i++) {
			item_array.append(items[i]);
		}
		item_list_editor->set_items(item_array);
	} else {
		item_list_editor->clear_items();
	}
	
	_clear_details();
}

void ItemDefinitionsEditor::select_item(const Ref<ItemDefinition> &p_item) {
	if (p_item.is_valid()) {
		item_list_editor->select_item(p_item);
		_update_details(p_item);
	}
}

void ItemDefinitionsEditor::_update_details(const Ref<ItemDefinition> &p_item) {
	current_item = p_item;
	
	// Clear existing details
	_clear_details();
	
	if (p_item.is_null()) {
		no_selection_label->set_visible(true);
		return;
	}
	
	no_selection_label->set_visible(false);
	
	// TODO: Create detailed property editor for item definition
	// For now, just show basic info
	Label *item_label = memnew(Label);
	details_container->add_child(item_label);
	item_label->set_text("Item: " + p_item->get_name());
	
	Label *id_label = memnew(Label);
	details_container->add_child(id_label);
	id_label->set_text("ID: " + p_item->get_id());
	
	Label *description_label = memnew(Label);
	details_container->add_child(description_label);
	description_label->set_text("Description: " + p_item->get_description());
	description_label->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
}

void ItemDefinitionsEditor::_clear_details() {
	// Remove all children except the no selection label
	Array children = details_container->get_children();
	for (int i = 0; i < children.size(); i++) {
		Node *child = Object::cast_to<Node>(children[i]);
		if (child && child != no_selection_label) {
			details_container->remove_child(child);
			child->queue_free();
		}
	}
}

void ItemDefinitionsEditor::_emit_removed(const Ref<ItemDefinition> &p_item) {
	emit_signal("removed", p_item);
}

void ItemDefinitionsEditor::_emit_duplicated(const Ref<ItemDefinition> &p_item) {
	emit_signal("duplicated", p_item);
}

void ItemDefinitionsEditor::_on_item_selected(const Variant &p_item, int p_index) {
	Ref<ItemDefinition> item = p_item;
	_update_details(item);
}

void ItemDefinitionsEditor::_on_item_popup_menu_requested(const Vector2 &p_position) {
	// TODO: Show context menu with options like duplicate, remove, etc.
	print_line("Context menu requested at position: " + p_position);
}

#endif // TOOLS_ENABLED