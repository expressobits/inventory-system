@tool
extends EditorPlugin

const icon_plugin = preload("res://addons/inventory-system/icons/inventory_main_screen.svg")
const inventory_editor_scene = preload("res://addons/inventory-system/editor/inventory_editor.tscn")
const InventoryImportPlugin = preload("res://addons/inventory-system/editor/import_plugin.gd")

var _inventory_editor : Control
var import_plugin : InventoryImportPlugin

func _enter_tree():
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
