@tool
extends VBoxContainer
class_name CraftStationTypesItemList

signal station_selected(station : CraftStationType)
signal item_popup_menu_requested(at_position: Vector2)

@onready var list : ItemList = %ItemList
@onready var search_line_edit = $Control/SearchLineEdit

var database : InventoryDatabase
var stations_list_handler : Array[CraftStationType]


var filter: String:
	set(next_filter):
		filter = next_filter
		apply_filter(database.stations_type)
	get:
		return filter


func load_craft_station_types(database : InventoryDatabase) -> void:
	self.database = database
	apply_filter(database.stations_type)


func update_item_list(items : Array[CraftStationType]):
	list.clear()
	for i in items.size():
		list.add_item("")
		update_item(i)


func update_item(index : int):
	var station = stations_list_handler[index]
	var name_to_show : String = station.name
	var icon : Texture2D = station.icon
	list.set_item_text(index, name_to_show+" ("+station.resource_path+")")
	list.set_item_icon(index, icon)


func apply_filter(stations : Array[CraftStationType]) -> void:
	stations_list_handler.clear()
	for station in stations:
		if filter == "" or filter.to_lower() in station.name.to_lower():
			stations_list_handler.append(station)
	update_item_list(stations_list_handler)


func get_index_of(station : CraftStationType) -> int:
	for index in stations_list_handler.size():
		var s = stations_list_handler[index]
		if s == station:
			return index
	return -1


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	if mouse_button_index != 1 and mouse_button_index != 2:
		return
	if not list.is_item_selectable(index):
		return
	station_selected.emit(stations_list_handler[index])
	
	if mouse_button_index == 2:
		item_popup_menu_requested.emit(at_position)
		list.select(index)


func _on_search_line_edit_text_changed(new_text):
	filter = new_text
