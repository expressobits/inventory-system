@tool
extends VBoxContainer
class_name ItemCategoriesItemList

signal category_selected(category : ItemCategory)
signal item_popup_menu_requested(at_position: Vector2)

@onready var list : ItemList = %ItemList
@onready var search_line_edit = $Control/SearchLineEdit

var database : InventoryDatabase
var categories_list_handler : Array[ItemCategory]


var filter: String:
	set(next_filter):
		filter = next_filter
		apply_filter(database.item_categories)
	get:
		return filter


func load_categories_types(database : InventoryDatabase) -> void:
	self.database = database
	apply_filter(database.item_categories)


func update_item_list(items : Array[ItemCategory]):
	list.clear()
	for i in items.size():
		list.add_item("")
		update_item(i)


func update_item(index : int):
	var category = categories_list_handler[index]
	var name_to_show : String = category.name
	var icon : Texture2D = category.icon
	list.set_item_text(index, name_to_show + " ("+category.resource_path+")")
	list.set_item_icon(index, icon)


func apply_filter(categories : Array[ItemCategory]) -> void:
	categories_list_handler.clear()
	for category in categories:
		if filter == "" or filter.to_lower() in category.name.to_lower():
			categories_list_handler.append(category)
	update_item_list(categories_list_handler)


func get_index_of(category : ItemCategory) -> int:
	for index in categories_list_handler.size():
		var c = categories_list_handler[index]
		if c == category:
			return index
	return -1


func _on_item_list_item_clicked(index, at_position, mouse_button_index):
	if mouse_button_index != 1 and mouse_button_index != 2:
		return
	if not list.is_item_selectable(index):
		return
	category_selected.emit(categories_list_handler[index])
	
	if mouse_button_index == 2:
		item_popup_menu_requested.emit(at_position)
		list.select(index)


func _on_search_line_edit_text_changed(new_text):
	filter = new_text


func get_index_of_category(category : ItemCategory) -> int:
	return categories_list_handler.find(category)
