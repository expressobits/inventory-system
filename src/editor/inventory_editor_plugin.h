/**
 * inventory_editor_plugin.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef INVENTORY_EDITOR_PLUGIN_H
#define INVENTORY_EDITOR_PLUGIN_H

#include "../base/item_definition.h"
#include "../base/recipe.h"
#include "../base/craft_station_type.h"
#include "../base/loot.h"
#include "item_definitions_editor.h"

#include <godot_cpp/classes/button_group.hpp>
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/menu_button.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/tab_container.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/accept_dialog.hpp>

using namespace godot;

class InventoryDatabase;
class InventoryItemListEditor;

class InventoryEditor : public Control {
	GDCLASS(InventoryEditor, Control);

private:
	enum OpenMenuId {
		OPEN_OPEN = 100,
		OPEN_CLEAR = 101,
	};

	enum DatabaseMenuId {
		DATABASE_NEW = 100,
		DATABASE_OPEN = 200,
		DATABASE_OPEN_RECENT = 250,
		DATABASE_SAVE = 300,
		DATABASE_IMPORT_JSON = 401,
		DATABASE_EXPORT_JSON = 501,
	};

	enum NewItemMenuId {
		NEW_ITEM_NEW_RESOURCE = 100,
		NEW_ITEM_FROM_RESOURCE = 101,
	};

	enum MiscMenuId {
		MISC_ONLINE_DOCUMENTATION = 100,
		MISC_GITHUB_REPOSITORY = 200,
		MISC_REPORT_BUG = 300,
		MISC_ABOUT_INVENTORY_SYSTEM = 400,
	};

	EditorPlugin *editor_plugin;
	Ref<InventoryDatabase> database;
	String database_path;

	// UI Components
	VBoxContainer *main_vbox;
	MarginContainer *margin_container;
	VBoxContainer *content_vbox;
	MarginContainer *toolbar_margin;
	HBoxContainer *toolbar;
	Control *content;
	TabContainer *tab_container;
	ItemDefinitionsEditor *item_definitions_editor;
	// RecipesEditor *recipes_editor;
	// CraftStationTypesEditor *craft_station_types_editor;
	// ItemCategoriesEditor *item_categories_editor;


	// Dialogs
	FileDialog *new_dialog;
	FileDialog *save_dialog;
	FileDialog *open_dialog;
	FileDialog *open_inv_dialog;
	FileDialog *save_inv_dialog;

	//Toolbar
	Button *database_new_button;
	MenuButton *database_open_button;
	Button *database_save_button;
	MenuButton *database_json_button;
	MenuButton *misc_button;
	Button *item_definitions_tab_button;
	Button *recipes_tab_button;
	Button *craft_station_types_tab_button;
	Button *item_categories_tab_button;
	Button *loots_tab_button;
	Label *title_label;
	Button *new_item_button;
	Button *new_recipe_button;
	Button *new_craft_station_type_button;
	Button *new_item_categories_button;
	Button *new_loot_button;

	void _create_ui();
	void _apply_theme();
	void _build_open_menu();
	void _build_json_menu();
	void _build_misc_menu();
	void _load_database(const Ref<InventoryDatabase> &p_database);
	void _new_file(const String &p_path);
	void _open_file(const String &p_path);
	void _save_file();
	void _import_inv_file(const String &p_path);

	void _on_database_new_button_pressed();
	void _on_database_open_menu_pressed();
	void _on_database_open_menu_id_pressed(int p_id);
	void _on_database_save_button_pressed();
	void _on_database_json_menu_pressed();
	void _on_database_json_menu_id_pressed(int p_id);
	void _on_misc_menu_pressed();
	void _on_misc_menu_id_pressed(int p_id);
	void _on_new_dialog_file_selected(const String &p_path);
	void _on_open_dialog_file_selected(const String &p_path);
	void _on_save_inv_dialog_file_selected(const String &p_path);
	void _on_open_inv_dialog_file_selected(const String &p_path);
	void _on_new_item_button_pressed();
	void _on_new_recipe_button_pressed();
	void _on_new_craft_station_button_pressed();
	void _on_new_category_button_pressed();
	void _on_new_loot_button_pressed();
	void _on_tab_button_pressed(int tab_index);
	void _on_item_definitions_tab_pressed();
	void _on_recipes_tab_pressed();
	void _on_craft_station_types_tab_pressed();
	void _on_item_categories_tab_pressed();
	void _on_loots_tab_pressed();
	void _set_current_tab_and_update_buttons(int tab_index);

	void _remove_item_definition(const Ref<ItemDefinition> &p_item_def);
	void _duplicate_item_definition(const Ref<ItemDefinition> &p_item_def);
	void _remove_recipe(const Ref<Recipe> &p_recipe);
	void _duplicate_recipe(const Ref<Recipe> &p_recipe);
	void _remove_craft_station_type(const Ref<CraftStationType> &p_craft_station_type);
	void _duplicate_craft_station_type(const Ref<CraftStationType> &p_craft_station_type);
	void _remove_item_category(const Ref<ItemCategory> &p_item_category);
	void _duplicate_item_category(const Ref<ItemCategory> &p_item_category);
	void _remove_loot(const Ref<Loot> &p_loot);
	void _duplicate_loot(const Ref<Loot> &p_loot);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin *p_plugin);
	void edit_database(const Ref<InventoryDatabase> &p_database, const String &p_path = "");

	InventoryEditor();
	~InventoryEditor();
};

class InventoryEditorPlugin : public EditorPlugin {
	GDCLASS(InventoryEditorPlugin, EditorPlugin);

private:
	InventoryEditor *inventory_editor;

protected:
	static void _bind_methods();
	void _notification(int p_notification);

public:
	virtual bool _has_main_screen() const override { return true; }
	virtual String _get_plugin_name() const override { return "Inventory"; }
	virtual void _make_visible(bool p_visible) override;
	virtual void _edit(Object *p_object) override;
	virtual bool _handles(Object *p_object) const override;
	virtual Ref<Texture2D> _get_plugin_icon() const override;

	InventoryEditorPlugin();
	~InventoryEditorPlugin();
};

#endif // INVENTORY_EDITOR_PLUGIN_H

#endif // TOOLS_ENABLED