@tool
extends Control
class_name ItemInList

signal selected(id : int)

@onready var item_name_label : Label = $Content/HBoxContainer/Label
@onready var item_id_label : Label = $Content/HBoxContainer/Panel/IDLabel
@onready var item_icon_texture : TextureRect = $Content/HBoxContainer/TextureRect
@onready var panel : Panel = $Content
var item_database : InventoryDatabaseItem
var style_box : StyleBoxFlat


func _ready():
	style_box = StyleBoxFlat.new()
	style_box.set_corner_radius_all(4)
	_on_theme_changed()


func set_item(item_database : InventoryDatabaseItem) -> void:
	self.item_database = item_database
	var item = item_database.item
	item_name_label.text = item.name
	item_id_label.text = str(item_database.id)
	item_icon_texture.texture = item.icon


func _on_content_gui_input(event):
	if event is InputEventMouseButton:
		var event_mouse_button = event as InputEventMouseButton
		if event_mouse_button.button_index != 1:
			return
		if not event_mouse_button.pressed:
			return
		panel.add_theme_stylebox_override("panel", style_box)
		emit_signal("selected", item_database.id)


func unselect():
	panel.remove_theme_stylebox_override("panel")


func _on_theme_changed():
	if not is_instance_valid(style_box):
		return
	style_box.bg_color = get_theme_color("highlight_color","Editor")
