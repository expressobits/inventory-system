@tool
extends EditorImportPlugin

var editor_plugin : InventorySystemEditorPlugin

enum Presets { DEFAULT }

func _get_import_order() -> int:
	return 0

func _get_priority() -> float:
	return 1.0

func _get_preset_count():
	return Presets.size()
	
	
func _get_preset_name(preset_index):
	match preset_index:
		Presets.DEFAULT:
			return "Default"
		_:
			return "Unknown"


func _get_import_options(path, preset_index):
	match preset_index:
		Presets.DEFAULT:
			return []
		_:
			return []


func _get_importer_name():
	return "inventory_system.items"


func _get_visible_name():
	return "Inventory Database"


func _get_recognized_extensions():
	return ["inv"]
	
	
func _get_save_extension():
	return "tres"
	

func _get_resource_type():
	return "InventoryDatabase"
	
	
func _get_option_visibility(path, option_name, options):
	return true
	
	
func _import(source_file, save_path, options, r_platform_variants, r_gen_files):
	var database = InventoryDatabase.new()
	var error = editor_plugin.import_from_inv_file(database, save_path)
	if error != OK:
		return
	return ResourceSaver.save(database, "%s.%s" % [save_path, _get_save_extension()])
