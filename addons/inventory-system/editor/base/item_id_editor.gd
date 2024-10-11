@tool
class_name ItemIDEditor
extends HBoxContainer

signal changed(id : String)

var id_line_edit : LineEdit
var button : Button
var label : Label
@export var ids_must_exist_in_database := false

var database : InventoryDatabase
var id : String

func _ready():
	offset_right = 241.0
	offset_bottom = 32.0
	
	label = Label.new()
	label.layout_mode = 2
	label.text = "ID"
	label.custom_minimum_size = Vector2(128, 0)
	add_child(label)
	
	button = Button.new()
	button.layout_mode = 2
	button.tooltip_text = "Edit"
	button.pressed.connect(_on_button_pressed)
	add_child(button)
	
	id_line_edit = LineEdit.new()
	id_line_edit.custom_minimum_size = Vector2(128, 0)
	id_line_edit.layout_mode = 2
	id_line_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	id_line_edit.placeholder_text = "Place ID here"
	id_line_edit.text_changed.connect(_on_id_value_changed)
	add_child(id_line_edit)
	
	
	theme_changed.connect(_on_theme_changed)
	_apply_theme()


func _apply_theme() -> void:
	if not is_instance_valid(button):
		return
	button.icon = get_theme_icon("Edit", "EditorIcons")
	button.tooltip_text = "Edit"


func setup(database : InventoryDatabase, id : String):
	self.database = database
	self.id = id
	id_line_edit.text = id
	id_line_edit.editable = false
	button.disabled = false
	id_line_edit.tooltip_text = ""
	button.tooltip_text = "Edit"
	id_line_edit.modulate = Color.WHITE


func _on_button_pressed():
	id_line_edit.editable = !id_line_edit.editable
	var new_id : String = id_line_edit.text
	if id_line_edit.editable:
		button.tooltip_text = "Confirm"
		button.icon = get_theme_icon("ImportCheck", "EditorIcons")
	else:
		button.tooltip_text = "Edit"
		button.icon = get_theme_icon("Edit", "EditorIcons")
		if self.id != new_id:
			self.id = new_id
			changed.emit(id)
			

func _check_valid_id():
	var new_id : String = id_line_edit.text
	var valid : bool
	var warn_text : String 
	if ids_must_exist_in_database:
		valid = new_id == self.id or has_in_database(new_id)
		warn_text = "Item id does not exist in database!"
	else:
		valid = new_id == self.id or not has_in_database(new_id)
		warn_text = "Item id already exists in database!"
	if valid:
		id_line_edit.tooltip_text = ""
		button.tooltip_text = "Confirm"
		id_line_edit.modulate = Color.WHITE
	else:
		id_line_edit.tooltip_text = warn_text
		button.tooltip_text = warn_text
		id_line_edit.modulate = Color.INDIAN_RED
	button.disabled = not valid


func has_in_database(id : String) -> bool:
	return database.has_item_id(id)


func _on_theme_changed():
	_apply_theme()


func _on_id_value_changed(value):
	_check_valid_id()
