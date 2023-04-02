@tool
extends EditorPlugin

const icon_plugin = preload("res://addons/inventory-system/icons/inventory_main_screen.svg")
const inventory_editor_scene = preload("res://addons/inventory-system/editor/inventory_editor.tscn")
const InventoryImportPlugin = preload("res://addons/inventory-system/editor/import_plugin.gd")
const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")
const InventoryConstants = preload("res://addons/inventory-system/editor/constants.gd")

var _inventory_editor : Control
var import_plugin : InventoryImportPlugin

var database_file_cache: Dictionary = {}

func _enter_tree():
	if Engine.is_editor_hint():
		InventorySettings.prepare()
		import_plugin = InventoryImportPlugin.new()
		import_plugin.editor_plugin = self
		add_import_plugin(import_plugin)
		
		_inventory_editor = inventory_editor_scene.instantiate()
		_inventory_editor.name = "Inventory System"
		_inventory_editor.editor_plugin = self
		get_editor_interface().get_editor_main_screen().add_child(_inventory_editor)
		_make_visible(false)
	#	_inventory_editor.load_defaults()
	#	_inventory_editor.set_editor(self)
	#	_inventory_editor_plugin = preload("res://addons/inventory-system/editor/inventory-system-inspector.gd").new()
	#	add_inspector_plugin()


func _exit_tree():
	remove_import_plugin(import_plugin)
	import_plugin = null
	
	if _inventory_editor:
		_inventory_editor.queue_free()
	# Clean-up of the plugin goes here.
#	remove_inspector_plugin()


func _has_main_screen():
	return true


func _make_visible(visible):
	if _inventory_editor:
		_inventory_editor.visible = visible


func _get_plugin_name():
	return "Inventory"


func _get_plugin_icon():
	return icon_plugin


func _build() -> bool:
	# Ignore errors in other files if we are just running the test scene
	if InventorySettings.get_user_value("is_running_test_scene", true): return true
	
	var can_build: bool = true
	var is_first_file: bool = true
	for database_file in database_file_cache.values():
		if database_file.errors.size() > 0:
			# Open the first file
			if is_first_file:
				get_editor_interface().edit_resource(load(database_file.path))
#				_inventory_editor.show_build_error_dialog()
				is_first_file = false
			push_error("You have %d error(s) in %s" % [database_file.errors.size(), database_file.path])
			can_build = false
	return can_build


## Make sure the cache points to real files
func update_dialogue_file_cache() -> void:
	var cache: Dictionary = {}
	
	# Open our cache file if it exists
	if FileAccess.file_exists(InventoryConstants.CACHE_PATH):
		var file: FileAccess = FileAccess.open(InventoryConstants.CACHE_PATH, FileAccess.READ)
		cache = JSON.parse_string(file.get_as_text())
	
	# Scan for dialogue files
	var current_files: PackedStringArray = _get_dialogue_files_in_filesystem()
	
	# Add any files to POT generation
	var files_for_pot: PackedStringArray = ProjectSettings.get_setting("internationalization/locale/translations_pot_files", [])
	var files_for_pot_changed: bool = false
	for path in current_files:
		if not files_for_pot.has(path):
			files_for_pot.append(path)
			files_for_pot_changed = true
	
	# Remove any files that don't exist any more
	for path in cache.keys():
		if not path in current_files:
			cache.erase(path)
			InventorySettings.remove_recent_file(path)
			
			# Remove missing files from POT generation
			if files_for_pot.has(path):
				files_for_pot.remove_at(files_for_pot.find(path))
				files_for_pot_changed = true
	
	# Update project settings if POT changed
	if files_for_pot_changed:
		ProjectSettings.set_setting("internationalization/locale/translations_pot_files", files_for_pot)
		ProjectSettings.save()
	
	database_file_cache = cache


## Recursively find any dialogue files in a directory
func _get_dialogue_files_in_filesystem(path: String = "res://") -> PackedStringArray:
	var files: PackedStringArray = []
	
	if DirAccess.dir_exists_absolute(path):
		var dir = DirAccess.open(path)
		dir.list_dir_begin()
		var file_name = dir.get_next()
		while file_name != "":
			var file_path: String = (path + "/" + file_name).simplify_path()
			if dir.current_is_dir():
				if not file_name in [".godot", ".tmp"]:
					files.append_array(_get_dialogue_files_in_filesystem(file_path))
			elif file_name.get_extension() == "tres":
				files.append(file_path)
			file_name = dir.get_next()
	
	return files
