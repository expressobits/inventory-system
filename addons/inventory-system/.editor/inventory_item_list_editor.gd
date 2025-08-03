## Inventory Item List Editor
##
## Item means an item in the GUI element ItemList. So, this class is responsible for the types:
##	* ItemDefinition
##	* CraftStationType
##	* ItemCategory.
##
## It is used to create a list of items in the editor on the left side.
## It provides a search bar to filter the items by name on top of the list.
##
## It maintains a dictionary of items and their corresponding IDs.
## It provides signals to notify when an item is selected or when the popup menu is requested.
## It is used in the ItemDefinitionsEditor class to display the list of items and handle item selection.

@tool
class_name InventoryItemListEditor
extends VBoxContainer

signal item_selected(item, index : int)
signal item_popup_menu_requested(at_position: Vector2)

var item_map : Dictionary = {}
var item_list_handler : Array

@onready var control: Control = $SearchItems
@onready var search_line_edit : LineEdit = $SearchItems/LineEdit
@onready var search_icon : TextureRect = $SearchItems/TextureRect
@onready var item_list : ItemList = $ItemList



var items: Array = []:
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
		

func _ready() -> void:
	offset_right = 256.0
	offset_bottom = 36.0
	
	control.custom_minimum_size = Vector2(0, 32)
	control.layout_mode = 2
	control.size_flags_vertical = 4
	
	search_line_edit.custom_minimum_size = Vector2(0, 32)
	search_line_edit.layout_mode = 1
	search_line_edit.anchors_preset = 15
	search_line_edit.anchor_right = 1.0
	search_line_edit.anchor_bottom = 1.0
	search_line_edit.grow_horizontal = 2
	search_line_edit.grow_vertical = 2
	search_line_edit.size_flags_horizontal = 3
	search_line_edit.placeholder_text = "Search Items"
	search_line_edit.draw_control_chars = true
	search_line_edit.text_changed.connect(_on_search_line_edit_text_changed)
	
	search_icon.custom_minimum_size = Vector2(16, 16)
	search_icon.layout_mode = 1
	search_icon.anchors_preset = 6
	search_icon.anchor_left = 1.0
	search_icon.anchor_top = 0.5
	search_icon.anchor_right = 1.0
	search_icon.anchor_bottom = 0.5
	search_icon.offset_left = -24.0
	search_icon.offset_top = -8.0
	search_icon.offset_right = -8.0
	search_icon.offset_bottom = 8.0
	search_icon.grow_horizontal = 0
	search_icon.grow_vertical = 2
	search_icon.expand_mode = 1
	
	assert(item_list != null, "ItemList not found")
	item_list.unique_name_in_owner = true
	item_list.layout_mode = 2
	item_list.size_flags_vertical = Control.SIZE_EXPAND_FILL
	item_list.fixed_icon_size = Vector2i(16, 16)
	item_list.item_clicked.connect(_on_item_list_item_clicked)


func load_items(items : Array) -> void:
	clear_items()
	for item in items:
		add_item(item)
	update_item_map()
	apply_filter()


func add_item(item):
	items.append(item)
	update_item_map()
	apply_filter()


func clear_items():
	items.clear()


func update_item_map() -> void:
	item_map = {}
	for item in items:
		item_map[item.id] = item


func update_item_list(items : Array):
	assert(item_list != null, "ItemList not found")
	item_list.clear()
	for i in items.size():
		item_list.add_item("")
		update_item(i)


func update_item(index : int):
	var item = item_list_handler[index]
	var name_to_show : String = str(item.id)
	var icon : Texture2D = null
	if item != null:
		if item.name.is_empty():
			name_to_show = "(Unnamed Item)"
		else:
			name_to_show = item.name
		icon = item.icon
	assert(item_list != null, "ItemList not found")
	item_list.set_item_text(index, name_to_show)
	item_list.set_item_icon(index, icon)


func get_index_of_item_id(id : String) -> int:
	for index in item_list_handler.size():
		var item = item_list_handler[index]
		if item.id == id:
			return index
	return -1


func select(item) -> void:
	var index = get_index_of_item_id(item.id)
	assert(index > -1, "Item not found in list")
	if index > -1:
		assert(item_list_handler[index] == item)
		item_list.select(index)


func apply_filter() -> void:
	item_list_handler.clear()
	for item_id in item_map.keys():
		var item = item_map[item_id]
		if item == null:
			continue
		
		if filter == "" or item == null or filter.to_lower() in item.name.to_lower(): # search by item name
			item_list_handler.append(item)
		
		if item is ItemDefinition and item not in item_list_handler: # search by category name
			for item_category in item.categories:
				if item not in item_list_handler and filter.to_lower() in item_category.name.to_lower():
					item_list_handler.append(item)
					
	update_item_list(item_list_handler)


func _on_item_list_item_activated(index):
	var item_text = item_list.get_item_text(index)
	var item_database = item_map.find_key(int(item_text))


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	if mouse_button_index != 1 and mouse_button_index != 2:
		return
	if not item_list.is_item_selectable(index):
		return
	item_selected.emit(item_list_handler[index], index)
	
	if mouse_button_index == 2:
		item_popup_menu_requested.emit(at_position)


func _on_search_line_edit_text_changed(new_text):
	filter = new_text
