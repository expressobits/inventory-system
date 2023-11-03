@tool
extends EditorPlugin

const icon_plugin = preload("res://addons/inventory-system/icons/inventory_main_screen.svg")
const inventory_editor_scene = preload("res://addons/inventory-system/editor/inventory_editor.tscn")
const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")
const InventoryConstants = preload("res://addons/inventory-system/editor/constants.gd")

var _inventory_editor : Control

var database_file_cache: Dictionary = {}

var singleton_name : String = "InventorySystem"

func _enter_tree():
	if Engine.is_editor_hint():
		InventorySettings.prepare()
		
		_inventory_editor = inventory_editor_scene.instantiate()
		_inventory_editor.name = "Inventory System"
		_inventory_editor.set_editor_plugin(self)
		get_editor_interface().get_editor_main_screen().add_child(_inventory_editor)
		_make_visible(false)
	#	_inventory_editor.load_defaults()
	#	_inventory_editor.set_editor(self)
	#	_inventory_editor_plugin = preload("res://addons/inventory-system/editor/inventory-system-inspector.gd").new()
	#	add_inspector_plugin()
	add_autoload_singleton(singleton_name, "res://addons/inventory-system/core/inventory_system.tscn")	


func _exit_tree():
	if _inventory_editor:
		_inventory_editor.queue_free()
	remove_autoload_singleton(singleton_name)
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

