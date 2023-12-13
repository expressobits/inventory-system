@tool
extends VBoxContainer
class_name InventoryItemListEditor

signal item_selected(item : InventoryItem, index : int)
signal item_popup_menu_requested(at_position: Vector2)

@onready var list : ItemList = %ItemList
@onready var search_line_edit = $Control/SearchLineEdit

var item_map : Dictionary = {}
var database : InventoryDatabase
var item_list_handler : Array[InventoryItem]

var items: Array[InventoryItem] = []:
	set(next_files):
		items = next_files
		items.sort()
		update_item_map()
		apply_filter()
	get:
		return items
		
var filter: String:
	set(next_filter):
		filter = next_filter
		apply_filter()
	get:
		return filter


func load_items(database : InventoryDatabase) -> void:
	clear_items()
	self.database = database
	for item in database.items:
		add_item(item)
	update_item_map()
	apply_filter()


func add_item(item : InventoryItem):
	items.append(item)
	update_item_map()
	apply_filter()


func clear_items():
	items.clear()


func update_item_map() -> void:
	item_map = {}
	for item in items:
		item_map[item.id] = item


func update_item_list(items : Array[InventoryItem]):
	list.clear()
	for i in items.size():
		list.add_item("")
		update_item(i)


func update_item(index : int):
	var item = item_list_handler[index]
	var name_to_show : String = str(item.id)
	var icon : Texture2D = null
	if item != null:
		if item.name.is_empty():
			name_to_show = "No name"
		else:
			name_to_show = item.name
		icon = item.icon
	list.set_item_text(index, name_to_show)
	list.set_item_icon(index, icon)


func get_index_of_item_id(id : int) -> int:
	for index in item_list_handler.size():
		var item = item_list_handler[index]
		if item.id == id:
			return index
	return -1

func apply_filter() -> void:
	item_list_handler.clear()
	for item_id in item_map.keys():
		var item = item_map[item_id]
		if item == null:
			continue
		
		if filter == "" or item == null or filter.to_lower() in item.name.to_lower(): # search by item name
			item_list_handler.append(item)
		
		if item not in item_list_handler: # search by category name
			for item_category in item.categories:
				if item not in item_list_handler and filter.to_lower() in item_category.name.to_lower():
					item_list_handler.append(item)
					
	update_item_list(item_list_handler)


func _on_item_list_item_activated(index):
	var item_text = list.get_item_text(index)
	var item_database = item_map.find_key(int(item_text))


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	if mouse_button_index != 1 and mouse_button_index != 2:
		return
	if not list.is_item_selectable(index):
		return
	item_selected.emit(item_list_handler[index], index)
	
	if mouse_button_index == 2:
		item_popup_menu_requested.emit(at_position)
		list.select(index)

func _on_search_line_edit_text_changed(new_text):
	filter = new_text
