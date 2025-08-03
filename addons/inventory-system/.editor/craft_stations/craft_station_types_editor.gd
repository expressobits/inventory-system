@tool
class_name CraftStationTypesEditor
extends BaseInventoryEditor

@onready var craft_station_type_editor : CraftStationTypeEditor = $HSplitContainer/CraftStationTypeEditor
@onready var craft_station_types_list : InventoryItemListEditor = $HSplitContainer/CraftStationTypesItemList
@onready var craft_station_types_popup_menu : PopupMenu = $CraftStationTypesPopupMenu


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	craft_station_type_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func _apply_theme():
	super._apply_theme()
	if not is_instance_valid(craft_station_types_list.search_icon):
		return
	craft_station_types_list.search_icon.texture = get_theme_icon("Search", "EditorIcons")
	

func on_load_database() -> void:
	craft_station_type_editor.load_station(database, null)
	load_craft_station_types()


func select(station : CraftStationType):
	craft_station_type_editor.load_station(database, station)
	craft_station_types_list.select(station)


func load_craft_station_types():
	craft_station_types_list.load_items(database.stations_type)


func _on_craft_station_type_editor_changed(station: CraftStationType):
	var index = craft_station_types_list.get_index_of_item_id(station.id)
	if index > -1:
		craft_station_types_list.update_item(index)


func _on_craft_station_types_popup_menu_id_pressed(id):
	match id:
		ITEM_REMOVE:
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Craft Station Type \""+current_data.name+"\"?"
		ITEM_DUPLICATE:
			super.duplicate_current_data()


func _on_craft_station_types_item_list_item_popup_menu_requested(at_position):
	craft_station_types_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	craft_station_types_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)

	icon = get_theme_icon("Duplicate", "EditorIcons")
	craft_station_types_popup_menu.add_icon_item(icon, "Duplicate", ITEM_DUPLICATE)

	var a = craft_station_types_list.get_global_mouse_position()
	craft_station_types_popup_menu.position = Vector2(get_viewport().position) + a
	craft_station_types_popup_menu.popup()
	
	
func remove_current_data() -> bool:
	var removed = super.remove_current_data()
	if removed:
		load_craft_station_types()
		craft_station_type_editor.load_station(database, null)
	return removed


func _on_craft_station_types_item_list_item_selected(item: Variant, index: int) -> void:
	current_data = item
	select(item)
