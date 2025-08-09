/**
 * base_resource_editor.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "base_resource_editor.h"
#include "../base/inventory_database.h"

using namespace godot;

void BaseResourceEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_create_ui"), &BaseResourceEditor::_create_ui);
	ClassDB::bind_method(D_METHOD("_connect_signals"), &BaseResourceEditor::_connect_signals);
	ClassDB::bind_method(D_METHOD("_disconnect_signals"), &BaseResourceEditor::_disconnect_signals);
}

BaseResourceEditor::BaseResourceEditor() {
	scroll_container = nullptr;
	main_vbox = nullptr;
	
	// Common size and anchor setup that most editors use
	set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	set_h_size_flags(Control::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

BaseResourceEditor::~BaseResourceEditor() {
}

void BaseResourceEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

void BaseResourceEditor::_create_ui() {
	// Create the common structure: ScrollContainer -> VBoxContainer
	// This is the pattern used by most resource editors
	scroll_container = memnew(ScrollContainer);
	add_child(scroll_container);
	scroll_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);

	main_vbox = memnew(VBoxContainer);
	scroll_container->add_child(main_vbox);
	main_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
}

#endif // TOOLS_ENABLED