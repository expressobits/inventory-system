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
#include "inventory_settings.h"
#include "item_definitions_editor.h"
#include "inventory_item_list_editor.h"
#include "recipes_editor.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

// InventoryEditor

void InventoryEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_database_menu_pressed"), &InventoryEditor::_on_database_menu_pressed);
	ClassDB::bind_method(D_METHOD("_on_database_menu_id_pressed", "id"), &InventoryEditor::_on_database_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_dialog_file_selected", "path"), &InventoryEditor::_on_new_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_open_dialog_file_selected", "path"), &InventoryEditor::_on_open_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_save_inv_dialog_file_selected", "path"), &InventoryEditor::_on_save_inv_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_open_inv_dialog_file_selected", "path"), &InventoryEditor::_on_open_inv_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_new_item_button_pressed"), &InventoryEditor::_on_new_item_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_recipe_button_pressed"), &InventoryEditor::_on_new_recipe_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_craft_station_button_pressed"), &InventoryEditor::_on_new_craft_station_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_new_category_button_pressed"), &InventoryEditor::_on_new_category_button_pressed);
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
	// Main container
	margin_container = memnew(MarginContainer);
	add_child(margin_container);
	margin_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	
	main_vbox = memnew(VBoxContainer);
	margin_container->add_child(main_vbox);
	
	// Toolbar
	toolbar_margin = memnew(MarginContainer);
	main_vbox->add_child(toolbar_margin);
	toolbar_margin->add_theme_constant_override("margin_left", 4);
	toolbar_margin->add_theme_constant_override("margin_right", 4);
	
	toolbar = memnew(HBoxContainer);
	toolbar_margin->add_child(toolbar);
	toolbar->add_theme_constant_override("separation", 0);
	
	// Database button
	database_button = memnew(MenuButton);
	toolbar->add_child(database_button);
	database_button->set_text("Database");
	database_button->connect("about_to_popup", callable_mp(this, &InventoryEditor::_on_database_menu_pressed));
	database_button->get_popup()->connect("id_pressed", callable_mp(this, &InventoryEditor::_on_database_menu_id_pressed));
	
	// Title label
	title_label = memnew(Label);
	toolbar->add_child(title_label);
	title_label->set_text("No Database");
	title_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	
	// New buttons
	new_item_button = memnew(Button);
	toolbar->add_child(new_item_button);
	new_item_button->set_text("New Item");
	new_item_button->set_disabled(true);
	new_item_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_item_button_pressed));
	
	new_recipe_button = memnew(Button);
	toolbar->add_child(new_recipe_button);
	new_recipe_button->set_text("New Recipe");
	new_recipe_button->set_disabled(true);
	new_recipe_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_recipe_button_pressed));
	
	new_craft_station_type_button = memnew(Button);
	toolbar->add_child(new_craft_station_type_button);
	new_craft_station_type_button->set_text("New Craft Station");
	new_craft_station_type_button->set_disabled(true);
	new_craft_station_type_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_craft_station_button_pressed));
	
	new_item_categories_button = memnew(Button);
	toolbar->add_child(new_item_categories_button);
	new_item_categories_button->set_text("New Category");
	new_item_categories_button->set_disabled(true);
	new_item_categories_button->connect("pressed", callable_mp(this, &InventoryEditor::_on_new_category_button_pressed));
	
	// Content area
	content = memnew(Control);
	main_vbox->add_child(content);
	content->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	
	tab_container = memnew(TabContainer);
	content->add_child(tab_container);
	tab_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	
	// Create tabs
	ItemDefinitionsEditor *items_tab = memnew(ItemDefinitionsEditor);
	tab_container->add_child(items_tab);
	items_tab->set_name("Item Definitions");
	items_tab->set_editor_plugin(editor_plugin);
	
	RecipesEditor *recipes_tab = memnew(RecipesEditor);
	tab_container->add_child(recipes_tab);
	recipes_tab->set_name("Recipes");
	recipes_tab->set_editor_plugin(editor_plugin);
	
	Control *craft_stations_tab = memnew(Control);
	tab_container->add_child(craft_stations_tab);
	craft_stations_tab->set_name("Craft Station Types");
	
	Control *categories_tab = memnew(Control);
	tab_container->add_child(categories_tab);
	categories_tab->set_name("Item Categories");
	
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
}

void InventoryEditor::_apply_theme() {
	if (!editor_plugin || !database_button) {
		return;
	}
	
	database_button->set_tooltip_text("Database Menu");
	
	// Dialog sizes
	float scale = EditorInterface::get_singleton()->get_editor_scale();
	Vector2i min_size = Vector2i(600, 500) * scale;
	
	new_dialog->set_min_size(min_size);
	open_dialog->set_min_size(min_size);
	open_inv_dialog->set_min_size(min_size);
	save_inv_dialog->set_min_size(min_size);
}

void InventoryEditor::_build_database_menu() {
	PopupMenu *menu = database_button->get_popup();
	menu->clear();
	
	menu->add_item("New Database", DATABASE_NEW);
	menu->add_item("Open Database...", DATABASE_OPEN);
	menu->add_separator();
	menu->add_item("Save Database", DATABASE_SAVE);
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
		title_label->set_text(database_path.is_empty() ? "Untitled Database" : database_path);
		
		// Update tab editors
		ItemDefinitionsEditor *items_editor = Object::cast_to<ItemDefinitionsEditor>(tab_container->get_tab_control(0));
		if (items_editor) {
			items_editor->load_from_database(database);
		}
		
		RecipesEditor *recipes_editor = Object::cast_to<RecipesEditor>(tab_container->get_tab_control(1));
		if (recipes_editor) {
			recipes_editor->load_from_database(database);
		}
	} else {
		content->set_visible(false);
		new_item_button->set_disabled(true);
		new_recipe_button->set_disabled(true);
		new_craft_station_type_button->set_disabled(true);
		new_item_categories_button->set_disabled(true);
		title_label->set_text("No Database");
		
		// Clear tab editors
		ItemDefinitionsEditor *items_editor = Object::cast_to<ItemDefinitionsEditor>(tab_container->get_tab_control(0));
		if (items_editor) {
			items_editor->load_from_database(Ref<InventoryDatabase>());
		}
		
		RecipesEditor *recipes_editor = Object::cast_to<RecipesEditor>(tab_container->get_tab_control(1));
		if (recipes_editor) {
			recipes_editor->load_from_database(Ref<InventoryDatabase>());
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
	
	// TODO: Implement JSON import
	print_line("JSON import not yet implemented: " + p_path);
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
	// TODO: Implement JSON export
	print_line("JSON export not yet implemented: " + p_path);
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
	
	// Create new recipe
	Ref<Recipe> new_recipe;
	new_recipe.instantiate();
	new_recipe->set_name("New Recipe");
	new_recipe->set_id(""); // Empty ID will need to be filled by user
	
	database->add_new_recipe(new_recipe);
	_save_file();
	_load_database(database);
	tab_container->set_current_tab(1);
}

void InventoryEditor::_on_new_craft_station_button_pressed() {
	if (database.is_null()) {
		return;
	}
	
	// TODO: Create new craft station type
	print_line("New craft station creation not yet implemented");
	tab_container->set_current_tab(2);
}

void InventoryEditor::_on_new_category_button_pressed() {
	if (database.is_null()) {
		return;
	}
	
	// TODO: Create new category
	print_line("New category creation not yet implemented");
	tab_container->set_current_tab(3);
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