@tool
class_name ItemDefinitionPropertyEditor
extends HBoxContainer

signal removed(key : String)
signal changed(key : String, is_dynamic_property : bool, value)

var color_rect : ColorRect
var value_type : TextureRect
var line_edit : LineEdit
var no_compatible : Label
var value_bool : CheckBox
var value_integer : SpinBox
var value_float_line_edit : SpinBox
var value_line_edit : LineEdit
var value_color : ColorPicker
var dynamic_property : HBoxContainer
var check_box : CheckBox
var delete_button : Button
var remove_confirmation_dialog : ConfirmationDialog
var icons_name : Array[String] = ["null", "bool", "int", "float", "String", "Vector2", "Vector2i", "Rect2", "Rect2i", "Vector3", "Vector3i", "Transform2D", "Vector4", "Vector4i", "Plane", "Quaterion", "AABB", "Basis", "Transform3D", "Projection", "Color", "StringName", "NodePath", "RID", "Object", "Callable", "Signal", "Dictionary", "Array", "PackedByteArray", "PackedInt32Array", "PackedInt64Array", "PackedFloat32Array", "PackedFloat64Array", "PackedStringArray", "PackedVector2Array", "PackedVector3Array", "PackedColorArray"]
var key : String
var value
var is_dynamic_property : bool


func _ready():
	custom_minimum_size = Vector2(0, 32)
	size_flags_horizontal = Control.SIZE_EXPAND_FILL
	
	value_type = TextureRect.new()
	value_type.custom_minimum_size = Vector2(24, 24)
	value_type.layout_mode = 2
	value_type.stretch_mode = TextureRect.STRETCH_KEEP_CENTERED
	value_type.texture = get_theme_icon(icons_name[typeof(value)], "EditorIcons")
	add_child(value_type)
	
	var key_label = Label.new()
	key_label.layout_mode = 2
	key_label.text = "Key"
	add_child(key_label)
	
	line_edit = LineEdit.new()
	line_edit.custom_minimum_size = Vector2(64, 0)
	line_edit.layout_mode = 2
	line_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	line_edit.placeholder_text = "Key here..."
	line_edit.editable = false
	line_edit.caret_blink = true
	line_edit.caret_blink_interval = 0.5
	line_edit.text = key
	add_child(line_edit)
	
	var label_value = Label.new()
	label_value.layout_mode = 2
	label_value.text = "Value"
	add_child(label_value)
	
	var control : HBoxContainer = HBoxContainer.new()
	control.layout_mode = 2
	control.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	add_child(control)
	
	no_compatible = Label.new()
	no_compatible.visible = false
	no_compatible.layout_mode = 2
	no_compatible.tooltip_text = "Use the default inspector of godot to edit this item's value."
	no_compatible.text = "This type is not compatible with the item editor."
	control.add_child(no_compatible)
	
	value_bool = CheckBox.new()
	value_bool.visible = false
	value_bool.layout_mode = 2
	value_bool.button_pressed = true
	control.add_child(value_bool)
	
	value_integer = SpinBox.new()
	value_integer.visible = false
	value_integer.custom_minimum_size = Vector2(128, 0)
	value_integer.layout_mode = 2
	value_integer.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	value_integer.rounded = true
	value_integer.allow_greater = true
	value_integer.allow_lesser = true
	control.add_child(value_integer)
	
	value_float_line_edit = SpinBox.new()
	value_float_line_edit.visible = false
	value_float_line_edit.custom_minimum_size = Vector2(128, 0)
	value_float_line_edit.layout_mode = 2
	value_float_line_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	value_float_line_edit.step = 0.001
	value_float_line_edit.allow_greater = true
	value_float_line_edit.allow_lesser = true
	control.add_child(value_float_line_edit)
	
	value_line_edit = LineEdit.new()
	value_line_edit.visible = false
	value_line_edit.custom_minimum_size = Vector2(128, 0)
	value_line_edit.layout_mode = 2
	value_line_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	value_line_edit.placeholder_text = "Value here..."
	value_line_edit.caret_blink = true
	value_line_edit.caret_blink_interval = 0.5
	control.add_child(value_line_edit)
	
	value_color = ColorPicker.new()
	value_color.visible = false
	value_color.layout_mode = 2
	value_color.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	control.add_child(value_color)
	
	var dynamic_property : HBoxContainer = HBoxContainer.new()
	dynamic_property.layout_mode = 2
	add_child(dynamic_property)
	
	var dynamic_property_label : Label = Label.new()
	dynamic_property_label.layout_mode = 2
	dynamic_property_label.text = "Is Dynamic"
	dynamic_property.add_child(dynamic_property_label)
	
	check_box = CheckBox.new()
	check_box.layout_mode = 2
	check_box.button_pressed = is_dynamic_property
	dynamic_property.add_child(check_box)
	
	delete_button = Button.new()
	delete_button.layout_mode = 2
	delete_button.tooltip_text = "Delete"
	delete_button.flat = true
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.pressed.connect(_on_delete_button_pressed)
	add_child(delete_button)
		
	remove_confirmation_dialog = ConfirmationDialog.new()
	remove_confirmation_dialog.size = Vector2i(400, 100)
	remove_confirmation_dialog.dialog_text = "Remove?"
	
	add_child(remove_confirmation_dialog)
		
	match typeof(value):
		TYPE_BOOL:
			value_bool.button_pressed = value
			value_bool.visible = true
		TYPE_INT:
			value_integer.value = int(value)
			value_integer.visible = true
		TYPE_FLOAT:
			value_float_line_edit.value = value
			value_float_line_edit.visible = true
		TYPE_STRING:
			value_line_edit.text = value
			value_line_edit.visible = true
		TYPE_COLOR:
			value_color.color = value
			value_color.visible = true
		_:
			no_compatible.visible = true
	
	value_bool.toggled.connect(_on_value_bool_toggled)
	value_integer.value_changed.connect(_on_value_integer_value_changed)
	value_float_line_edit.value_changed.connect(_on_value_float_value_changed)
	value_line_edit.text_changed.connect(_on_value_line_edit_text_changed)
	value_color.color_changed.connect(_on_value_color_color_changed)
	check_box.toggled.connect(_on_dynamic_property_checkbox_toggled)
	remove_confirmation_dialog.confirmed.connect(_on_remove_confirmation_dialog_confirmed)

func setup(key : String, is_dynamic_property : bool, value):
	self.key = key
	self.value = value
	self.is_dynamic_property = is_dynamic_property

func _changed():
	changed.emit(key, is_dynamic_property, value)


func _on_dynamic_property_checkbox_toggled(toggled : bool):
	is_dynamic_property = toggled
	_changed()


func _on_delete_button_pressed():
	remove_confirmation_dialog.popup_centered()


func _on_remove_confirmation_dialog_confirmed():
	removed.emit(key)


func _on_value_line_edit_text_changed(new_text):
	value = new_text
	_changed()


func _on_value_float_value_changed(new_value):
	value = new_value
	_changed()


func _on_value_integer_value_changed(new_value):
	value = int(new_value)
	_changed()


func _on_value_bool_toggled(button_pressed):
	value = button_pressed
	_changed()


func _on_value_color_color_changed(color):
	value = color
	_changed()
