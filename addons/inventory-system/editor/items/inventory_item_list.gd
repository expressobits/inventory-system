@tool
extends VBoxContainer


signal item_selected(item_id : int, index : int)
signal item_popup_menu_requested(at_position: Vector2)
signal new_item_pressed

@onready var list : ItemList = %ItemList
@onready var search_line_edit = $Control/SearchLineEdit

var item_map : Dictionary = {}
var database : InventoryDatabase
var item_list_handler : Array[InventoryDatabaseItem]

var item_ids: PackedInt32Array = []:
	set(next_files):
		item_ids = next_files
		item_ids.sort()
		update_item_map()
		apply_filter()
	get:
		return item_ids
		
var filter: String:
	set(next_filter):
		filter = next_filter
		apply_filter()
	get:
		return filter


func load_items(database : InventoryDatabase) -> void:
	clear_items()
	self.database = database
	for item_database in database.items:
		item_ids.append(item_database.id)
		item_ids.sort()
		update_item_map()
		apply_filter()


func clear_items():
	item_ids.clear()
#	for item_in_list in _item_list:
#		item_in_list.queue_free()
#	_item_list.clear()


func update_item_map() -> void:
	item_map = {}
	for id in item_ids:
		var item : InventoryDatabaseItem = database.get_item_database(id)
		item_map[id] = item


func update_item_list(items : Array[InventoryDatabaseItem]):
	list.clear()
	for i in items.size():
		list.add_item("")
		update_item(i)


func update_item(index : int):
	var item_database = item_list_handler[index]
	var name_to_show : String = str(item_database.id)
	var icon : Texture2D = null
	if item_database.item != null:
		if item_database.item.name.is_empty():
			name_to_show = "No name"
		else:
			name_to_show = item_database.item.name
		icon = item_database.item.icon
	list.set_item_text(index, name_to_show)
	list.set_item_icon(index, icon)


func get_index_of_item_id(id : int) -> int:
	for index in item_list_handler.size():
		var item = item_list_handler[index]
		if item.id == id:
			return index
	return -1;


func apply_filter() -> void:
	item_list_handler.clear()
	for item_database_id in item_map.keys():
		var item_database = item_map[item_database_id]
		if item_database == null:
			continue
		if filter == "" or item_database.item == null or filter.to_lower() in item_database.item.name.to_lower():
			var item = item_database.item
			item_list_handler.append(item_database)
	update_item_list(item_list_handler)


func _on_item_list_item_activated(index):
	var item_text = list.get_item_text(index)
	var item_database = item_map.find_key(int(item_text))


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	if mouse_button_index != 1 and mouse_button_index != 2:
		return
	emit_signal("item_selected", item_list_handler[index], index)
	
	if mouse_button_index == 2:
		emit_signal("item_popup_menu_requested", at_position)



func _on_new_item_button_pressed():
	emit_signal("new_item_pressed")


func _on_search_line_edit_text_changed(new_text):
	filter = new_text
