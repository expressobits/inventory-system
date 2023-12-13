@tool
extends HBoxContainer
class_name ItemIDEditor

signal changed(id : int)

@onready var id_spin_box : SpinBox = %IDSpinBox
@onready var button : Button = $Button
@export var ids_must_exist_in_database := false

var database : InventoryDatabase
var id : int

func _ready():
	_apply_theme()


func _apply_theme() -> void:
	if not is_instance_valid(button):
		return
	button.icon = get_theme_icon("Edit", "EditorIcons")
	button.tooltip_text = "Edit"


func setup(database : InventoryDatabase, id : int):
	self.database = database
	self.id = id
	id_spin_box.value = id
	id_spin_box.editable = false
	button.disabled = false
	id_spin_box.tooltip_text = ""
	button.tooltip_text = "Edit"
	id_spin_box.modulate = Color.WHITE


func _on_button_pressed():
	id_spin_box.editable = !id_spin_box.editable
	var new_id = int(id_spin_box.value)
	if id_spin_box.editable:
		button.tooltip_text = "Confirm"
		button.icon = get_theme_icon("ImportCheck", "EditorIcons")
	else:
		button.tooltip_text = "Edit"
		button.icon = get_theme_icon("Edit", "EditorIcons")
		if self.id != new_id:
			self.id = new_id
			changed.emit(id)
			

func _check_valid_id():
	var new_id = int(id_spin_box.value)
	var valid : bool
	var warn_text : String 
	if ids_must_exist_in_database:
		valid = new_id == self.id or database.has_item_id(new_id)
		warn_text = "Item id does not exist in database!"
	else:
		valid = new_id == self.id or not database.has_item_id(new_id)
		warn_text = "Item id already exists in database!"
	if valid:
		id_spin_box.tooltip_text = ""
		button.tooltip_text = "Confirm"
		id_spin_box.modulate = Color.WHITE
	else:
		id_spin_box.tooltip_text = warn_text
		button.tooltip_text = warn_text
		id_spin_box.modulate = Color.INDIAN_RED
	button.disabled = not valid


func _on_theme_changed():
	_apply_theme()


func _on_id_spin_box_value_changed(value):
	_check_valid_id()
