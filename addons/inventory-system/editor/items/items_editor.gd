@tool
extends Control
class_name ItemsEditor

var database : InventoryDatabase
var editor_plugin : EditorPlugin
@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var new_item_dialog : FileDialog = $NewItemResourceDialog
@onready var open_item_dialog : FileDialog = $OpenItemDialog
@onready var list : ItemList = %ItemList


var item_map: Dictionary = {}
var current_id_item : int = -1
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


func _ready():
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	item_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func load_items_from_database(database : InventoryDatabase) -> void:
	self.database = database
	load_items()


func load_items() -> void:
	clear_items()
	for item_database in database.items:
		item_ids.append(item_database.id)
		item_ids.sort()
		update_item_map()
		apply_filter()


func select(id : int):
	for item_in_list in database.items:
		if item_in_list.id == id:
			item_editor.load_item(database.items[id])


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


func get_nice_file(file_path: String, path_bit_count: int = 1) -> String:
	var bits = file_path.replace("res://", "").replace(".dialogue", "").split("/")
	bits = bits.slice(-path_bit_count)
	return "/".join(bits)


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
	
#	select_file(current_file_path)

func update_item_list(items : Array[InventoryDatabaseItem]):
	list.clear()
	for item_database in items:
		var name_to_show : String = str(item_database.id)
		var icon : Texture2D = null
		if item_database.item != null:
			name_to_show = item_database.item.name
			icon = item_database.item.icon
		list.add_item(name_to_show, icon)


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_item_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_item_dialog.min_size = Vector2(600, 500) * scale


func _on_new_item_button_pressed():
	if not is_instance_valid(database):
		return
	
	new_item_dialog.popup_centered()


func _on_new_item_resource_dialog_file_selected(path):
	var item : InventoryItem = InventoryItem.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : InventoryItem = load(path)
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		var new_database_item : InventoryDatabaseItem = InventoryDatabaseItem.new()
		new_database_item.id = database.get_valid_id()
		new_database_item.item = res
		database.items.append(new_database_item)
		load_items()
	else:
		print(err)


func _on_theme_changed():
	_apply_theme()


func _on_item_list_item_activated(index):
	var item_text = list.get_item_text(index)
	var item_database = item_map.find_key(int(item_text))
	select(int(item_text))
#	emit_signal("file_double_clicked", file)


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	item_editor.load_item(item_list_handler[index])
#	emit_signal("file_selected", file)
	
#	if mouse_button_index == 2:
#		emit_signal("file_popup_menu_requested", at_position)
