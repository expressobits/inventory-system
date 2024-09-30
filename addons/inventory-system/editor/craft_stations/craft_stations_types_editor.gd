@tool
class_name CraftStationTypesEditor
extends InventoryTabEditor

@onready var craft_station_type_editor : CraftStationTypeEditor = $HSplitContainer/CraftStationTypeEditor
@onready var craft_station_types_list : CraftStationTypesItemList = $HSplitContainer/CraftStationTypesItemList
@onready var craft_station_types_popup_menu : PopupMenu = $CraftStationTypesPopupMenu
@onready var search_icon = $HSplitContainer/CraftStationTypesItemList/Control/SearchIcon


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	craft_station_type_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func _apply_theme():
	super._apply_theme()
	if not is_instance_valid(search_icon):
		return
	search_icon.texture = get_theme_icon("Search", "EditorIcons")
	

func on_load_database() -> void:
	craft_station_type_editor.load_station(database, null)
	load_craft_station_types()


func select(station : CraftStationType):
	craft_station_type_editor.load_station(database, station)


func load_craft_station_types():
	craft_station_types_list.load_craft_station_types(database)


func _on_craft_station_types_item_list_station_selected(station):
	current_data = station
	select(station)


func _on_craft_station_type_editor_changed(station):
	var index = craft_station_types_list.get_index_of(station)
	if index > -1:
		craft_station_types_list.update_item(index)


func _on_craft_station_types_popup_menu_id_pressed(id):
	match id:
		ITEM_REMOVE:
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Craft Station Type \""+current_data.name+"\"?"


func _on_craft_station_types_item_list_item_popup_menu_requested(at_position):
	craft_station_types_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	craft_station_types_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)


	var a = craft_station_types_list.get_global_mouse_position()
	craft_station_types_popup_menu.position = Vector2(get_viewport().position) + a
	craft_station_types_popup_menu.popup()
	
