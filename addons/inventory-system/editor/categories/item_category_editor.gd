@tool
class_name ItemCategoryEditor
extends Control

signal changed(category : ItemCategory)

var category_index : int
var category : ItemCategory
var editor_plugin : EditorPlugin

@onready var id_editor : ItemCategoryIDEditor = %IDEditor
@onready var name_text_edit : LineEdit = %NameTextEdit
@onready var icon_selector : IconSelector = %IconSelector
@onready var custom_properties : CustomPropertiesCategoryEditor = $MarginContainer/VBoxContainer/CategoryCustomProperties
@onready var color_picker = %ColorPicker


func _ready():
	$MarginContainer.visible = false
	id_editor.changed.connect(_on_id_editor_changed)
	icon_selector.icon_changed.connect(_on_icon_changed)
	

func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


func load_category(database : InventoryDatabase, category : ItemCategory):
	self.category = category
	if not is_instance_valid(category):
		$MarginContainer.visible = false
		return
	id_editor.setup(database, category.id)
	$MarginContainer.visible = true
	name_text_edit.text = category.name
	color_picker.color = category.color
	icon_selector.load_icon(category.icon)
	custom_properties.load_item(category)


func _on_id_editor_changed(id : String):
	category.id = id
	changed.emit(category)


func _on_name_text_edit_text_changed(new_text):
	category.name = new_text
	changed.emit(category)


func _on_icon_changed(icon : Texture2D):
	category.icon = icon
	changed.emit(category)


func _on_color_picker_color_changed(color):
	category.color = color
	changed.emit(category)
