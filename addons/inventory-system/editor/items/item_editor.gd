@tool
extends Control
class_name ItemEditor


var item_database : InventoryDatabaseItem

@onready var item_name_text_edit : LineEdit = $MarginContainer/VBoxContainer/HBoxContainer/TextEdit


func load_item(item_database : InventoryDatabaseItem):
	self.item_database = item_database
	item_name_text_edit.text = item_database.item.name


func save_item():
	pass


func _on_text_edit_text_changed():
	print("_on_text_edit_text_changed")
	item_database.item.name = item_name_text_edit.text
