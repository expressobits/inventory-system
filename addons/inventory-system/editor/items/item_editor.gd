@tool
extends Control
class_name ItemEditor

signal changed(id : int)

var item_database : InventoryDatabaseItem

@onready var item_id_spin_box : SpinBox = $MarginContainer/VBoxContainer/ItemID/IDSpinBox
@onready var item_name_text_edit : LineEdit = $MarginContainer/VBoxContainer/HBoxContainer/TextEdit
@onready var item_max_stack_spin_box : SpinBox = $MarginContainer/VBoxContainer/MaxStack/MaxStackSpinBox

func load_item(item_database : InventoryDatabaseItem):
	self.item_database = item_database
	item_id_spin_box.value = item_database.id
	if is_instance_valid(item_database.item):
		item_name_text_edit.text = item_database.item.name
		item_max_stack_spin_box.value = item_database.item.max_stack
	else:
		item_name_text_edit.text = "No resource item!"


func _on_id_spin_box_value_changed(value):
	item_database.id = int(item_id_spin_box.value)
	emit_signal("changed", item_database.id)


func _on_max_stack_spin_box_value_changed(value):
	item_database.item.max_stack = int(item_max_stack_spin_box.value)
	emit_signal("changed", item_database.id)


func _on_text_edit_text_changed(new_text):
	item_database.item.name = item_name_text_edit.text
	emit_signal("changed", item_database.id)
