@tool
class_name InventoryTabEditor
extends Control

signal data_changed

var database : InventoryDatabase
var editor_plugin : EditorPlugin
var current_data : Resource

const ITEM_COPY_RESOURCE_PATH = 100
const ITEM_REMOVE = 105
const ITEM_REMOVE_AND_DELETE = 106

@onready var new_resource_dialog : FileDialog = %NewResourceDialog
@onready var open_resource_dialog = $OpenResourceDialog
@onready var remove_confirmation_dialog = %RemoveConfirmationDialog
@onready var remove_and_delete_confirmation_dialog = %RemoveAndDeleteConfirmationDialog


static func get_name_of_resource(resource: Resource):
	return get_name_of_resource_path(resource.resource_path)


static func get_name_of_resource_path(path : String):
	return path.get_file().trim_suffix('.tres')


func _ready():
	new_resource_dialog.file_selected.connect(_on_new_resource_dialog_file_selected.bind())
	open_resource_dialog.file_selected.connect(_on_open_resource_dialog_file_selected.bind())
	remove_confirmation_dialog.confirmed.connect(_on_remove_confirmation_dialog_confirmed.bind())
	remove_and_delete_confirmation_dialog.confirmed.connect(_on_remove_and_delete_confirmation_dialog_confirmed.bind())


func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
	on_load_database()


func on_load_database():
	pass


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_resource_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	open_resource_dialog.min_size = Vector2(600, 500) * scale
	new_resource_dialog.min_size = Vector2(600, 500) * scale


func new_data_pressed():
	if not is_instance_valid(database):
		return
	
	new_resource_dialog.popup_centered()


func new_data_from_resource_pressed():
	if not is_instance_valid(database):
		return
	
	open_resource_dialog.popup_centered()


func remove_current_data():
	pass


func _on_new_resource_dialog_file_selected(path):
	pass # Replace with function body.


func _on_open_resource_dialog_file_selected(path):
	pass # Replace with function body.


func _on_remove_confirmation_dialog_confirmed():
	remove_current_data()


func _on_remove_and_delete_confirmation_dialog_confirmed():
	var dir = DirAccess.open(".")
	var code = dir.remove_absolute(current_data.resource_path)
	if code == OK:
		remove_current_data()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	current_data = null
