@tool
extends HBoxContainer
class_name IconSelector

signal icon_changed(icon : Texture2D)

var icon_edit_button : Button
var icon_text_edit : LineEdit
var icon_file_dialog : FileDialog 
var editor_plugin : EditorPlugin

func _init():
	icon_file_dialog = FileDialog.new()

func _ready() -> void:
	theme_changed.connect(apply_theme)
	
	var label : Label = Label.new()
	label.custom_minimum_size = Vector2(128, 0)
	label.text = "Icon"
	label.layout_mode = 2
	add_child(label)
	
	icon_edit_button = Button.new()
	icon_edit_button.unique_name_in_owner = true
	icon_edit_button.layout_mode = 2
	icon_edit_button.tooltip_text = "Open Icon Texture2D"
	icon_edit_button.icon_alignment = HorizontalAlignment.HORIZONTAL_ALIGNMENT_CENTER
	add_child(icon_edit_button)
	
	icon_text_edit = LineEdit.new()
	icon_text_edit.unique_name_in_owner = true
	icon_text_edit.layout_mode = 2
	icon_text_edit.editable = false
	icon_text_edit.tooltip_text = "Open Icon Texture2D"
	icon_text_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	add_child(icon_text_edit)
	
	icon_file_dialog.file_mode = FileDialog.FileMode.FILE_MODE_OPEN_FILE
	icon_file_dialog.auto_translate_mode = 1
	icon_file_dialog.filters.append("*.png ; Texture2D")
	icon_file_dialog.ok_button_text = "Open"
	icon_file_dialog.title = "Open a File"
	icon_file_dialog.size = Vector2(312, 175)
	icon_edit_button.pressed.connect(_on_icon_edit_button_pressed)
	icon_file_dialog.file_selected.connect(_on_icon_file_dialog_file_selected)
	add_child(icon_file_dialog)
	
	apply_theme()


func setup(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(icon_edit_button):
		return
	icon_edit_button.icon = get_theme_icon("Load", "EditorIcons")
	icon_edit_button.tooltip_text = "Open Icon Texture2D"
	
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	icon_file_dialog.min_size = Vector2(600, 500) * scale


func load_icon(icon : Texture2D):
	if is_instance_valid(icon):
		icon_text_edit.text = icon.resource_path
	else:
		icon_text_edit.text = ""


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		return
	if file is Texture2D:
		var tex : Texture2D = file
		icon_changed.emit(tex)
		icon_text_edit.text = tex.resource_path
	else:
		print("Error on open texture!")
