@tool
extends EditorImportPlugin

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
	var file = FileAccess.open(source_file, FileAccess.READ)
	if file == null:
		return FileAccess.get_open_error()
	
	var json = ""
	while file.get_position() < file.get_length():
		json += file.get_line() + "\n"
		
	print(json)
		
	var database = InventoryDatabase.new()
	
	database.import_to_invdata(json)
	
	return ResourceSaver.save(database, "%s.%s" % [save_path, _get_save_extension()])
