@tool
extends Control
class_name InventoryTabEditor

var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var new_resource_dialog : FileDialog = $NewResourceDialog
@onready var open_resource_dialog = $OpenResourceDialog
@onready var remove_confirmation_dialog = %RemoveConfirmationDialog
@onready var remove_and_delete_confirmation_dialog = %RemoveAndDeleteConfirmationDialog


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


func _on_new_resource_dialog_file_selected(path):
	pass # Replace with function body.


func _on_open_resource_dialog_file_selected(path):
	pass # Replace with function body.


func _on_remove_confirmation_dialog_confirmed():
	pass # Replace with function body.


func _on_remove_and_delete_confirmation_dialog_confirmed():
	pass # Replace with function body.
