/**
 * inventory_settings.h
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#ifndef INVENTORY_SETTINGS_H
#define INVENTORY_SETTINGS_H

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class InventorySettings : public Object {
	GDCLASS(InventorySettings, Object);

private:
	static const String USER_CONFIG_PATH;

	static Dictionary get_user_config();
	static void save_user_config(const Dictionary &p_config);

protected:
	static void _bind_methods();

public:
	static void set_user_value(const String &p_key, const Variant &p_value);
	static Variant get_user_value(const String &p_key, const Variant &p_default = Variant());
	
	static void add_recent_file(const String &p_path);
	static void move_recent_file(const String &p_from_path, const String &p_to_path);
	static void remove_recent_file(const String &p_path);
	static Array get_recent_files();
	static void clear_recent_files();

	InventorySettings();
	~InventorySettings();
};

#endif // INVENTORY_SETTINGS_H

#endif // TOOLS_ENABLED