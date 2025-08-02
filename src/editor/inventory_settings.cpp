/**
 * inventory_settings.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "inventory_settings.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

using namespace godot;

void InventorySettings::_bind_methods() {
	ClassDB::bind_static_method("InventorySettings", D_METHOD("set_user_value", "key", "value"), &InventorySettings::set_user_value);
	ClassDB::bind_static_method("InventorySettings", D_METHOD("get_user_value", "key", "default"), &InventorySettings::get_user_value, DEFVAL(Variant()));
	ClassDB::bind_static_method("InventorySettings", D_METHOD("add_recent_file", "path"), &InventorySettings::add_recent_file);
	ClassDB::bind_static_method("InventorySettings", D_METHOD("move_recent_file", "from_path", "to_path"), &InventorySettings::move_recent_file);
	ClassDB::bind_static_method("InventorySettings", D_METHOD("remove_recent_file", "path"), &InventorySettings::remove_recent_file);
	ClassDB::bind_static_method("InventorySettings", D_METHOD("get_recent_files"), &InventorySettings::get_recent_files);
	ClassDB::bind_static_method("InventorySettings", D_METHOD("clear_recent_files"), &InventorySettings::clear_recent_files);
}

InventorySettings::InventorySettings() {
}

InventorySettings::~InventorySettings() {
}

Dictionary InventorySettings::get_user_config() {
	Dictionary user_config;
	user_config["recent_files"] = Array();
	
	if (FileAccess::file_exists("user://inventory_system_user_config.json")) {
		Ref<FileAccess> file = FileAccess::open("user://inventory_system_user_config.json", FileAccess::READ);
		if (file.is_valid()) {
			String content = file->get_as_text();
			Ref<JSON> json;
			json.instantiate();
			Error error = json->parse(content);
			if (error == OK) {
				Variant result = json->get_data();
				if (result.get_type() == Variant::DICTIONARY) {
					Dictionary loaded_config = result;
					user_config.merge(loaded_config, true);
				}
			}
		}
	}
	
	return user_config;
}

void InventorySettings::save_user_config(const Dictionary &p_config) {
	Ref<FileAccess> file = FileAccess::open("user://inventory_system_user_config.json", FileAccess::WRITE);
	if (file.is_valid()) {
		Ref<JSON> json;
		json.instantiate();
		String json_string = json->stringify(p_config);
		file->store_string(json_string);
	}
}

void InventorySettings::set_user_value(const String &p_key, const Variant &p_value) {
	Dictionary user_config = get_user_config();
	user_config[p_key] = p_value;
	save_user_config(user_config);
}

Variant InventorySettings::get_user_value(const String &p_key, const Variant &p_default) {
	Dictionary user_config = get_user_config();
	return user_config.get(p_key, p_default);
}

void InventorySettings::add_recent_file(const String &p_path) {
	Array recent_files = get_user_value("recent_files", Array());
	
	// Remove if already exists
	int existing_index = recent_files.find(p_path);
	if (existing_index >= 0) {
		recent_files.remove_at(existing_index);
	}
	
	// Add to front
	recent_files.insert(0, p_path);
	
	// Limit to reasonable number
	while (recent_files.size() > 10) {
		recent_files.remove_at(recent_files.size() - 1);
	}
	
	set_user_value("recent_files", recent_files);
}

void InventorySettings::move_recent_file(const String &p_from_path, const String &p_to_path) {
	Array recent_files = get_user_value("recent_files", Array());
	
	for (int i = 0; i < recent_files.size(); i++) {
		if (recent_files[i] == p_from_path) {
			recent_files[i] = p_to_path;
			break;
		}
	}
	
	set_user_value("recent_files", recent_files);
}

void InventorySettings::remove_recent_file(const String &p_path) {
	Array recent_files = get_user_value("recent_files", Array());
	
	int index = recent_files.find(p_path);
	if (index >= 0) {
		recent_files.remove_at(index);
		set_user_value("recent_files", recent_files);
	}
}

Array InventorySettings::get_recent_files() {
	return get_user_value("recent_files", Array());
}

void InventorySettings::clear_recent_files() {
	set_user_value("recent_files", Array());
}

#endif // TOOLS_ENABLED