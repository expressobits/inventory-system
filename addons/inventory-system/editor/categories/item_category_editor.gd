@tool
class_name ItemCategoryEditor
extends Control

signal changed(category : ItemCategory)

var category_index : int
var category : ItemCategory
var editor_plugin : EditorPlugin

@onready var id_editor : ItemCategoryIDEditor = %IDEditor
@onready var name_text_edit : LineEdit = %NameTextEdit
@onready var icon_text_edit : LineEdit = %IconLineEdit
@onready var icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var custom_properties : CustomPropertiesCategoryEditor = $MarginContainer/VBoxContainer/CategoryCustomProperties
@onready var color_picker = %ColorPicker


func _ready():
	apply_theme()
	$MarginContainer.visible = false
	id_editor.changed.connect(_on_id_editor_changed)
	


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func load_category(database : InventoryDatabase, category : ItemCategory):
	self.category = category
	if not is_instance_valid(category):
		$MarginContainer.visible = false
		return
	id_editor.setup(database, category.id)
	$MarginContainer.visible = true
	name_text_edit.text = category.name
	color_picker.color = category.color
	if is_instance_valid(category.icon):
		icon_text_edit.text = category.icon.resource_path
	else:
		icon_text_edit.text = ""
	custom_properties.load_item(category)


func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(icon_edit_button):
		return
	icon_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	icon_edit_button.tooltip_text = "Open Icon Texture2D"
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	icon_file_dialog.min_size = Vector2(600, 500) * scale


func _on_theme_changed():
	apply_theme()


func _on_id_editor_changed(id : String):
	category.id = id
	changed.emit(category)


func _on_name_text_edit_text_changed(new_text):
	category.name = new_text
	changed.emit(category)


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		return
	if file is Texture2D:
		var tex : Texture2D = file
		category.icon = tex
		icon_text_edit.text = category.icon.resource_path
		changed.emit(category)
	else:
		print("Error on open texture!")


func _on_color_picker_color_changed(color):
	category.color = color
	changed.emit(category)
