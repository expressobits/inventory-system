@tool
class_name CraftStationTypeEditor
extends Control

signal changed(station : CraftStationType)

var station_index : int
var station : CraftStationType
var editor_plugin : EditorPlugin

@onready var name_text_edit : LineEdit = %NameTextEdit
@onready var craft_station_type_id_editor: CraftStationTypeIDEditor = $MarginContainer/VBoxContainer/CraftStationTypeIDEditor
@onready var icon_selector : IconSelector = %IconSelector

func _ready():
	$MarginContainer.visible = false
	craft_station_type_id_editor.changed.connect(_on_craft_station_type_id_changed)
	icon_selector.icon_changed.connect(_on_icon_selector_changed)


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


func load_station(database : InventoryDatabase, station : CraftStationType):
	self.station = station
	if not is_instance_valid(station):
		$MarginContainer.visible = false
		return
	$MarginContainer.visible = true
	craft_station_type_id_editor.setup(database, station.id)
	name_text_edit.text = station.name
	icon_selector.load_icon(station.icon)


func _on_icon_selector_changed(icon : Texture2D):
	station.icon = icon
	changed.emit(station)


func _on_craft_station_type_id_changed(id : String):
	station.id = id
	changed.emit(station)


func _on_name_text_edit_text_changed(new_text):
	station.name = new_text
	changed.emit(station)
