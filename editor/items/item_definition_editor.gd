@tool
class_name ItemDefinitionEditor
extends Control

signal changed(id : String)

var item : ItemDefinition
var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var item_id_editor : ItemIDEditor = %ItemIDEditor
@onready var item_name_text_edit : LineEdit = %ItemNameTextEdit
@onready var item_max_stack_spin_box : SpinBox = %MaxStackSpinBox
@onready var icon_selector : IconSelector = %IconSelector
@onready var custom_properties : CustomPropertiesItemEditor = $ScrollContainer/MarginContainer/VBoxContainer/CustomProperties
@onready var weight_spin_box = %WeightSpinBox
@onready var categories_in_item : CategoriesInItem = $ScrollContainer/MarginContainer/VBoxContainer/CategoriesInItem
@onready var can_stack_check_box : CheckBox = %CanStackCheckBox
@onready var max_stack = %MaxStack
@onready var size_x_spin_box: SpinBox = %SizeXSpinBox
@onready var size_y_spin_box: SpinBox = %SizeYSpinBox


func _ready():
	can_stack_check_box.toggled.connect(_can_stack_check_box_toggled)
	$ScrollContainer.visible = false
	icon_selector.icon_changed.connect(_on_icon_changed)


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


func load_item(item : ItemDefinition, database : InventoryDatabase):
	if size_x_spin_box.value_changed.is_connected(_on_size_x_spin_box_value_changed):
		size_x_spin_box.value_changed.disconnect(_on_size_x_spin_box_value_changed)
	if size_y_spin_box.value_changed.is_connected(_on_size_y_spin_box_value_changed):
		size_y_spin_box.value_changed.disconnect(_on_size_y_spin_box_value_changed)
	
	$ScrollContainer.visible = false
	await get_tree().create_timer(0.2).timeout
	
	self.item = item
	self.database = database
	if not is_instance_valid(item):
		$ScrollContainer.visible = false
		return
	item_id_editor.setup(database, item.id)
	if is_instance_valid(item):
		item_name_text_edit.text = item.name
		item_max_stack_spin_box.value = item.max_stack
		weight_spin_box.value = item.weight
		can_stack_check_box.button_pressed = item.can_stack
		icon_selector.load_icon(item.icon)
		max_stack.visible = item.can_stack
		size_x_spin_box.value = item.size.x
		size_y_spin_box.value = item.size.y
		size_x_spin_box.value_changed.connect(_on_size_x_spin_box_value_changed.bind(item))
		size_y_spin_box.value_changed.connect(_on_size_y_spin_box_value_changed.bind(item))
		$ScrollContainer.visible = true
	else:
		item_name_text_edit.text = "No resource item!"
		$ScrollContainer.visible = false
		
	custom_properties.load_item(database, item)
	categories_in_item.load_item(database, item)
	
	size_x_spin_box.editable = true

func reload_item():
	load_item(item, database)


func _on_max_stack_spin_box_value_changed(value):
	item.max_stack = int(item_max_stack_spin_box.value)
	changed.emit(item.id)


func _on_text_edit_text_changed(new_text):
	item.name = item_name_text_edit.text
	changed.emit(item.id)


func _on_icon_changed(icon : Texture2D):
	item.icon = icon
	changed.emit(item.id)


func _on_item_resource_file_dialog_file_selected(path):
	var file = load(path)
	if file is ItemDefinition:
		var inventory_item : ItemDefinition = file
		load_item(item, database)
		changed.emit(item.id)
	else:
		print("Error on open scene!")


func _on_item_id_editor_changed(id : String):
	item.id = id
	changed.emit(item.id)


func _on_weight_spin_box_value_changed(value):
	item.weight = weight_spin_box.value
	changed.emit(item.id)


func _can_stack_check_box_toggled(value):
	item.can_stack = value
	max_stack.visible = item.can_stack
	changed.emit(item.id)
	custom_properties.loading_properties()


func _on_categories_in_item_changed():
	changed.emit(item.id)
	reload_item()


func _on_size_x_spin_box_value_changed(value: float, item : ItemDefinition) -> void:
	item.size = Vector2i(value, item.size.y)
	changed.emit(item.id)


func _on_size_y_spin_box_value_changed(value: float, item : ItemDefinition) -> void:
	item.size = Vector2i(item.size.x, value)
	changed.emit(item.id)
