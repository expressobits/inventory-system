@tool
@icon("res://addons/inventory-system/icons/inventory_database.svg")
extends Resource
class_name InventoryDatabase

## Database of items and their id information and dropped item as [PackedScene]

## TODO DOC Scene containing the dropable item version, this information is used by [InventoryHandler] to drop items
@export var items : Array[InventoryItem] = []:
	set(new_items):
		items = new_items
		update_items_cache()
	get:
		return items
		

@export var recipes : Array[Recipe]

@export var stations_type : Array[CraftStationType]

@export var item_categories : Array[ItemCategory]

var items_cache : Dictionary

# TODO Create loading dropped items and items with folder
# @export var path_test := "res://addons/inventory-system/demos/fps/items/"
#func load_items():
#	for item_list in item_list_test:
#		var obj = load(str(path_test, item_list.name, ".tres"))
#		var item = obj as Item
#		items.append(items)
		
#var item_data_example = {
#	"item" : resource,
#	"id" : 0,
#	"dropped_item", packedScene,
#	"hand_item", packedScene,
#}

func update_items_cache():
	items_cache.clear()
	for item in items:
		if item != null:
			items_cache[item.id] = item

## Returns the id of dropped item as [PackedScene], return 0 if not found
func get_id_from_dropped_item(dropped_item : PackedScene) -> int:
	for item_data in items:
		if item_data.dropped_item == dropped_item:
			return item_data.item.id
	# printerr("dropped_item ",dropped_item," is not in the database!")
	return InventoryItem.NONE


## Returns the [InventoryItem] of id, return null if not found
func get_item(id : int) -> InventoryItem:
	if items_cache.has(id):
		return items_cache[id]
#	for i in items:
#		if i.id == id:
#			return i
	# printerr("id ",id," is not in the database!")
	return null


func has_item_id(id : int) -> bool:
	for item in items:
		if item.id == id:
			return true
	return false


func get_valid_id() -> int:
	for i in items:
		return i.id
	return -1


func get_new_valid_id() -> int:
	for i in 92233720368547758:
		if not has_item_id(i):
			return i
	return -1
	
