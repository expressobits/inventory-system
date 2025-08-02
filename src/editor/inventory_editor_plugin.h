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

using namespace godot;

class InventoryDatabase;
class InventoryItemListEditor;

class InventoryEditor : public Control {
	GDCLASS(InventoryEditor, Control);

private:
	enum DatabaseMenuId {
		DATABASE_NEW = 100,
		DATABASE_OPEN = 200,
		DATABASE_OPEN_RECENT = 250,
		DATABASE_SAVE = 300,
		DATABASE_IMPORT_JSON = 401,
		DATABASE_EXPORT_JSON = 501,
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
	MenuButton *database_button;
	Label *title_label;
	Button *new_item_button;
	Button *new_recipe_button;
	Button *new_craft_station_type_button;
	Button *new_item_categories_button;
	Control *content;
	TabContainer *tab_container;

	// Dialogs
	FileDialog *new_dialog;
	FileDialog *save_dialog;
	FileDialog *open_dialog;
	FileDialog *open_inv_dialog;
	FileDialog *save_inv_dialog;

	void _create_ui();
	void _apply_theme();
	void _build_database_menu();
	void _load_database(const Ref<InventoryDatabase> &p_database);
	void _new_file(const String &p_path);
	void _open_file(const String &p_path);
	void _save_file();
	void _import_inv_file(const String &p_path);

	void _on_database_menu_pressed();
	void _on_database_menu_id_pressed(int p_id);
	void _on_recent_menu_id_pressed(int p_id);
	void _on_new_dialog_file_selected(const String &p_path);
	void _on_open_dialog_file_selected(const String &p_path);
	void _on_save_inv_dialog_file_selected(const String &p_path);
	void _on_open_inv_dialog_file_selected(const String &p_path);
	void _on_new_item_button_pressed();
	void _on_new_recipe_button_pressed();
	void _on_new_craft_station_button_pressed();
	void _on_new_category_button_pressed();

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