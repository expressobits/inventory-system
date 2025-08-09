/**
 * base_resource_editor.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef BASE_RESOURCE_EDITOR_H
#define BASE_RESOURCE_EDITOR_H

#include "base_inventory_editor.h"
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class InventoryDatabase;

/**
 * Base class for individual resource editors (ItemDefinitionEditor, ItemCategoryEditor, etc.)
 * Provides common UI structure and signal management patterns.
 */
class BaseResourceEditor : public BaseInventoryEditor {
	GDCLASS(BaseResourceEditor, BaseInventoryEditor);

protected:
	// Common UI components that most editors use
	ScrollContainer* scroll_container;
	VBoxContainer* main_vbox;

	// Common initialization patterns
	virtual void _create_ui();
	virtual void _connect_signals() {}
	virtual void _disconnect_signals() {}

	static void _bind_methods();
	void _notification(int p_what);

public:
	BaseResourceEditor();
	virtual ~BaseResourceEditor();

	// Get access to common UI components
	ScrollContainer* get_scroll_container() const { return scroll_container; }
	VBoxContainer* get_main_vbox() const { return main_vbox; }
};

#endif // BASE_RESOURCE_EDITOR_H

#endif // TOOLS_ENABLED