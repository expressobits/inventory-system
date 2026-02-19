@tool
extends HBoxContainer
class_name IconSelector

signal icon_changed(icon : Texture2D)

var texture_picker: EditorResourcePicker

func _ready() -> void:
	var label : Label = Label.new()
	label.custom_minimum_size = Vector2(64, 0)
	label.text = "Icon"
	label.layout_mode = 2
	add_child(label)
	
	texture_picker = EditorResourcePicker.new()
	texture_picker.base_type = "Texture2D"
	texture_picker.layout_mode = 2
	texture_picker.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	texture_picker.resource_changed.connect(_on_texture_picker_resource_changed)
	add_child(texture_picker)


func load_icon(icon : Texture2D):
	if is_instance_valid(icon):
		texture_picker.edited_resource = icon


func _on_texture_picker_resource_changed(resource: Resource):
	icon_changed.emit(resource)
