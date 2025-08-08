/**
 * inventory_editor_plugin.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "inventory_editor_plugin.h"

#include "../base/inventory_database.h"
#include "../base/craft_station_type.h"
#include "../base/item_category.h"
#include "inventory_settings.h"
#include "inventory_item_list_editor.h"
#include "recipes_editor.h"
#include "craft_station_types_editor.h"
#include "item_categories_editor.h"
#include "loots_editor.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/v_separator.hpp>

using namespace godot;

// InventoryEditor

void InventoryEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_database_menu_pressed"), &InventoryEditor::_on_database_menu_pressed);
	ClassDB::bind_method(D_METHOD("_on_database_menu_id_pressed", "id"), &InventoryEditor::_on_database_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_recent_menu_id_pressed", "id"), &InventoryEditor::_on_recent_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_dialog_file_selected", "path"), &InventoryEditor::_on_new_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_open_dialog_file_selected", "path"), &InventoryEditor::_on_open_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_save_inv_dialog_file_selected", "path"), &InventoryEditor::_on_save_inv_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_open_inv_dialog_file_selected", "path"), &InventoryEditor::_on_open_inv_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_new_item_button_pressed"), &InventoryEditor::_on_new_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_recipe_button_pressed"), &InventoryEditor::_on_new_recipe_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_craft_station_button_pressed"), &InventoryEditor::_on_new_craft_station_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_category_button_pressed"), &InventoryEditor::_on_new_category_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_loot_button_pressed"), &InventoryEditor::_on_new_loot_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_tab_button_pressed", "tab_index"), &InventoryEditor::_on_tab_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_definitions_tab_pressed"), &InventoryEditor::_on_item_definitions_tab_pressed);
	ClassDB::bind_method(D_METHOD("_on_recipes_tab_pressed"), &InventoryEditor::_on_recipes_tab_pressed);
	ClassDB::bind_method(D_METHOD("_on_craft_station_types_tab_pressed"), &InventoryEditor::_on_craft_station_types_tab_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_categories_tab_pressed"), &InventoryEditor::_on_item_categories_tab_pressed);
	ClassDB::bind_method(D_METHOD("_on_loots_tab_pressed"), &InventoryEditor::_on_loots_tab_pressed);
	ClassDB::bind_method(D_METHOD("_remove_item_definition", "item_def"), &InventoryEditor::_remove_item_definition);
	ClassDB::bind_method(D_METHOD("_duplicate_item_definition", "item_def"), &InventoryEditor::_duplicate_item_definition);
	ClassDB::bind_method(D_METHOD("_remove_recipe", "recipe"), &InventoryEditor::_remove_recipe);
	ClassDB::bind_method(D_METHOD("_duplicate_recipe", "recipe"), &InventoryEditor::_duplicate_recipe);
	ClassDB::bind_method(D_METHOD("_remove_craft_station_type", "craft_station_type"), &InventoryEditor::_remove_craft_station_type);
	ClassDB::bind_method(D_METHOD("_duplicate_craft_station_type", "craft_station_type"), &InventoryEditor::_duplicate_craft_station_type);
	ClassDB::bind_method(D_METHOD("_remove_item_category", "item_category"), &InventoryEditor::_remove_item_category);
	ClassDB::bind_method(D_METHOD("_duplicate_item_category", "item_category"), &InventoryEditor::_duplicate_item_category);
	ClassDB::bind_method(D_METHOD("_remove_loot", "loot"), &InventoryEditor::_remove_loot);
	ClassDB::bind_method(D_METHOD("_duplicate_loot", "loot"), &InventoryEditor::_duplicate_loot);
}

void InventoryEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
			_apply_theme();
			_load_database(Ref<InventoryDatabase>());
		} break;
		case NOTIFICATION_THEME_CHANGED: {
			_apply_theme();
		} break;
	}
}

InventoryEditor::InventoryEditor() {
	editor_plugin = nullptr;
	
	// Initialize tab button pointers
	item_definitions_tab_button = nullptr;
	recipes_tab_button = nullptr;
	craft_station_types_tab_button = nullptr;
	item_categories_tab_button = nullptr;
	loots_tab_button = nullptr;
	
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

InventoryEditor::~InventoryEditor() {
}

void InventoryEditor::set_editor_plugin(EditorPlugin *p_plugin) {
	editor_plugin = p_plugin;
}

void InventoryEditor::edit_database(const Ref<InventoryDatabase> &p_database, const String &p_path) {
	database = p_database;
	database_path = p_path;
	_load_database(database);
}

void InventoryEditor::_create_ui() {
	// Main MarginContainer to match .tscn structure
	margin_container = memnew(MarginContainer);
	add_child(margin_container);
	margin_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	margin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	margin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	// Main VBoxContainer
	main_vbox = memnew(VBoxContainer);
	margin_container->add_child(main_vbox);
	
	// Toolbar MarginContainer - matches .tscn margins
	toolbar_margin = memnew(MarginContainer);
	main_vbox->add_child(toolbar_margin);
	toolbar_margin->add_theme_constant_override("margin_left", 4);
	toolbar_margin->add_theme_constant_override("margin_right", 4);
	
	// Toolbar HBoxContainer - matches .tscn separation
	toolbar = memnew(HBoxContainer);
	toolbar_margin->add_child(toolbar);
	toolbar->add_theme_constant_override("separation", 0);
	
	// Database MenuButton - matches .tscn properties
	database_button = memnew(MenuButton);
	toolbar->add_child(database_button);
	database_button->set_custom_minimum_size(Vector2(28, 28));
	database_button->set_text("Database");
	database_button->set_flat(false);
	database_button->connect("about_to_popup", callable_mp(this, &InventoryEditor::_on_database_menu_pressed));
	database_button->get_popup()->connect("id_pressed", callable_mp(this, &InventoryEditor::_on_database_menu_id_pressed));
	database_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/inventory_database_editor.svg"));
	
	// VSeparator after database button
	VSeparator *sep_after_db = memnew(VSeparator);
	toolbar->add_child(sep_after_db);
	
	// Tab selector buttons - small buttons to select editor tabs
	item_definitions_tab_button = memnew(Button);
	toolbar->add_child(item_definitions_tab_button);
	item_definitions_tab_button->set_custom_minimum_size(Vector2(28, 28));
	item_definitions_tab_button->set_text("Items");
	item_definitions_tab_button->set_tooltip_text("Item Definitions Editor");
	item_definitions_tab_button->set_toggle_mode(true);
	item_definitions_tab_button->set_button_group(memnew(ButtonGroup));
	item_definitions_tab_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_item_definitions_tab_pressed));
	item_definitions_tab_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_inventory_item.svg"));
	
	recipes_tab_button = memnew(Button);
	toolbar->add_child(recipes_tab_button);
	recipes_tab_button->set_custom_minimum_size(Vector2(28, 28));
	recipes_tab_button->set_text("Recipes");
	recipes_tab_button->set_tooltip_text("Recipes Editor");
	recipes_tab_button->set_toggle_mode(true);
	recipes_tab_button->set_button_group(item_definitions_tab_button->get_button_group());
	recipes_tab_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_recipes_tab_pressed));
	recipes_tab_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_recipe.svg"));
	
	craft_station_types_tab_button = memnew(Button);
	toolbar->add_child(craft_station_types_tab_button);
	craft_station_types_tab_button->set_custom_minimum_size(Vector2(28, 28));
	craft_station_types_tab_button->set_text("Stations");
	craft_station_types_tab_button->set_tooltip_text("Craft Station Types Editor");
	craft_station_types_tab_button->set_toggle_mode(true);
	craft_station_types_tab_button->set_button_group(item_definitions_tab_button->get_button_group());
	craft_station_types_tab_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_craft_station_types_tab_pressed));
	craft_station_types_tab_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_craft_station_type.svg"));
	
	item_categories_tab_button = memnew(Button);
	toolbar->add_child(item_categories_tab_button);
	item_categories_tab_button->set_custom_minimum_size(Vector2(28, 28));
	item_categories_tab_button->set_text("Categories");
	item_categories_tab_button->set_tooltip_text("Item Categories Editor");
	item_categories_tab_button->set_toggle_mode(true);
	item_categories_tab_button->set_button_group(item_definitions_tab_button->get_button_group());
	item_categories_tab_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_item_categories_tab_pressed));
	item_categories_tab_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_item_category.svg"));
	
	loots_tab_button = memnew(Button);
	toolbar->add_child(loots_tab_button);
	loots_tab_button->set_custom_minimum_size(Vector2(28, 28));
	loots_tab_button->set_text("Loots");
	loots_tab_button->set_tooltip_text("Loots Editor");
	loots_tab_button->set_toggle_mode(true);
	loots_tab_button->set_button_group(item_definitions_tab_button->get_button_group());
	loots_tab_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_loots_tab_pressed));
	loots_tab_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_loot.svg"));
	
	// VSeparator after tab buttons
	VSeparator *sep1 = memnew(VSeparator);
	toolbar->add_child(sep1);
	
	// New Item Definition Button - matches .tscn properties  
	new_item_button = memnew(Button);
	toolbar->add_child(new_item_button);
	new_item_button->set_custom_minimum_size(Vector2(32, 32));
	new_item_button->set_text("New Item Definition");
	new_item_button->set_tooltip_text("New Inventory Item");
	new_item_button->set_flat(true);
	new_item_button->set_disabled(true);
	new_item_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_item_button_pressed));
	new_item_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_inventory_item.svg"));
	
	// VSeparator
	VSeparator *sep2 = memnew(VSeparator);
	toolbar->add_child(sep2);
	
	// New Recipe Button - matches .tscn properties
	new_recipe_button = memnew(Button);
	toolbar->add_child(new_recipe_button);
	new_recipe_button->set_custom_minimum_size(Vector2(32, 32));
	new_recipe_button->set_text("New Recipe");
	new_recipe_button->set_tooltip_text("New Recipe");
	new_recipe_button->set_flat(true);
	new_recipe_button->set_disabled(true);
	new_recipe_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_recipe_button_pressed));
	new_recipe_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_recipe.svg"));

	// VSeparator
	VSeparator *sep3 = memnew(VSeparator);
	toolbar->add_child(sep3);
	
	// New Craft Station Type Button - matches .tscn properties
	new_craft_station_type_button = memnew(Button);
	toolbar->add_child(new_craft_station_type_button);
	new_craft_station_type_button->set_custom_minimum_size(Vector2(32, 32));
	new_craft_station_type_button->set_text("New Craft Station Type");
	new_craft_station_type_button->set_tooltip_text("New Craft Station Type");
	new_craft_station_type_button->set_flat(true);
	new_craft_station_type_button->set_disabled(true);
	new_craft_station_type_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_craft_station_button_pressed));
	new_craft_station_type_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_craft_station_type.svg"));

	// VSeparator
	VSeparator *sep4 = memnew(VSeparator);
	toolbar->add_child(sep4);
	
	// New Item Category Button - matches .tscn properties
	new_item_categories_button = memnew(Button);
	toolbar->add_child(new_item_categories_button);
	new_item_categories_button->set_custom_minimum_size(Vector2(32, 32));
	new_item_categories_button->set_text("New Item Category");
	new_item_categories_button->set_tooltip_text("New Item Category");
	new_item_categories_button->set_flat(true);
	new_item_categories_button->set_disabled(true);
	new_item_categories_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_category_button_pressed));
	new_item_categories_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_item_category.svg"));

	// VSeparator
	VSeparator *sep5 = memnew(VSeparator);
	toolbar->add_child(sep5);
	
	// New Loot Button - matches .tscn properties
	new_loot_button = memnew(Button);
	toolbar->add_child(new_loot_button);
	new_loot_button->set_custom_minimum_size(Vector2(32, 32));
	new_loot_button->set_text("New Loot");
	new_loot_button->set_tooltip_text("New Loot");
	new_loot_button->set_flat(true);
	new_loot_button->set_disabled(true);
	new_loot_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_loot_button_pressed));
	new_loot_button->set_button_icon(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/new_loot.svg"));

	// VSeparator
	VSeparator *sep6 = memnew(VSeparator);
	toolbar->add_child(sep6);
	
	// Title Label - matches .tscn properties (expand fill, right alignment)
	title_label = memnew(Label);
	toolbar->add_child(title_label);
	title_label->set_text("Open the InventoryDatabase resource to see the content here");
	title_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	title_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
	title_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	
	// Content MarginContainer - matches .tscn margins and visibility
	content = memnew(MarginContainer);
	main_vbox->add_child(content);
	content->set_visible(false); // Hidden by default like in .tscn
	content->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	content->add_theme_constant_override("margin_left", 4);
	content->add_theme_constant_override("margin_top", 4);
	content->add_theme_constant_override("margin_right", 4);
	content->add_theme_constant_override("margin_bottom", 4);
	
	// TabContainer - matches .tscn properties but with hidden tab bar
	tab_container = memnew(TabContainer);
	content->add_child(tab_container);
	tab_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	tab_container->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	tab_container->set_tabs_visible(false); // Hide the tab bar since we use toolbar buttons
	
	// Create tabs
	item_definitions_editor = memnew(ItemDefinitionsEditor);
	tab_container->add_child(item_definitions_editor);
	item_definitions_editor->set_name("Item Definitions");
	item_definitions_editor->set_editor_plugin(editor_plugin);
	item_definitions_editor->connect("removed", callable_mp(this, &InventoryEditor::_remove_item_definition));
	item_definitions_editor->connect("duplicated", callable_mp(this, &InventoryEditor::_duplicate_item_definition));

	RecipesEditor *recipes_tab = memnew(RecipesEditor);
	tab_container->add_child(recipes_tab);
	recipes_tab->set_name("Recipes");
	recipes_tab->set_editor_plugin(editor_plugin);
	recipes_tab->connect("removed", callable_mp(this, &InventoryEditor::_remove_recipe));
	recipes_tab->connect("duplicated", callable_mp(this, &InventoryEditor::_duplicate_recipe));
	
	CraftStationTypesEditor *craft_stations_tab = memnew(CraftStationTypesEditor);
	tab_container->add_child(craft_stations_tab);
	craft_stations_tab->set_name("Craft Station Types");
	craft_stations_tab->set_editor_plugin(editor_plugin);
	craft_stations_tab->connect("removed", callable_mp(this, &InventoryEditor::_remove_craft_station_type));
	craft_stations_tab->connect("duplicated", callable_mp(this, &InventoryEditor::_duplicate_craft_station_type));
	
	ItemCategoriesEditor *categories_tab = memnew(ItemCategoriesEditor);
	tab_container->add_child(categories_tab);
	categories_tab->set_name("Item Categories");
	categories_tab->set_editor_plugin(editor_plugin);
	categories_tab->connect("removed", callable_mp(this, &InventoryEditor::_remove_item_category));
	categories_tab->connect("duplicated", callable_mp(this, &InventoryEditor::_duplicate_item_category));
	
	LootsEditor *loots_tab = memnew(LootsEditor);
	tab_container->add_child(loots_tab);
	loots_tab->set_name("Loots");
	loots_tab->set_editor_plugin(editor_plugin);
	loots_tab->connect("removed", callable_mp(this, &InventoryEditor::_remove_loot));
	loots_tab->connect("duplicated", callable_mp(this, &InventoryEditor::_duplicate_loot));
	
	// Set current tab after all tabs are added
	tab_container->set_current_tab(0);
	
	// Create dialogs
	new_dialog = memnew(FileDialog);
	add_child(new_dialog);
	new_dialog->set_access(FileDialog::ACCESS_RESOURCES);
	new_dialog->set_file_mode(FileDialog::FILE_MODE_SAVE_FILE);
	new_dialog->add_filter("*.tres", "Godot Resource");
	new_dialog->connect("file_selected", callable_mp(this, &InventoryEditor::_on_new_dialog_file_selected));
	
	open_dialog = memnew(FileDialog);
	add_child(open_dialog);
	open_dialog->set_access(FileDialog::ACCESS_RESOURCES);
	open_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_FILE);
	open_dialog->add_filter("*.tres", "Godot Resource");
	open_dialog->connect("file_selected", callable_mp(this, &InventoryEditor::_on_open_dialog_file_selected));
	
	open_inv_dialog = memnew(FileDialog);
	add_child(open_inv_dialog);
	open_inv_dialog->set_access(FileDialog::ACCESS_RESOURCES);
	open_inv_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_FILE);
	open_inv_dialog->add_filter("*.json", "JSON File");
	open_inv_dialog->connect("file_selected", callable_mp(this, &InventoryEditor::_on_open_inv_dialog_file_selected));
	
	save_inv_dialog = memnew(FileDialog);
	add_child(save_inv_dialog);
	save_inv_dialog->set_access(FileDialog::ACCESS_RESOURCES);
	save_inv_dialog->set_file_mode(FileDialog::FILE_MODE_SAVE_FILE);
	save_inv_dialog->add_filter("*.json", "JSON File");
	save_inv_dialog->connect("file_selected", callable_mp(this, &InventoryEditor::_on_save_inv_dialog_file_selected));

	// Dialog sizes
	float scale = EditorInterface::get_singleton()->get_editor_scale();
	Vector2i min_size = Vector2i(600, 500) * scale;
	
	new_dialog->set_min_size(min_size);
	open_dialog->set_min_size(min_size);
	open_inv_dialog->set_min_size(min_size);
	save_inv_dialog->set_min_size(min_size);
}

void InventoryEditor::_apply_theme() {
	
}

void InventoryEditor::_build_database_menu() {
	PopupMenu *menu = database_button->get_popup();
	menu->clear();

	menu->add_icon_item(get_theme_icon("New", "EditorIcons"), "New Database", DATABASE_NEW);
	menu->add_icon_item(get_theme_icon("Load", "EditorIcons"), "Open Database...", DATABASE_OPEN);

	// Add Open Recent submenu
	Array recent_files = InventorySettings::get_recent_files();
	if (!recent_files.is_empty()) {
		PopupMenu *recent_menu = memnew(PopupMenu);
		recent_menu->set_name("recent_menu");
		menu->add_child(recent_menu);
		
		for (int i = 0; i < recent_files.size() && i < 10; i++) {
			String path = recent_files[i];
			String display_name = path.get_file();
			recent_menu->add_icon_item(get_theme_icon("File", "EditorIcons"), display_name, DATABASE_OPEN_RECENT + i);
			recent_menu->set_item_tooltip(i, path);
		}
		
		recent_menu->add_separator();
		recent_menu->add_item("Clear Recent Files", DATABASE_OPEN_RECENT + 100);
		recent_menu->connect("id_pressed", callable_mp(this, &InventoryEditor::_on_recent_menu_id_pressed));
		
		menu->add_submenu_item("Open Recent", "recent_menu");
		menu->set_item_icon(2, get_theme_icon("Load", "EditorIcons"));
		menu->set_item_disabled(menu->get_item_index(DATABASE_OPEN_RECENT), false);
	} else {
		menu->add_item("Open Recent", DATABASE_OPEN_RECENT);
		menu->set_item_disabled(menu->get_item_index(DATABASE_OPEN_RECENT), true);
	}
	
	menu->add_separator();
	menu->add_icon_item(get_theme_icon("Save", "EditorIcons"), "Save", DATABASE_SAVE);
	menu->add_separator();
	menu->add_item("Import JSON...", DATABASE_IMPORT_JSON);
	menu->add_item("Export JSON...", DATABASE_EXPORT_JSON);

	// Disable items if no database is loaded
	menu->set_item_disabled(menu->get_item_index(DATABASE_SAVE), database.is_null());
	menu->set_item_disabled(menu->get_item_index(DATABASE_IMPORT_JSON), database.is_null());
	menu->set_item_disabled(menu->get_item_index(DATABASE_EXPORT_JSON), database.is_null());
}

void InventoryEditor::_load_database(const Ref<InventoryDatabase> &p_database) {
	database = p_database;
	
	if (database.is_valid()) {
		content->set_visible(true);
		new_item_button->set_disabled(false);
		new_recipe_button->set_disabled(false);
		new_craft_station_type_button->set_disabled(false);
		new_item_categories_button->set_disabled(false);
		new_loot_button->set_disabled(false);
		
		// Enable tab buttons
		if (item_definitions_tab_button) item_definitions_tab_button->set_disabled(false);
		if (recipes_tab_button) recipes_tab_button->set_disabled(false);
		if (craft_station_types_tab_button) craft_station_types_tab_button->set_disabled(false);
		if (item_categories_tab_button) item_categories_tab_button->set_disabled(false);
		if (loots_tab_button) loots_tab_button->set_disabled(false);
		
		// Set the first tab as active
		if (item_definitions_tab_button) item_definitions_tab_button->set_pressed(true);
		tab_container->set_current_tab(0);
		
		title_label->set_text(database_path.is_empty() ? "Untitled Database" : database_path);
		
		// Update tab editors
		ItemDefinitionsEditor *items_editor = Object::cast_to<ItemDefinitionsEditor>(tab_container->get_tab_control(0));
		if (items_editor) {
			items_editor->load_from_database(database.ptr());
		}
		
		RecipesEditor *recipes_editor = Object::cast_to<RecipesEditor>(tab_container->get_tab_control(1));
		if (recipes_editor) {
			recipes_editor->load_from_database(database.ptr());
		}
		
		CraftStationTypesEditor *craft_stations_editor = Object::cast_to<CraftStationTypesEditor>(tab_container->get_tab_control(2));
		if (craft_stations_editor) {
			craft_stations_editor->load_from_database(database.ptr());
		}
		
		ItemCategoriesEditor *categories_editor = Object::cast_to<ItemCategoriesEditor>(tab_container->get_tab_control(3));
		if (categories_editor) {
			categories_editor->load_from_database(database.ptr());
		}
		
		LootsEditor *loots_editor = Object::cast_to<LootsEditor>(tab_container->get_tab_control(4));
		if (loots_editor) {
			loots_editor->load_from_database(database.ptr());
		}
	} else {
		content->set_visible(false);
		new_item_button->set_disabled(true);
		new_recipe_button->set_disabled(true);
		new_craft_station_type_button->set_disabled(true);
		new_item_categories_button->set_disabled(true);
		new_loot_button->set_disabled(true);
		
		// Disable tab buttons
		if (item_definitions_tab_button) item_definitions_tab_button->set_disabled(true);
		if (recipes_tab_button) recipes_tab_button->set_disabled(true);
		if (craft_station_types_tab_button) craft_station_types_tab_button->set_disabled(true);
		if (item_categories_tab_button) item_categories_tab_button->set_disabled(true);
		if (loots_tab_button) loots_tab_button->set_disabled(true);
		
		title_label->set_text("No Database");
		
		// Clear tab editors
		ItemDefinitionsEditor *items_editor = Object::cast_to<ItemDefinitionsEditor>(tab_container->get_tab_control(0));
		if (items_editor) {
			items_editor->load_from_database(nullptr);
		}
		
		RecipesEditor *recipes_editor = Object::cast_to<RecipesEditor>(tab_container->get_tab_control(1));
		if (recipes_editor) {
			recipes_editor->load_from_database(nullptr);
		}
		
		CraftStationTypesEditor *craft_stations_editor = Object::cast_to<CraftStationTypesEditor>(tab_container->get_tab_control(2));
		if (craft_stations_editor) {
			craft_stations_editor->load_from_database(nullptr);
		}
		
		ItemCategoriesEditor *categories_editor = Object::cast_to<ItemCategoriesEditor>(tab_container->get_tab_control(3));
		if (categories_editor) {
			categories_editor->load_from_database(nullptr);
		}
		
		LootsEditor *loots_editor = Object::cast_to<LootsEditor>(tab_container->get_tab_control(4));
		if (loots_editor) {
			loots_editor->load_from_database(nullptr);
		}
	}
}

void InventoryEditor::_new_file(const String &p_path) {
	Ref<InventoryDatabase> new_database;
	new_database.instantiate();
	database = new_database;
	database_path = p_path;
	_save_file();
	_load_database(database);
}

void InventoryEditor::_open_file(const String &p_path) {
	Ref<Resource> res = ResourceLoader::get_singleton()->load(p_path);
	Ref<InventoryDatabase> db = res;
	if (db.is_null()) {
		// TODO: Show error dialog
		print_line("Failed to load InventoryDatabase from: " + p_path);
		return;
	}
	
	database = db;
	database_path = p_path;
	title_label->set_text(p_path);
	_load_database(database);
	
	InventorySettings::add_recent_file(p_path);
}

void InventoryEditor::_save_file() {
	if (database.is_null() || database_path.is_empty()) {
		return;
	}
	
	ResourceSaver::get_singleton()->save(database, database_path);
	if (editor_plugin) {
		editor_plugin->get_editor_interface()->get_resource_filesystem()->scan();
	}
}

void InventoryEditor::_import_inv_file(const String &p_path) {
	if (database.is_null()) {
		return;
	}
	
	// Read JSON file
	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
	if (!file.is_valid()) {
		print_line("Failed to read JSON file: " + p_path);
		return;
	}
	
	String json_string = file->get_as_text();
	file->close();
	
	// Parse JSON
	Ref<JSON> json;
	json.instantiate();
	Error parse_result = json->parse(json_string);
	if (parse_result != OK) {
		print_line("Failed to parse JSON: " + p_path);
		return;
	}
	
	Dictionary data = json->get_data();
	
	// Import data into database
	database->deserialize(data);
	_load_database(database);
	print_line("Database imported from JSON: " + p_path);
}

void InventoryEditor::_on_database_menu_pressed() {
	_build_database_menu();
}

void InventoryEditor::_on_database_menu_id_pressed(int p_id) {
	switch (p_id) {
		case DATABASE_NEW:
			new_dialog->popup_centered();
			break;
		case DATABASE_OPEN:
			open_dialog->popup_centered();
			break;
		case DATABASE_SAVE:
			_save_file();
			break;
		case DATABASE_IMPORT_JSON:
			open_inv_dialog->popup_centered();
			break;
		case DATABASE_EXPORT_JSON:
			save_inv_dialog->popup_centered();
			break;
	}
}

void InventoryEditor::_on_recent_menu_id_pressed(int p_id) {
	Array recent_files = InventorySettings::get_recent_files();
	
	if (p_id >= DATABASE_OPEN_RECENT + 100) {
		// Clear recent files
		InventorySettings::clear_recent_files();
		return;
	}
	
	int index = p_id - DATABASE_OPEN_RECENT;
	if (index >= 0 && index < recent_files.size()) {
		String path = recent_files[index];
		_open_file(path);
	}
}

void InventoryEditor::_on_new_dialog_file_selected(const String &p_path) {
	_new_file(p_path);
}

void InventoryEditor::_on_open_dialog_file_selected(const String &p_path) {
	_open_file(p_path);
}

void InventoryEditor::_on_save_inv_dialog_file_selected(const String &p_path) {
	if (database.is_null()) {
		return;
	}
	
	// Serialize database to JSON
	Dictionary data = database->serialize();
	Ref<JSON> json;
	json.instantiate();
	String json_string = json->stringify(data, "\t");
	
	// Write to file
	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE);
	if (file.is_valid()) {
		file->store_string(json_string);
		file->close();
		print_line("Database exported to JSON: " + p_path);
	} else {
		print_line("Failed to write JSON file: " + p_path);
	}
}

void InventoryEditor::_on_open_inv_dialog_file_selected(const String &p_path) {
	_import_inv_file(p_path);
}

void InventoryEditor::_on_new_item_button_pressed() {
	if (database.is_null()) {
		return;
	}
	
	// Create new item definition
	Ref<ItemDefinition> new_item;
	new_item.instantiate();
	new_item->set_name("New Item Definition");
	new_item->set_id(""); // Empty ID will need to be filled by user
	
	database->add_new_item(new_item);
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(0);
}

void InventoryEditor::_on_new_recipe_button_pressed() {
	if (database.is_null()) {
		return;
	}
	database->add_recipe();
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(1);
}

void InventoryEditor::_on_new_craft_station_button_pressed() {
	if (database.is_null()) {
		return;
	}

	database->add_craft_station_type();
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(2);
}

void InventoryEditor::_on_new_category_button_pressed() {
	if (database.is_null()) {
		return;
	}
	
	database->add_item_category();
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(3);
}

void InventoryEditor::_on_new_loot_button_pressed() {
	if (database.is_null()) {
		return;
	}
	
	database->add_loot();
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(4);
}

void InventoryEditor::_remove_item_definition(const Ref<ItemDefinition> &p_item_def) {
	if (p_item_def.is_null()) {
		return;
	}
	database->remove_item(p_item_def);
	_save_file();
}

void InventoryEditor::_duplicate_item_definition(const Ref<ItemDefinition> &p_item_def) {
	if (p_item_def->get_id() == "") {
		WARN_PRINT("Item definition with empty id cannot be duplicated.");
		return;
	}
	Ref<ItemDefinition> new_item = p_item_def->duplicate();
	while (database->has_item_id(new_item->get_id())) {
		new_item->set_id(new_item->get_id() + "_duplicate");
	}
	while (database->has_item_name(new_item->get_name())) {
		new_item->set_name(new_item->get_name() + "(2)");
	}
	database->add_new_item(new_item);
	_save_file();
	_load_database(database);
	item_definitions_editor->select(new_item);
}

void InventoryEditor::_remove_recipe(const Ref<Recipe> &p_recipe) {
	if (p_recipe.is_null()) {
		return;
	}
	database->remove_recipe(p_recipe);
	_save_file();
}

void InventoryEditor::_duplicate_recipe(const Ref<Recipe> &p_recipe) {
	Ref<Recipe> new_recipe = p_recipe->duplicate();
	database->add_new_recipe(new_recipe);
	_save_file();
	_load_database(database);
	// Switch to recipes tab and select the new recipe
	tab_container->set_current_tab(1);
}

void InventoryEditor::_remove_craft_station_type(const Ref<CraftStationType> &p_craft_station_type) {
	if (p_craft_station_type.is_null()) {
		return;
	}
	database->remove_craft_station_type(p_craft_station_type);
	_save_file();
}

void InventoryEditor::_duplicate_craft_station_type(const Ref<CraftStationType> &p_craft_station_type) {
	if (p_craft_station_type->get_id() == "") {
		WARN_PRINT("Craft station type with empty id cannot be duplicated.");
		return;
	}
	Ref<CraftStationType> new_craft_station = p_craft_station_type->duplicate();
	String base_id = new_craft_station->get_id();
	int counter = 2;
	while (database->has_craft_station_type_id(base_id + "_" + String::num(counter))) {
		counter++;
	}
	new_craft_station->set_id(base_id + "_" + String::num(counter));
	new_craft_station->set_name(new_craft_station->get_name() + " (2)");
	database->add_new_craft_station_type(new_craft_station);
	_save_file();
	_load_database(database);
	// Switch to craft station types tab
	tab_container->set_current_tab(2);
}

void InventoryEditor::_remove_item_category(const Ref<ItemCategory> &p_item_category) {
	if (p_item_category.is_null()) {
		return;
	}
	database->remove_category(p_item_category);
	_save_file();
}

void InventoryEditor::_duplicate_item_category(const Ref<ItemCategory> &p_item_category) {
	if (p_item_category->get_id() == "") {
		WARN_PRINT("Item category with empty id cannot be duplicated.");
		return;
	}
	Ref<ItemCategory> new_category = p_item_category->duplicate();
	String base_id = new_category->get_id();
	int counter = 2;
	while (database->has_item_category_id(base_id + "_" + String::num(counter))) {
		counter++;
	}
	new_category->set_id(base_id + "_" + String::num(counter));
	new_category->set_name(new_category->get_name() + " (2)");
	database->add_new_category(new_category);
	_save_file();
	_load_database(database);
	// Switch to item categories tab
	tab_container->set_current_tab(3);
}

void InventoryEditor::_remove_loot(const Ref<Loot> &p_loot) {
	if (p_loot.is_null()) {
		return;
	}
	database->remove_loot(p_loot);
	_save_file();
}

void InventoryEditor::_duplicate_loot(const Ref<Loot> &p_loot) {
	Ref<Loot> new_loot = memnew(Loot);
	new_loot->set_name(p_loot->get_name() + " (Copy)");
	new_loot->set_items(p_loot->get_items());
	database->add_new_loot(new_loot);
	_save_file();
	_load_database(database);
	// Switch to loots tab
	tab_container->set_current_tab(4);
}

void InventoryEditor::_on_tab_button_pressed(int tab_index) {
	if (tab_container && tab_index >= 0 && tab_index < tab_container->get_tab_count()) {
		tab_container->set_current_tab(tab_index);
		
		// Update button states - make sure the correct button is pressed
		if (item_definitions_tab_button) item_definitions_tab_button->set_pressed(tab_index == 0);
		if (recipes_tab_button) recipes_tab_button->set_pressed(tab_index == 1);
		if (craft_station_types_tab_button) craft_station_types_tab_button->set_pressed(tab_index == 2);
		if (item_categories_tab_button) item_categories_tab_button->set_pressed(tab_index == 3);
		if (loots_tab_button) loots_tab_button->set_pressed(tab_index == 4);
	}
}

void InventoryEditor::_on_item_definitions_tab_pressed() {
	_on_tab_button_pressed(0);
}

void InventoryEditor::_on_recipes_tab_pressed() {
	_on_tab_button_pressed(1);
}

void InventoryEditor::_on_craft_station_types_tab_pressed() {
	_on_tab_button_pressed(2);
}

void InventoryEditor::_on_item_categories_tab_pressed() {
	_on_tab_button_pressed(3);
}

void InventoryEditor::_on_loots_tab_pressed() {
	_on_tab_button_pressed(4);
}

// InventoryEditorPlugin

void InventoryEditorPlugin::_bind_methods() {
}

void InventoryEditorPlugin::_notification(int p_notification) {
	switch (p_notification) {
		case NOTIFICATION_ENTER_TREE: {
			inventory_editor = memnew(InventoryEditor);
			inventory_editor->set_editor_plugin(this);
			EditorInterface::get_singleton()->get_editor_main_screen()->add_child(inventory_editor);
			_make_visible(false);
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (inventory_editor) {
				EditorInterface::get_singleton()->get_editor_main_screen()->remove_child(inventory_editor);
				memdelete(inventory_editor);
				inventory_editor = nullptr;
			}
		} break;
	}
}

InventoryEditorPlugin::InventoryEditorPlugin() {
	inventory_editor = nullptr;
}

InventoryEditorPlugin::~InventoryEditorPlugin() {
}

void InventoryEditorPlugin::_make_visible(bool p_visible) {
	if (inventory_editor) {
		inventory_editor->set_visible(p_visible);
	}
}

void InventoryEditorPlugin::_edit(Object *p_object) {
	Ref<InventoryDatabase> database = Object::cast_to<InventoryDatabase>(p_object);
	if (database.is_valid() && inventory_editor) {
		inventory_editor->edit_database(database);
	}
}

bool InventoryEditorPlugin::_handles(Object *p_object) const {
	return Object::cast_to<InventoryDatabase>(p_object) != nullptr;
}

Ref<Texture2D> InventoryEditorPlugin::_get_plugin_icon() const {
	return Ref<Texture2D>(ResourceLoader::get_singleton()->load("res://addons/inventory-system/icons/inventory_main_screen.svg"));
}

#endif // TOOLS_ENABLED