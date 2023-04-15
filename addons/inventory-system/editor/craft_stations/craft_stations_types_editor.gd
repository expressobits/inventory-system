@tool
extends Control
class_name CraftStationTypesEditor

var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var craft_station_type_editor : CraftStationTypeEditor = $HSplitContainer/CraftStationTypeEditor
@onready var new_craft_station_type_dialog : FileDialog = $NewCraftStationTypeResourceDialog
@onready var open_craft_station_type_dialog : FileDialog = $OpenCraftStationTypeDialog
@onready var craft_station_types_list : CraftStationTypesItemList = $HSplitContainer/CraftStationTypesItemList
@onready var craft_station_types_popup_menu : PopupMenu = $HSplitContainer/CraftStationTypesItemList/CraftStationTypesPopupMenu
@onready var craft_station_type_remove_confirmation_dialog = %CraftStationTypeRemoveConfirmationDialog
@onready var search_icon = $HSplitContainer/CraftStationTypesItemList/Control/SearchIcon


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	craft_station_type_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_craft_station_type_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_craft_station_type_dialog.min_size = Vector2(600, 500) * scale
	
	search_icon.texture = get_theme_icon("Search", "EditorIcons")
	

func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
	craft_station_type_editor.load_station(null)
	load_craft_station_types()


func select(station : CraftStationType):
	craft_station_type_editor.load_station(station)


func load_craft_station_types():
	craft_station_types_list.load_craft_station_types(database)


func _on_craft_station_types_item_list_station_selected(station):
	select(station)


func _on_craft_station_type_editor_changed(station):
	var index = craft_station_types_list.get_index_of(station)
	if index > -1:
		craft_station_types_list.update_item(index)
