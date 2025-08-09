/**
 * loot_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "loot_editor.h"
#include "../base/inventory_database.h"
#include "../base/loot.h"
#include "../base/loot_item.h"
#include "../base/item_definition.h"
#include "../base/item_stack.h"
#include "item_stack_selector.h"

#include <godot_cpp/classes/v_separator.hpp>
#include <godot_cpp/classes/h_separator.hpp>

using namespace godot;

void LootEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_id_changed", "id"), &LootEditor::_on_id_changed);
	ClassDB::bind_method(D_METHOD("_on_name_text_changed", "text"), &LootEditor::_on_name_text_changed);
	ClassDB::bind_method(D_METHOD("_on_add_item_button_pressed"), &LootEditor::_on_add_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_remove_item_button_pressed"), &LootEditor::_on_remove_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_items_list_item_selected", "index"), &LootEditor::_on_items_list_item_selected);
	ClassDB::bind_method(D_METHOD("_on_items_list_item_clicked", "index", "position", "mouse_button_index"), &LootEditor::_on_items_list_item_clicked);
	ClassDB::bind_method(D_METHOD("_on_context_menu_id_pressed", "id"), &LootEditor::_on_context_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_stack_changed", "item_stack"), &LootEditor::_on_item_stack_changed);
	ClassDB::bind_method(D_METHOD("_on_weight_value_changed", "value"), &LootEditor::_on_weight_value_changed);
	ClassDB::bind_method(D_METHOD("_on_min_amount_value_changed", "value"), &LootEditor::_on_min_amount_value_changed);
	ClassDB::bind_method(D_METHOD("_on_max_amount_value_changed", "value"), &LootEditor::_on_max_amount_value_changed);

	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "loot", PROPERTY_HINT_RESOURCE_TYPE, "Loot")));
}

void LootEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
			_apply_theme();
		} break;
		case NOTIFICATION_THEME_CHANGED: {
			_apply_theme();
		} break;
	}
}

LootEditor::LootEditor() {
	database = nullptr;
	editor_plugin = nullptr;
	item_stack_selector = nullptr;
	items_hsplit = nullptr;
	items_left_vbox = nullptr;
	items_right_vbox = nullptr;
	
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

LootEditor::~LootEditor() {
}

void LootEditor::_create_ui() {
	ScrollContainer *scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	main_vbox = memnew(VBoxContainer);
	main_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	scroll_container->add_child(main_vbox);

	// Header section - using pattern from item_definition_editor
	header_hbox = memnew(HBoxContainer);
	main_vbox->add_child(header_hbox);

	// Left side: VBoxContainer for ID and Name
	VBoxContainer *left_vbox = memnew(VBoxContainer);
	header_hbox->add_child(left_vbox);
	left_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// ResourceIDEditor - ID comes first
	id_editor = memnew(ResourceIDEditor);
	left_vbox->add_child(id_editor);
	id_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_LOOT);
	id_editor->connect("changed", callable_mp(this, &LootEditor::_on_id_changed));

	// Name field - HBoxContainer following item_definition_editor pattern
	HBoxContainer *name_hbox = memnew(HBoxContainer);
	left_vbox->add_child(name_hbox);
	name_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label *name_label = memnew(Label);
	name_hbox->add_child(name_label);
	name_label->set_text("Name");
	name_label->set_custom_minimum_size(Vector2(160, 0));

	name_line_edit = memnew(LineEdit);
	name_hbox->add_child(name_line_edit);
	name_line_edit->set_custom_minimum_size(Vector2(0, 32));
	name_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	name_line_edit->set_placeholder("Enter loot name here");
	name_line_edit->connect("text_changed", callable_mp(this, &LootEditor::_on_name_text_changed));

	// Total weight field - HBoxContainer following item_definition_editor pattern
	HBoxContainer *weight_hbox = memnew(HBoxContainer);
	left_vbox->add_child(weight_hbox);
	weight_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	Label *weight_info_label = memnew(Label);
	weight_hbox->add_child(weight_info_label);
	weight_info_label->set_text("Total Weight");
	weight_info_label->set_custom_minimum_size(Vector2(160, 0));

	total_weight_label = memnew(Label);
	weight_hbox->add_child(total_weight_label);
	total_weight_label->set_text("0.0");
	total_weight_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	// Spacer Control matching item_definition_editor
	Control *spacer = memnew(Control);
	left_vbox->add_child(spacer);
	spacer->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Separator
	HSeparator *sep1 = memnew(HSeparator);
	main_vbox->add_child(sep1);

	// Items section with horizontal split container
	items_hsplit = memnew(HSplitContainer);
	main_vbox->add_child(items_hsplit);
	items_hsplit->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	items_hsplit->set_split_offset(-300); // Default split position

	// Left side: Items list
	items_left_vbox = memnew(VBoxContainer);
	items_hsplit->add_child(items_left_vbox);
	items_left_vbox->set_custom_minimum_size(Vector2(200, 0));

	// Items section header with title and buttons
	HBoxContainer *items_header = memnew(HBoxContainer);
	items_left_vbox->add_child(items_header);
	
	items_label = memnew(Label);
	items_header->add_child(items_label);
	items_label->set_text("Loot Items:");
	items_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	add_item_button = memnew(Button);
	items_header->add_child(add_item_button);
	add_item_button->set_text("+");
	add_item_button->set_tooltip_text("Add Item");
	add_item_button->set_custom_minimum_size(Vector2(32, 32));
	add_item_button->set_flat(true); // Make button flat
	add_item_button->connect("pressed", callable_mp(this, &LootEditor::_on_add_item_button_pressed));

	remove_item_button = memnew(Button);
	items_header->add_child(remove_item_button);
	remove_item_button->set_text("-");
	remove_item_button->set_tooltip_text("Remove Item");
	remove_item_button->set_custom_minimum_size(Vector2(32, 32));
	remove_item_button->set_flat(true); // Make button flat
	remove_item_button->set_disabled(true);
	remove_item_button->connect("pressed", callable_mp(this, &LootEditor::_on_remove_item_button_pressed));

	// Items list - set to expand and fill available space
	items_list = memnew(ItemList);
	items_left_vbox->add_child(items_list);
	items_list->set_custom_minimum_size(Vector2(0, 150));
	items_list->set_v_size_flags(Control::SIZE_EXPAND_FILL); // Fill available vertical space
	items_list->set_allow_rmb_select(true);
	items_list->set_fixed_icon_size(Vector2i(16, 16)); // Match inventory_item_list icon size
	items_list->connect("item_selected", callable_mp(this, &LootEditor::_on_items_list_item_selected));
	items_list->connect("item_clicked", callable_mp(this, &LootEditor::_on_items_list_item_clicked));

	// Right side: Item details section
	items_right_vbox = memnew(VBoxContainer);
	items_hsplit->add_child(items_right_vbox);
	items_right_vbox->set_custom_minimum_size(Vector2(300, 0));

	item_details_vbox = memnew(VBoxContainer);
	items_right_vbox->add_child(item_details_vbox);
	item_details_vbox->set_v_size_flags(Control::SIZE_SHRINK_END); // Don't expand, stay compact

	item_details_label = memnew(Label);
	item_details_vbox->add_child(item_details_label);
	item_details_label->set_text("Item Details:");

	// Item selection using ItemStackSelector - no label needed
	item_stack_selector = memnew(ItemStackSelector);
	item_details_vbox->add_child(item_stack_selector);
	item_stack_selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	item_stack_selector->set_show_quantity(false); // Hide quantity for loot editor
	item_stack_selector->connect("changed", callable_mp(this, &LootEditor::_on_item_stack_changed));

	// Weight
	weight_hbox = memnew(HBoxContainer);
	item_details_vbox->add_child(weight_hbox);

	weight_label = memnew(Label);
	weight_hbox->add_child(weight_label);
	weight_label->set_text("Weight:");
	weight_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	weight_spinbox = memnew(SpinBox);
	weight_hbox->add_child(weight_spinbox);
	weight_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	weight_spinbox->set_min(0.0);
	weight_spinbox->set_max(999999.0);
	weight_spinbox->set_step(0.1);
	weight_spinbox->set_value(1.0);
	weight_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_weight_value_changed));

	// Min and Max amount - combined in one horizontal container
	HBoxContainer *amount_hbox = memnew(HBoxContainer);
	item_details_vbox->add_child(amount_hbox);

	min_amount_label = memnew(Label);
	amount_hbox->add_child(min_amount_label);
	min_amount_label->set_text("Min Amount:");
	min_amount_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	min_amount_spinbox = memnew(SpinBox);
	amount_hbox->add_child(min_amount_spinbox);
	min_amount_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	min_amount_spinbox->set_min(1);
	min_amount_spinbox->set_max(999999);
	min_amount_spinbox->set_step(1);
	min_amount_spinbox->set_value(1);
	min_amount_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_min_amount_value_changed));

	max_amount_label = memnew(Label);
	amount_hbox->add_child(max_amount_label);
	max_amount_label->set_text("Max Amount:");
	max_amount_label->set_custom_minimum_size(Vector2(160, 0)); // Match other editors

	max_amount_spinbox = memnew(SpinBox);
	amount_hbox->add_child(max_amount_spinbox);
	max_amount_spinbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	max_amount_spinbox->set_min(1);
	max_amount_spinbox->set_max(999999);
	max_amount_spinbox->set_step(1);
	max_amount_spinbox->set_value(1);
	max_amount_spinbox->connect("value_changed", callable_mp(this, &LootEditor::_on_max_amount_value_changed));

	// Context menu
	context_menu = memnew(PopupMenu);
	add_child(context_menu);
	context_menu->connect("id_pressed", callable_mp(this, &LootEditor::_on_context_menu_id_pressed));

	// Disable item details by default
	item_details_vbox->set_visible(false);
}

void LootEditor::_apply_theme() {
	if (!editor_plugin) {
		return;
	}
	
	// Set icons for add and remove buttons using Godot editor icons
	if (add_item_button) {
		Ref<Texture2D> add_icon = get_theme_icon("Add", "EditorIcons");
		if (add_icon.is_valid()) {
			add_item_button->set_button_icon(add_icon);
			add_item_button->set_text(""); // Remove text when icon is available
		}
	}
	
	if (remove_item_button) {
		Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
		if (remove_icon.is_valid()) {
			remove_item_button->set_button_icon(remove_icon);
			remove_item_button->set_text(""); // Remove text when icon is available
		}
	}
	
	// Setup context menu with proper icons
	if (context_menu) {
		context_menu->clear();
		
		// Add Remove option with editor icon
		Ref<Texture2D> remove_icon = get_theme_icon("Remove", "EditorIcons");
		context_menu->add_icon_item(remove_icon, "Remove", BaseInventoryEditor::ITEM_REMOVE);
		
		// Add Duplicate option with editor icon
		Ref<Texture2D> duplicate_icon = get_theme_icon("Duplicate", "EditorIcons");
		context_menu->add_icon_item(duplicate_icon, "Duplicate", BaseInventoryEditor::ITEM_DUPLICATE);
	}
}

void LootEditor::load_loot(const Ref<Loot> &p_loot, InventoryDatabase *p_database) {
	current_loot = p_loot;
	database = p_database;
	_update_ui();
}

void LootEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
	_apply_theme();
}

void LootEditor::clear() {
	current_loot = Ref<Loot>();
	database = nullptr;
	_update_ui();
}

void LootEditor::_update_ui() {
	if (current_loot.is_null()) {
		if (id_editor) {
			id_editor->setup(database, "");
		}
		name_line_edit->set_text("");
		items_list->clear();
		item_details_vbox->set_visible(false);
		_update_total_weight();
		return;
	}

	if (id_editor) {
		id_editor->setup(database, current_loot->get_id());
	}
	name_line_edit->set_text(current_loot->get_name());
	_update_items_list();
	_update_total_weight();
}

void LootEditor::_update_items_list() {
	if (!items_list || current_loot.is_null()) {
		return;
	}

	items_list->clear();
	
	TypedArray<LootItem> items = current_loot->get_items();
	for (int i = 0; i < items.size(); i++) {
		Ref<LootItem> item = items[i];
		if (item.is_valid()) {
			String item_id = item->get_item_id();
			String display_text = "No Item";
			Ref<Texture2D> item_icon;
			
			// Get item definition to show name and icon
			if (!item_id.is_empty() && database) {
				Ref<ItemDefinition> item_def = database->get_item(item_id);
				if (item_def.is_valid()) {
					display_text = item_def->get_name();
					item_icon = item_def->get_icon();
				} else {
					display_text = item_id; // Fallback to ID if definition not found
				}
			}
			
			// Show weight and min-max amount range
			display_text += " (weight: " + String::num_real(item->get_weight()) + ") ";
			if (item->get_min_amount() == item->get_max_amount()) {
				display_text += "(" + String::num_int64(item->get_min_amount()) + ")";
			} else {
				display_text += "(" + String::num_int64(item->get_min_amount()) + "-" + String::num_int64(item->get_max_amount()) + ")";
			}
			
			items_list->add_item(display_text);
			if (item_icon.is_valid()) {
				items_list->set_item_icon(i, item_icon);
			}
			items_list->set_item_metadata(i, item);
		}
	}
	if (remove_item_button) {
		remove_item_button->set_disabled(items.size() == 0);
	}
}

void LootEditor::_update_item_details() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	
	if (selected_item.is_null()) {
		item_details_vbox->set_visible(false);
		return;
	}

	item_details_vbox->set_visible(true);
	
	// Update item stack selector 
	if (item_stack_selector && database) {
		Ref<ItemStack> item_stack = memnew(ItemStack);
		item_stack->set_item_id(selected_item->get_item_id());
		item_stack->set_amount(selected_item->get_min_amount()); // Use min_amount as default
		item_stack_selector->setup(item_stack, database);
	}
	
	weight_spinbox->set_value(selected_item->get_weight());
	min_amount_spinbox->set_value(selected_item->get_min_amount());
	max_amount_spinbox->set_value(selected_item->get_max_amount());
}

void LootEditor::_update_total_weight() {
	float total = 0.0;
	if (current_loot.is_valid()) {
		total = current_loot->get_total_weight();
	}
	total_weight_label->set_text(String::num_real(total));
}

Ref<LootItem> LootEditor::_get_selected_loot_item() {
	if (!items_list || items_list->get_selected_items().size() == 0) {
		return Ref<LootItem>();
	}

	int selected = items_list->get_selected_items()[0];
	return items_list->get_item_metadata(selected);
}

void LootEditor::_on_id_changed(const String &p_id) {
	if (current_loot.is_valid() && id_editor) {
		current_loot->set_id(id_editor->get_id());
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_name_text_changed(const String &p_text) {
	if (current_loot.is_valid()) {
		current_loot->set_name(p_text);
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_add_item_button_pressed() {
	if (current_loot.is_null()) {
		return;
	}

	Ref<LootItem> new_item = memnew(LootItem);
	new_item->set_item_id("");
	new_item->set_weight(1.0);
	new_item->set_min_amount(1);
	new_item->set_max_amount(1);

	TypedArray<LootItem> items = current_loot->get_items();
	items.append(new_item);
	current_loot->set_items(items);

	_update_items_list();
	_update_total_weight();
	
	// Select the new item
	if (items_list->get_item_count() > 0) {
		items_list->select(items_list->get_item_count() - 1);
		_on_items_list_item_selected(items_list->get_item_count() - 1);
	}
	
	emit_signal("changed", current_loot);
}

void LootEditor::_on_remove_item_button_pressed() {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || current_loot.is_null()) {
		return;
	}

	TypedArray<LootItem> items = current_loot->get_items();
	int index = items.find(selected_item);
	if (index >= 0) {
		items.remove_at(index);
		current_loot->set_items(items);
		_update_items_list();
		_update_total_weight();
		item_details_vbox->set_visible(false);
		emit_signal("changed", current_loot);
	}
}

void LootEditor::_on_items_list_item_selected(int p_index) {
	_update_item_details();
}

void LootEditor::_on_items_list_item_clicked(int p_index, const Vector2 &p_position, int p_mouse_button_index) {
	if (p_mouse_button_index == MOUSE_BUTTON_LEFT) {
		// Left click - select the item and update details
		items_list->select(p_index);
		_update_item_details();
	} else if (p_mouse_button_index == MOUSE_BUTTON_RIGHT) {
		// Right click - show context menu with proper positioning
		if (!context_menu) {
			return;
		}

		items_list->select(p_index);
		_update_item_details();
		
		// Position popup at mouse position using the same pattern as other editors
		Vector2 global_pos = get_global_mouse_position();
		Variant v = get_viewport()->get("position");
		context_menu->set_position(((Vector2)v) + global_pos);
		context_menu->popup();
	}
}


void LootEditor::_on_context_menu_id_pressed(int p_id) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || current_loot.is_null()) {
		return;
	}

	switch (p_id) {
		case BaseInventoryEditor::ITEM_REMOVE:
			_on_remove_item_button_pressed();
			break;
		case BaseInventoryEditor::ITEM_DUPLICATE: {
			Ref<LootItem> new_item = memnew(LootItem);
			new_item->set_item_id(selected_item->get_item_id());
			new_item->set_weight(selected_item->get_weight());
			new_item->set_min_amount(selected_item->get_min_amount());
			new_item->set_max_amount(selected_item->get_max_amount());

			TypedArray<LootItem> items = current_loot->get_items();
			items.append(new_item);
			current_loot->set_items(items);

			_update_items_list();
			_update_total_weight();
			emit_signal("changed", current_loot);
		} break;
	}
}

void LootEditor::_on_item_stack_changed(const Ref<ItemStack> &p_item_stack) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null() || p_item_stack.is_null()) {
		return;
	}

	selected_item->set_item_id(p_item_stack->get_item_id());
	_update_items_list();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_weight_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_weight(p_value);
	_update_items_list();
	_update_total_weight();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_min_amount_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_min_amount((int)p_value);
	
	// Ensure max amount is not less than min amount
	if (selected_item->get_max_amount() < (int)p_value) {
		selected_item->set_max_amount((int)p_value);
		max_amount_spinbox->set_value((int)p_value);
	}
	
	_update_items_list();
	emit_signal("changed", current_loot);
}

void LootEditor::_on_max_amount_value_changed(double p_value) {
	Ref<LootItem> selected_item = _get_selected_loot_item();
	if (selected_item.is_null()) {
		return;
	}

	selected_item->set_max_amount((int)p_value);
	
	// Ensure min amount is not greater than max amount
	if (selected_item->get_min_amount() > (int)p_value) {
		selected_item->set_min_amount((int)p_value);
		min_amount_spinbox->set_value((int)p_value);
	}
	
	_update_items_list();
	emit_signal("changed", current_loot);
}

#endif // TOOLS_ENABLED