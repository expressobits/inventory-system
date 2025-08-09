/**
 * craft_station_type_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef CRAFT_STATION_TYPE_EDITOR_H
#define CRAFT_STATION_TYPE_EDITOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include "base_resource_editor.h"
#include "icon_selector.h"
#include "resource_id_editor.h"

using namespace godot;

class InventoryDatabase;
class CraftStationType;

class CraftStationTypeEditor : public BaseResourceEditor {
	GDCLASS(CraftStationTypeEditor, BaseResourceEditor);

private:
	Ref<CraftStationType> craft_station_type;
	InventoryDatabase* database;
	EditorPlugin* editor_plugin;

	// Form components
	ResourceIDEditor* resource_id_editor;
	LineEdit* name_edit;
	IconSelector* icon_selector;

	void _create_ui() override;
	void _connect_signals() override;
	void _disconnect_signals() override;

	// Signal handlers - matching addon method names exactly
	void _on_resource_id_editor_changed(const String& id);
	void _on_name_changed(const String& text);
	void _on_name_focus_exited();
	void _on_icon_changed(const Ref<Texture2D>& icon);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_editor_plugin(EditorPlugin* p_editor_plugin);
	void load_craft_station_type(const Ref<CraftStationType>& p_craft_station_type, InventoryDatabase* p_database);
	void reload_craft_station_type();

	CraftStationTypeEditor();
	~CraftStationTypeEditor();
};

#endif // CRAFT_STATION_TYPE_EDITOR_H

#endif // TOOLS_ENABLED