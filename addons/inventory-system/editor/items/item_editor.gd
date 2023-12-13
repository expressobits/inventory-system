@tool
extends Control
class_name ItemEditor

signal changed(id : int)

var item : InventoryItem
var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var item_id_editor : ItemIDEditor = $ScrollContainer/MarginContainer/VBoxContainer/ItemIDEditor
@onready var item_name_text_edit : LineEdit = %ItemNameTextEdit
@onready var item_max_stack_spin_box : SpinBox = %MaxStackSpinBox
@onready var item_icon_text_edit : LineEdit = %IconLineEdit
@onready var item_icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var item_resource_text_edit : LineEdit = %ItemResourceLineEdit
@onready var item_resource_edit_button : Button = %ItemResourceEditButton
@onready var item_resource_file_dialog : FileDialog = $ItemResourceFileDialog
@onready var custom_properties : CustomPropertiesItemEditor = $ScrollContainer/MarginContainer/VBoxContainer/CustomProperties
@onready var weight_spin_box = $ScrollContainer/MarginContainer/VBoxContainer/Weight/WeightSpinBox
@onready var categories_in_item : CategoriesInItem = $ScrollContainer/MarginContainer/VBoxContainer/CategoriesInItem
@onready var can_stack_check_box : CheckBox = %CanStackCheckBox
@onready var max_stack = %MaxStack


func _ready():
	can_stack_check_box.toggled.connect(_can_stack_check_box_toggled.bind())
	apply_theme()
	$ScrollContainer.visible = false


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func load_item(item : InventoryItem, database : InventoryDatabase):
	self.item = item
	self.database = database
	if not is_instance_valid(item):
		$ScrollContainer.visible = false
		return
	item_id_editor.setup(database, item.id)
	if is_instance_valid(item):
		item_resource_text_edit.text = item.resource_path
		item_name_text_edit.text = item.name
		item_max_stack_spin_box.value = item.max_stack
		weight_spin_box.value = item.weight
		can_stack_check_box.button_pressed = item.can_stack
		if is_instance_valid(item.icon):
			item_icon_text_edit.text = item.icon.resource_path
		else:
			item_icon_text_edit.text = ""
		max_stack.visible = item.can_stack
		$ScrollContainer.visible = true
	else:
		item_resource_text_edit.text = "No resource path item!"
		item_name_text_edit.text = "No resource item!"
		$ScrollContainer.visible = false
		
	custom_properties.load_item(database, item)
	categories_in_item.load_item(database, item)


# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(item_icon_edit_button):
		return
	item_icon_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_icon_edit_button.tooltip_text = "Open Icon Texture2D"
	
	item_resource_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_resource_edit_button.tooltip_text = "Open Resource Inventory Item"
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	icon_file_dialog.min_size = Vector2(600, 500) * scale
	item_resource_file_dialog.min_size = Vector2(600, 500) * scale


func _on_max_stack_spin_box_value_changed(value):
	item.max_stack = int(item_max_stack_spin_box.value)
	changed.emit(item.id)


func _on_text_edit_text_changed(new_text):
	item.name = item_name_text_edit.text
	changed.emit(item.id)


func _on_theme_changed():
	apply_theme()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		return
	if file is Texture2D:
		var tex : Texture2D = file
		item.icon = tex
		item_icon_text_edit.text = item.icon.resource_path
		changed.emit(item.id)
	else:
		print("Error on open texture!")


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_item_resource_edit_button_pressed():
	item_resource_file_dialog.popup_centered()


func _on_item_resource_file_dialog_file_selected(path):
	var file = load(path)
	if file is InventoryItem:
		var inventory_item : InventoryItem = file
		load_item(item, database)
		changed.emit(item.id)
	else:
		print("Error on open scene!")


func _on_item_id_editor_changed(id : int):
	item.id = id
	database.update_items_cache()
	changed.emit(item.id)


func _on_weight_spin_box_value_changed(value):
	item.weight = weight_spin_box.value
	changed.emit(item.id)


func _can_stack_check_box_toggled(value):
	item.can_stack = value
	max_stack.visible = item.can_stack
	changed.emit(item.id)
	custom_properties.loading_properties()
