@tool
@icon("res://addons/inventory-system/icons/inventory_database.svg")
extends Resource
class_name InventoryDatabase

## Database of items and their id information

@export var items : Array[InventoryItem] = []:
	set(new_items):
		items = new_items
		update_items_cache()
	get:
		return items

@export var recipes : Array[Recipe]

@export var stations_type : Array[CraftStationType]

@export var item_categories : Array[ItemCategory] = []:
	set(new_item_categories):
		item_categories = new_item_categories
		update_items_categories_cache()
	get:
		return item_categories

var items_cache : Dictionary

var categories_code_cache : Dictionary


func update_items_cache():
	items_cache.clear()
	for item in items:
		if item != null:
			items_cache[item.id] = item


func update_items_categories_cache():
	categories_code_cache.clear()
	for i in item_categories.size():
		var category = item_categories[i]
		if category != null:
			if not Engine.is_editor_hint():
				category.code = int(pow(2, i))
			categories_code_cache[int(pow(2, i))] = category


func add_new_item(item : InventoryItem):
	items.append(item)
	update_items_cache()	


func remove_item(item : InventoryItem):
	var index = items.find(item)
	if index > -1:
		items.remove_at(index)
		update_items_cache()


func add_new_category(category : ItemCategory):
	item_categories.append(category)
	update_items_categories_cache()	


func remove_category(category : ItemCategory):
	var index = item_categories.find(category)
	if index > -1:
		item_categories.remove_at(index)
		update_items_categories_cache()


## Returns the id of dropped item as [PackedScene], return 0 if not found
func get_id_from_dropped_item(dropped_item : PackedScene) -> int:
	for item_data in items:
		if item_data.dropped_item == dropped_item:
			return item_data.item.id
	return InventoryItem.NONE


## Returns the [InventoryItem] of id, return null if not found
func get_item(id : int) -> InventoryItem:
	if items_cache.has(id):
		return items_cache[id]
	return null


func has_item_id(id : int) -> bool:
	return items_cache.has(id)


func get_valid_id() -> int:
	for i in items:
		return i.id
	return -1


func get_new_valid_id() -> int:
	for i in 92233720368547758:
		if not has_item_id(i):
			return i
	return -1


func get_category(code : int) -> ItemCategory:
	return categories_code_cache[code]
