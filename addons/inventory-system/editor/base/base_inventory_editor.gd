@tool
extends Control
class_name InventoryTabEditor

var database : InventoryDatabase
var editor_plugin : EditorPlugin

func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
	on_load_database()


func on_load_database():
	pass


func _apply_theme():
	pass
