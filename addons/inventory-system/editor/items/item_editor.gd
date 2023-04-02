@tool
extends Control
class_name ItemEditor

signal changed(id : int)

var item_database : InventoryDatabaseItem

@onready var item_id_spin_box : SpinBox = %IDSpinBox
@onready var item_name_text_edit : LineEdit = %ItemNameTextEdit
@onready var item_max_stack_spin_box : SpinBox = %MaxStackSpinBox
@onready var item_icon_text_edit : LineEdit = %IconLineEdit
@onready var item_icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var hand_item_text_edit : LineEdit = %HandItemLineEdit
@onready var hand_item_edit_button : Button = %HandItemEditButton
@onready var hand_item_file_dialog : FileDialog = $HandItemFileDialog
@onready var dropped_item_text_edit : LineEdit = %DroppedItemLineEdit
@onready var dropped_item_edit_button : Button = %DroppedItemEditButton
@onready var dropped_item_file_dialog : FileDialog = $DroppedItemFileDialog

func _ready():
	apply_theme()
	$MarginContainer.visible = false


func load_item(item_database : InventoryDatabaseItem):
	self.item_database = item_database
	item_id_spin_box.value = item_database.id
	if is_instance_valid(item_database.item):
		item_name_text_edit.text = item_database.item.name
		item_max_stack_spin_box.value = item_database.item.max_stack
		item_icon_text_edit.text = item_database.item.icon.resource_path
		hand_item_text_edit.text = item_database.hand_item.resource_path
		dropped_item_text_edit.text = item_database.dropped_item.resource_path
		$MarginContainer.visible = true
	else:
		item_name_text_edit.text = "No resource item!"
		$MarginContainer.visible = false


# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(item_icon_edit_button):
		return
	item_icon_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_icon_edit_button.tooltip_text = "Open Icon Texture2D"
	
	hand_item_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	hand_item_edit_button.tooltip_text = "Open PackedScene"
	
	dropped_item_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	dropped_item_edit_button.tooltip_text = "Open PackedScene"


func _on_id_spin_box_value_changed(value):
	item_database.id = int(item_id_spin_box.value)
	emit_signal("changed", item_database.id)


func _on_max_stack_spin_box_value_changed(value):
	item_database.item.max_stack = int(item_max_stack_spin_box.value)
	emit_signal("changed", item_database.id)


func _on_text_edit_text_changed(new_text):
	item_database.item.name = item_name_text_edit.text
	emit_signal("changed", item_database.id)


func _on_theme_changed():
	apply_theme()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		print("Ok")
	if file is Texture2D:
		var tex : Texture2D = file
		item_database.item.icon = tex
		item_icon_text_edit.text = item_database.item.icon.resource_path
		emit_signal("changed", item_database.id)
	else:
		print("Error on open texture!")


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_hand_item_file_dialog_file_selected(path):
	var file = load(path)
	if file is PackedScene:
		var scene : PackedScene = file
		item_database.hand_item = scene
		hand_item_text_edit.text = item_database.hand_item.resource_path
		emit_signal("changed", item_database.id)
	else:
		print("Error on open scene!")


func _on_dropped_item_file_dialog_file_selected(path):
	var file = load(path)
	if file is PackedScene:
		var scene : PackedScene = file
		item_database.dropped_item = scene
		hand_item_text_edit.text = item_database.dropped_item.resource_path
		emit_signal("changed", item_database.id)
	else:
		print("Error on open scene!")


func _on_hand_item_edit_button_pressed():
	hand_item_file_dialog.popup_centered()


func _on_dropped_item_edit_button_pressed():
	dropped_item_file_dialog.popup_centered()
