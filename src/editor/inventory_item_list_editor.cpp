/**
 * inventory_item_list_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "inventory_item_list_editor.h"

#include "../base/item_definition.h"
#include "../base/item_category.h"

#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

void InventoryItemListEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &InventoryItemListEditor::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &InventoryItemListEditor::get_items);
	ClassDB::bind_method(D_METHOD("set_filter", "filter"), &InventoryItemListEditor::set_filter);
	ClassDB::bind_method(D_METHOD("get_filter"), &InventoryItemListEditor::get_filter);
	
	ClassDB::bind_method(D_METHOD("_on_search_text_changed", "text"), &InventoryItemListEditor::_on_search_text_changed);
	ClassDB::bind_method(D_METHOD("_on_item_list_item_selected", "index"), &InventoryItemListEditor::_on_item_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_list_item_clicked", "index", "at_position", "button_index"), &InventoryItemListEditor::_on_item_list_item_clicked);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items"), "set_items", "get_items");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "filter"), "set_filter", "get_filter");

	ADD_SIGNAL(MethodInfo("item_selected", PropertyInfo(Variant::OBJECT, "item"), PropertyInfo(Variant::INT, "index")));
	ADD_SIGNAL(MethodInfo("item_popup_menu_requested", PropertyInfo(Variant::VECTOR2, "at_position")));
}

void InventoryItemListEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

InventoryItemListEditor::InventoryItemListEditor() {
	set_custom_minimum_size(Vector2(256, 0));
}

InventoryItemListEditor::~InventoryItemListEditor() {
}

void InventoryItemListEditor::_create_ui() {
	// Search container
	search_items = memnew(Control);
	add_child(search_items);
	search_items->set_custom_minimum_size(Vector2(0, 32));
	search_items->set_v_size_flags(Control::SIZE_SHRINK_CENTER);

	// Search line edit
	search_line_edit = memnew(LineEdit);
	search_items->add_child(search_line_edit);
	search_line_edit->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	search_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	search_line_edit->set_placeholder("Search Items");
	search_line_edit->connect("text_changed", callable_mp(this, &InventoryItemListEditor::_on_search_text_changed));

	// Search icon
	search_icon = memnew(TextureRect);
	search_items->add_child(search_icon);
	search_icon->set_custom_minimum_size(Vector2(16, 16));
	search_icon->set_anchors_preset(Control::PRESET_CENTER_RIGHT);
	search_icon->set_offset(Side::SIDE_LEFT, -24);
	search_icon->set_offset(Side::SIDE_TOP, -8);
	search_icon->set_offset(Side::SIDE_RIGHT, -8);
	search_icon->set_offset(Side::SIDE_BOTTOM, 8);
	search_icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	search_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	search_icon->set_texture(get_theme_icon("Search", "EditorIcons"));

	// Item list
	item_list = memnew(ItemList);
	add_child(item_list);
	item_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	item_list->set_fixed_icon_size(Vector2i(16, 16));
	item_list->connect("item_selected", callable_mp(this, &InventoryItemListEditor::_on_item_list_item_selected));
	item_list->connect("item_clicked", callable_mp(this, &InventoryItemListEditor::_on_item_list_item_clicked));
}

void InventoryItemListEditor::set_items(const Array &p_items) {
	items = p_items;
	_update_item_map();
	_apply_filter();
}

void InventoryItemListEditor::set_filter(const String &p_filter) {
	filter = p_filter;
	_apply_filter();
}

void InventoryItemListEditor::load_items(const Array &p_items) {
	clear_items();
	for (int i = 0; i < p_items.size(); i++) {
		add_item(p_items[i]);
	}
	_update_item_map();
	_apply_filter();
}

void InventoryItemListEditor::add_item(const Variant &p_item) {
	items.append(p_item);
	_update_item_map();
	_apply_filter();
}

void InventoryItemListEditor::clear_items() {
	items.clear();
}

void InventoryItemListEditor::_update_item_map() {
	item_map.clear();
	for (int i = 0; i < items.size(); i++) {
		Object *obj = items[i];
		if (!obj) continue;
		
		// Try to get id property for different item types
		String id;
		if (obj->has_method("get_id")) {
			id = obj->call("get_id");
		} else if (obj->get("id").get_type() == Variant::STRING) {
			id = obj->get("id");
		}
		
		if (!id.is_empty()) {
			item_map[id] = obj;
		}
	}
}

void InventoryItemListEditor::_update_item_list(const Array &p_items) {
	if (!item_list) return;
	
	item_list->clear();
	for (int i = 0; i < p_items.size(); i++) {
		item_list->add_item("");
		_update_item(i);
	}
}

void InventoryItemListEditor::_update_item(int p_index) {
	if (!item_list || p_index >= item_list_handler.size()) return;
	
	Object *obj = item_list_handler[p_index];
	if (!obj) return;
	
	String name_to_show;
	Ref<Texture2D> icon;
	
	// Get name
	if (obj->get("name").get_type() == Variant::STRING) {
		String name = obj->get("name");
		name_to_show = name.is_empty() ? "(Unnamed Item)" : name;
	} else {
		name_to_show = "(Unnamed Item)";
	}
	
	// Get icon if available
	if (obj->get("icon").get_type() == Variant::OBJECT) {
		icon = obj->get("icon");
	}
	
	item_list->set_item_text(p_index, name_to_show);
	if (icon.is_valid()) {
		item_list->set_item_icon(p_index, icon);
	}
}

int InventoryItemListEditor::get_index_of_item_id(const String &p_id) {
	for (int i = 0; i < item_list_handler.size(); i++) {
		Object *obj = item_list_handler[i];
		if (!obj) continue;
		
		String id;
		if (obj->has_method("get_id")) {
			id = obj->call("get_id");
		} else if (obj->get("id").get_type() == Variant::STRING) {
			id = obj->get("id");
		}
		
		if (id == p_id) {
			return i;
		}
	}
	return -1;
}

void InventoryItemListEditor::select_item(const Variant &p_item) {
	Object *obj = p_item;
	if (!obj) return;
	
	String id;
	if (obj->has_method("get_id")) {
		id = obj->call("get_id");
	} else if (obj->get("id").get_type() == Variant::STRING) {
		id = obj->get("id");
	}
	
	int index = get_index_of_item_id(id);
	if (index >= 0 && item_list) {
		item_list->select(index);
	}
}

void InventoryItemListEditor::_apply_filter() {
	item_list_handler.clear();
	
	for (int i = 0; i < items.size(); i++) {
		Object *obj = items[i];
		if (!obj) continue;
		
		String name;
		if (obj->get("name").get_type() == Variant::STRING) {
			name = obj->get("name");
		}
		
		// Check name filter
		if (filter.is_empty() || name.to_lower().contains(filter.to_lower())) {
			item_list_handler.append(obj);
			continue;
		}
		
		// Check category filter for ItemDefinition
		ItemDefinition *item_def = Object::cast_to<ItemDefinition>(obj);
		if (item_def) {
			TypedArray<ItemCategory> categories = item_def->get_categories();
			for (int j = 0; j < categories.size(); j++) {
				ItemCategory *category = Object::cast_to<ItemCategory>(categories[j]);
				if (category) {
					String cat_name = category->get_name();
					if (cat_name.to_lower().contains(filter.to_lower())) {
						item_list_handler.append(obj);
						break;
					}
				}
			}
		}
	}
	
	_update_item_list(item_list_handler);
}

void InventoryItemListEditor::_emit_item_selected(const Variant &p_item, int p_index) {
	emit_signal("item_selected", p_item, p_index);
}

void InventoryItemListEditor::update_item(int p_index) {
	_update_item(p_index);
}

void InventoryItemListEditor::_emit_item_popup_menu_requested(const Vector2 &p_position) {
	emit_signal("item_popup_menu_requested", p_position);
}

void InventoryItemListEditor::_on_search_text_changed(const String &p_text) {
	set_filter(p_text);
}

void InventoryItemListEditor::_on_item_list_item_selected(int p_index) {
	if (!item_list->is_item_selectable(p_index) || p_index >= item_list_handler.size()) {
		return;
	}
	
	_emit_item_selected(item_list_handler[p_index], p_index);
}

void InventoryItemListEditor::_on_item_list_item_clicked(int p_index, const Vector2 &p_at_position, int p_button_index) {
	if (!item_list->is_item_selectable(p_index) || p_index >= item_list_handler.size()) {
		return;
	}

	if (p_button_index != MouseButton::MOUSE_BUTTON_RIGHT) {
		return;
	}

	if (!item_list->get_selected_items().has(p_index)) {
		item_list->select(p_index);
		item_list->emit_signal("item_selected", p_index);
	}

	_emit_item_popup_menu_requested(p_at_position);
}

#endif // TOOLS_ENABLED