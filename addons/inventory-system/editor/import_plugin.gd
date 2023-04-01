@tool
extends EditorImportPlugin


signal compiled_resource(resource: Resource)


const InventoryDatabaseResource = preload("res://addons/inventory-system/core/inventory_database.gd")
const compiler_version = 1


var editor_plugin


func _get_importer_name() -> String:
	# NOTE: A change to this forces a re-import of all dialogue
	return "inventory_system_database_compiler_%s" % compiler_version


func _get_visible_name() -> String:
	return "InventoryDatabase"


func _get_import_order() -> int:
	return -1000


func _get_priority() -> float:
	return 1000.0


func _get_resource_type():
	return "InventoryDatabase"


func _get_recognized_extensions() -> PackedStringArray:
	return PackedStringArray(["inv"])


func _get_save_extension():
	return "tres"


func _get_preset_count() -> int:
	return 0


func _get_preset_name(preset_index: int) -> String:
	return "Unknown"


func _get_import_options(path: String, preset_index: int) -> Array:
	return []


func _get_option_visibility(path: String, option_name: StringName, options: Dictionary) -> bool:
	return true


func _import(source_file: String, save_path: String, options: Dictionary, platform_variants: Array[String], gen_files: Array[String]) -> Error:
	return compile_file(source_file, "%s.%s" % [save_path, _get_save_extension()])


func compile_file(path: String, resource_path: String, will_cascade_cache_data: bool = true) -> Error:
	# Get the raw file contents
	if not FileAccess.file_exists(path): return ERR_FILE_NOT_FOUND

	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
#	var raw_text: String = file.get_as_text()

	# Get the current addon version
	var config: ConfigFile = ConfigFile.new()
	config.load("res://addons/inventory-system/plugin.cfg")
	var version: String = config.get_value("plugin", "version")

	# Save the results to a resource
	var resource : InventoryDatabase = InventoryDatabase.new()
	resource.set_meta("inventory_system_version", version)
	
#	Resource.new
	
	var item_database : InventoryDatabaseItem = InventoryDatabaseItem.new()
	item_database.id = 0
	item_database.item = preload("res://addons/inventory-system/demos/base/items/stone.tres")

	resource.items = [item_database]
	resource.recipes = []

#	if will_cascade_cache_data:
#		editor_plugin.add_to_dialogue_file_cache(path, resource_path, data)
	var err = ResourceSaver.save(resource, resource_path)

	emit_signal("compiled_resource", resource)

	return err
