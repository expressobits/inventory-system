@tool
class_name CraftStationTypeEditor
extends Control

signal changed(station : CraftStationType)

var station_index : int
var station : CraftStationType
var editor_plugin : EditorPlugin

@onready var name_text_edit : LineEdit = %NameTextEdit
@onready var icon_text_edit : LineEdit = %IconLineEdit
@onready var icon_edit_button : Button = %IconEditButton
@onready var icon_file_dialog : FileDialog = $IconFileDialog
@onready var craft_station_type_id_editor: CraftStationTypeIDEditor = $MarginContainer/VBoxContainer/CraftStationTypeIDEditor

func _ready():
	apply_theme()
	$MarginContainer.visible = false
	craft_station_type_id_editor.changed.connect(_on_craft_station_type_id_changed)


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func load_station(database : InventoryDatabase, station : CraftStationType):
	self.station = station
	if not is_instance_valid(station):
		$MarginContainer.visible = false
		return
	$MarginContainer.visible = true
	craft_station_type_id_editor.setup(database, station.id)
	name_text_edit.text = station.name
	if is_instance_valid(station.icon):
		icon_text_edit.text = station.icon.resource_path
	else:
		icon_text_edit.text = ""


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


func _on_craft_station_type_id_changed(id : String):
	station.id = id
	changed.emit(station)


func _on_name_text_edit_text_changed(new_text):
	station.name = new_text
	changed.emit(station)


func _on_icon_edit_button_pressed():
	icon_file_dialog.popup_centered()


func _on_icon_file_dialog_file_selected(path):
	var file = load(path)
	if file == null:
		return
	if file is Texture2D:
		var tex : Texture2D = file
		station.icon = tex
		icon_text_edit.text = station.icon.resource_path
		changed.emit(station)
	else:
		print("Error on open texture!")


#func _on_resource_file_dialog_file_selected(path):
#	var file = load(path)
#	if file is CraftStationType:
#		var craft_station_type : CraftStationType = file
#		database.stations[station_index] = craft_station_type
#		load_station(craft_station_type)
#		changed.emit(item_database.id)
#	else:
#		print("Error on open scene!")
