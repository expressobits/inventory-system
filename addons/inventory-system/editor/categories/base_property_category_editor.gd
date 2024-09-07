@tool
class_name BasePropertyCategoryEditor
extends HBoxContainer

signal removed

@onready var delete_button : Button = $DeleteButton
@onready var line_edit = $KeyLineEdit
@onready var value_line_edit = $Control/ValueString
@onready var value_float_line_edit = $Control/ValueFloat
@onready var value_integer = $Control/ValueInteger
@onready var value_type = $ValueType
@onready var value_bool : CheckBox = $Control/ValueBool
@onready var value_color = $Control/ValueColor
@onready var no_compatible = $Control/NoCompatible
@onready var dynamic_property = $DynamicProperty
@onready var check_box : CheckBox = $DynamicProperty/CheckBox

@onready var remove_confirmation_dialog = $RemoveConfirmationDialog
@onready var color_rect = $ColorRect

#@export var colors : Array[Color]
#@export var colors_in_light_theme : Array[Color]
@export var icons_name : Array[String]

var category : ItemCategory
var key : String
var value


func _ready():
	dynamic_property.visible = true
	var index = category.item_dynamic_properties.find(key)
	check_box.button_pressed = index != -1
	check_box.toggled.connect(_on_dynamic_property_checkbox_toggled)
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.tooltip_text = "Delete"
	line_edit.text = key
#	color_rect.color = colors[typeof(value)]
	value_type.texture = get_theme_icon(icons_name[typeof(value)], "EditorIcons")
	match typeof(value):
		TYPE_BOOL:
			value_bool.button_pressed = value
			value_bool.visible = true
		TYPE_INT:
			value_integer.value = value
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

func setup(category : ItemCategory, key : String, value):
	self.category = category
	self.key = key
	self.value = value


func _on_dynamic_property_checkbox_toggled(toggled : bool):
	var array = category.item_dynamic_properties.duplicate()
	if toggled:
		array.append(key)
	else:
		var index = category.item_dynamic_properties.find(key)
		if index == -1:
			return
		array.remove_at(index)
	category.item_dynamic_properties = array


func _on_delete_button_pressed():
	remove_confirmation_dialog.popup_centered()


func _on_remove_confirmation_dialog_confirmed():
	category.item_properties.erase(key)
	removed.emit()


func _on_value_line_edit_text_changed(new_text):
	category.item_properties[key] = new_text


func _on_value_float_value_changed(value):
	category.item_properties[key] = value


func _on_value_integer_value_changed(value):
	category.item_properties[key] = value


func _on_value_bool_toggled(button_pressed):
	category.item_properties[key] = button_pressed


func _on_value_color_color_changed(color):
	category.item_properties[key] = color
