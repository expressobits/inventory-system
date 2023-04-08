@tool
extends VBoxContainer


signal item_selected(item_id : int)
signal item_popup_menu_requested(at_position: Vector2)
signal new_item_pressed

@onready var list : ItemList = %ItemList

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
		
		# See if a value with just the file name is already in the map
#		for key in file_map.keys():
#			if file_map[key] == nice_file:
#				var bit_count = nice_file.count("/") + 2
#
#				var existing_nice_file = get_nice_file(key, bit_count)
#				nice_file = get_nice_file(file, bit_count)
#
#				while nice_file == existing_nice_file:
#					bit_count += 1
#					existing_nice_file = get_nice_file(key, bit_count)
#					nice_file = get_nice_file(file, bit_count)
#
#				file_map[key] = existing_nice_file
		
		item_map[id] = item


func update_item_list(items : Array[InventoryDatabaseItem]):
	list.clear()
	for item_database in items:
		var name_to_show : String = str(item_database.id)
		var icon : Texture2D = null
		if item_database.item != null:
			if item_database.item.name.is_empty():
				name_to_show = "No name"
			else:
				name_to_show = item_database.item.name
			icon = item_database.item.icon
		list.add_item(name_to_show, icon)


func apply_filter() -> void:
	item_list_handler.clear()
	for item_database_id in item_map.keys():
		var item_database = item_map[item_database_id]
		if item_database == null:
			continue
		if filter == "" or item_database.item == null or filter.to_lower() in item_database.item.resource_name.to_lower():
#			var nice_file = file_map[file]
#			if file in unsaved_files:
#				nice_file += MODIFIED_SUFFIX
			var item = item_database.item
			item_list_handler.append(item_database)
	update_item_list(item_list_handler)


func _on_item_list_item_activated(index):
	var item_text = list.get_item_text(index)
	var item_database = item_map.find_key(int(item_text))
#	emit_signal("item_selected", item)
#	select(int(item_text))
#	emit_signal("file_double_clicked", file)


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	emit_signal("item_selected", item_list_handler[index])
#	emit_signal("file_selected", file)
	
	if mouse_button_index == 2:
		emit_signal("item_popup_menu_requested", at_position)



func _on_new_item_button_pressed():
	emit_signal("new_item_pressed")
