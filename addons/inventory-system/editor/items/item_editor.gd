@tool
extends Control
class_name ItemEditor

signal changed(id : int)

var item : InventoryItem
var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var item_id_spin_box : SpinBox = %IDSpinBox
@onready var dropped_item_text_edit : LineEdit = %DroppedItemLineEdit
@onready var dropped_item_edit_button : Button = %DroppedItemEditButton
@onready var dropped_item_file_dialog : FileDialog = $DroppedItemFileDialog
@onready var item_name_text_edit : LineEdit = %ItemNameTextEdit
@onready var item_max_stack_spin_box : SpinBox = %MaxStackSpinBox
@onready var item_icon_text_edit : LineEdit = %IconLineEdit
@onready var item_icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var hand_item_text_edit : LineEdit = %HandItemLineEdit
@onready var hand_item_edit_button : Button = %HandItemEditButton
@onready var hand_item_file_dialog : FileDialog = $HandItemFileDialog
@onready var item_resource_text_edit : LineEdit = %ItemResourceLineEdit
@onready var item_resource_edit_button : Button = %ItemResourceEditButton
@onready var item_resource_file_dialog : FileDialog = $ItemResourceFileDialog

func _ready():
	apply_theme()
	$MarginContainer.visible = false


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func load_item(item : InventoryItem, database : InventoryDatabase):
	self.item = item
	self.database = database
	if not is_instance_valid(item):
		$MarginContainer.visible = false
		return
	item_id_spin_box.value = item.id
	if is_instance_valid(item):
		item_resource_text_edit.text = item.resource_path
		item_name_text_edit.text = item.name
		item_max_stack_spin_box.value = item.max_stack
		if is_instance_valid(item.icon):
			item_icon_text_edit.text = item.icon.resource_path
		else:
			item_icon_text_edit.text = ""
		if is_instance_valid(database.get_hand_item(item.id)):
			hand_item_text_edit.text = database.get_hand_item(item.id).resource_path
		else:
			hand_item_text_edit.text = ""
		if is_instance_valid(database.get_dropped_item(item.id)):
			dropped_item_text_edit.text = database.get_dropped_item(item.id).resource_path
		else:
			dropped_item_text_edit.text = ""
		$MarginContainer.visible = true
	else:
		item_resource_text_edit.text = "No resource path item!"
		item_name_text_edit.text = "No resource item!"
		$MarginContainer.visible = false


# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(item_icon_edit_button):
		return
	item_icon_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_icon_edit_button.tooltip_text = "Open Icon Texture2D"
	
	hand_item_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	hand_item_edit_button.tooltip_text = "Open PackedScene"
	
	dropped_item_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	dropped_item_edit_button.tooltip_text = "Open PackedScene"
	
	item_resource_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_resource_edit_button.tooltip_text = "Open Resource Inventory Item"
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	icon_file_dialog.min_size = Vector2(600, 500) * scale
	hand_item_file_dialog.min_size = Vector2(600, 500) * scale
	dropped_item_file_dialog.min_size = Vector2(600, 500) * scale
	item_resource_file_dialog.min_size = Vector2(600, 500) * scale


func _on_id_spin_box_value_changed(value):
	item.id = int(item_id_spin_box.value)
	emit_signal("changed", item.id)


func _on_max_stack_spin_box_value_changed(value):
	item.max_stack = int(item_max_stack_spin_box.value)
	emit_signal("changed", item.id)


func _on_text_edit_text_changed(new_text):
	item.name = item_name_text_edit.text
	emit_signal("changed", item.id)


func _on_theme_changed():
	apply_theme()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		print("Ok")
	if file is Texture2D:
		var tex : Texture2D = file
		item.icon = tex
		item_icon_text_edit.text = item.icon.resource_path
		emit_signal("changed", item.id)
	else:
		print("Error on open texture!")


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_hand_item_file_dialog_file_selected(path):
	var file = load(path)
	if file is PackedScene:
		var scene : PackedScene = file
		database.set_hand_item(item.id, scene)
		hand_item_text_edit.text = path
		emit_signal("changed", item.id)
	else:
		print("Error on open scene!")


func _on_dropped_item_file_dialog_file_selected(path):
	var file = load(path)
	if file is PackedScene:
		var scene : PackedScene = file
		database.set_dropped_item(item.id, scene)
		hand_item_text_edit.text = path
		emit_signal("changed", item.id)
	else:
		print("Error on open scene!")


func _on_hand_item_edit_button_pressed():
	hand_item_file_dialog.popup_centered()


func _on_dropped_item_edit_button_pressed():
	dropped_item_file_dialog.popup_centered()


func _on_item_resource_edit_button_pressed():
	item_resource_file_dialog.popup_centered()


func _on_item_resource_file_dialog_file_selected(path):
	var file = load(path)
	if file is InventoryItem:
		var inventory_item : InventoryItem = file
		load_item(item, database)
		emit_signal("changed", item.id)
	else:
		print("Error on open scene!")
