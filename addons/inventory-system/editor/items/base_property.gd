@tool
extends HBoxContainer
class_name BasePropertyItemEditor

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

var item : InventoryItem
var key : String
var value


func _ready():
	dynamic_property.visible = not item.can_stack
	var index = item.dynamic_properties.find(key)
	check_box.button_pressed = index != -1
	check_box.toggled.connect(_on_dynamic_property_checkbox_toggled.bind())
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

func setup(item : InventoryItem,key : String, value):
	self.item = item
	self.key = key
	self.value = value


func _on_dynamic_property_checkbox_toggled(toggled : bool):
	var array = item.dynamic_properties.duplicate()
	if toggled:
		array.append(key)
	else:
		var index = item.dynamic_properties.find(key)
		if index == -1:
			return
		array.remove_at(index)
	item.dynamic_properties = array


func _on_delete_button_pressed():
	remove_confirmation_dialog.popup_centered()


func _on_remove_confirmation_dialog_confirmed():
	item.properties.erase(key)
	removed.emit()


func _on_value_line_edit_text_changed(new_text):
	item.properties[key] = new_text


func _on_value_float_value_changed(value):
	item.properties[key] = value


func _on_value_integer_value_changed(value):
	item.properties[key] = value


func _on_value_bool_toggled(button_pressed):
	item.properties[key] = button_pressed


func _on_value_color_color_changed(color):
	item.properties[key] = color
