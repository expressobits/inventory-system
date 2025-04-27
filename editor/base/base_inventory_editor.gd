@tool
class_name BaseInventoryEditor
extends Control

signal data_changed
signal removed(data : Resource)
signal duplicated(data : Resource)

var database : InventoryDatabase
var editor_plugin : EditorPlugin
var current_data : Resource

const ITEM_REMOVE = 105
const ITEM_DUPLICATE = 106

@onready var remove_confirmation_dialog = %RemoveConfirmationDialog


static func get_name_of_resource(resource: Resource):
	return get_name_of_resource_path(resource.resource_path)


static func get_name_of_resource_path(path : String):
	return path.get_file().trim_suffix('.tres')


func _ready():
	remove_confirmation_dialog.confirmed.connect(_on_remove_confirmation_dialog_confirmed)


func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
	on_load_database()


func on_load_database():
	pass


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(remove_confirmation_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale() 	


func remove_current_data() -> bool:
	if current_data != null:
		removed.emit(current_data)
		return true
	return false


func duplicate_current_data():
	if current_data != null:
		duplicated.emit(current_data)


func _on_remove_confirmation_dialog_confirmed():
	remove_current_data()
