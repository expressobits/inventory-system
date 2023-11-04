@tool
extends InventoryTabEditor
class_name CraftStationTypesEditor

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
	craft_station_type_editor.load_station(null)
	load_craft_station_types()


func select(station : CraftStationType):
	craft_station_type_editor.load_station(station)


func load_craft_station_types():
	craft_station_types_list.load_craft_station_types(database)


func remove_current_data():
	remove_station(current_data)


func remove_station(station : CraftStationType):
	var index = database.stations_type.find(station)
	if index == -1:
		return
	database.stations_type.remove_at(index)
	ResourceSaver.save(database, database.resource_path)
	load_craft_station_types()
	data_changed.emit()


func _on_craft_station_types_item_list_station_selected(station):
	current_data = station
	select(station)


func _on_craft_station_type_editor_changed(station):
	var index = craft_station_types_list.get_index_of(station)
	if index > -1:
		craft_station_types_list.update_item(index)


func _on_new_resource_dialog_file_selected(path):
	var item : CraftStationType = CraftStationType.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : CraftStationType = load(path)
		res.name = get_name_of_resource_path(path)
		database.stations_type.append(res)
		ResourceSaver.save(database, database.resource_path)
		load_craft_station_types()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	else:
		print(err)


func _on_craft_station_types_popup_menu_id_pressed(id):
	match id:
		ITEM_COPY_RESOURCE_PATH:
			DisplayServer.clipboard_set(current_data.resource_path)
		ITEM_REMOVE:
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Craft Station Type \""+current_data.name+"\"?"
		ITEM_REMOVE_AND_DELETE:
			remove_and_delete_confirmation_dialog.popup_centered()
			remove_and_delete_confirmation_dialog.dialog_text = "Remove Craft Station Type \""+current_data.name+"\" And Delete Resource \""+current_data.resource_path+"\"?"


func _on_craft_station_types_item_list_item_popup_menu_requested(at_position):
	craft_station_types_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	var copy = get_theme_icon("CopyNodePath", "EditorIcons")
	craft_station_types_popup_menu.add_icon_item(copy, "Copy Resource Path", ITEM_COPY_RESOURCE_PATH)
	craft_station_types_popup_menu.add_separator()
	craft_station_types_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)
	craft_station_types_popup_menu.add_icon_item(icon, "Remove and Delete Resource", ITEM_REMOVE_AND_DELETE)
	craft_station_types_popup_menu.set_item_disabled(3, true)


	var a = craft_station_types_list.get_global_mouse_position()
	craft_station_types_popup_menu.position = Vector2(get_viewport().position) + a
	craft_station_types_popup_menu.popup()


func _on_open_resource_dialog_file_selected(path):
	var res = load(path)
	if res is CraftStationType:
		var station : CraftStationType = res as CraftStationType
		if database.stations_type.has(station):
			push_warning("The craft station type \""+ station.name +"\" already exists in the list of craft stations types in the database!")
			return
		database.stations_type.append(station)
		ResourceSaver.save(database, database.resource_path)
		load_craft_station_types()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		data_changed.emit()
			
