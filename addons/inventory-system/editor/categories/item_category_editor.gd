@tool
extends Control
class_name ItemCategoryEditor

signal changed(category : ItemCategory)

var category_index : int
var category : ItemCategory
var editor_plugin : EditorPlugin

@onready var name_text_edit : LineEdit = %NameTextEdit
@onready var icon_text_edit : LineEdit = %IconLineEdit
@onready var icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var resource_text_edit : LineEdit = %ResourceLineEdit
@onready var resource_edit_button : Button = %ResourceEditButton
@onready var resource_file_dialog : FileDialog = $ResourceFileDialog
@onready var color_picker = %ColorPicker


func _ready():
	apply_theme()
	$MarginContainer.visible = false


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func load_category(category : ItemCategory):
	self.category = category
	if not is_instance_valid(category):
		$MarginContainer.visible = false
		return
	$MarginContainer.visible = true
	resource_text_edit.text = category.resource_path
	name_text_edit.text = category.name
	color_picker.color = category.color
	if is_instance_valid(category.icon):
		icon_text_edit.text = category.icon.resource_path
	else:
		icon_text_edit.text = ""


func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(icon_edit_button):
		return
	icon_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	icon_edit_button.tooltip_text = "Open Icon Texture2D"

	resource_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	resource_edit_button.tooltip_text = "Open Resource Craft Station Type"
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	icon_file_dialog.min_size = Vector2(600, 500) * scale
	resource_file_dialog.min_size = Vector2(600, 500) * scale


func _on_theme_changed():
	apply_theme()


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
