@icon("res://addons/inventory-system/icons/inventory_database.svg")
extends Resource
class_name InventoryDatabase

## Database of items and their id information and dropped item as [PackedScene]

## TODO DOC Scene containing the dropable item version, this information is used by [InventoryHandler] to drop items
@export var items : Array


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


## Returns the id of [InventoryItem], return -1 if not found.
func get_id_from_item(item : InventoryItem) -> int:
	for item_data in items:
		if item_data.item == item:
			return item_data.id
	printerr("item ",item," is not in the database!")
	return -1


## Returns the id of dropped item as [PackedScene], return -1 if not found
func get_id_from_dropped_item(dropped_item : PackedScene) -> int:
	for item_data in items:
		if item_data.dropped_item == dropped_item:
			return item_data.id
	printerr("dropped_item ",dropped_item," is not in the database!")
	return -1


## Returns the [InventoryItem] of id, return null if not found
func get_item(id : int) -> InventoryItem:
	for item_data in items:
		if item_data.id == id:
			return item_data.item
	printerr("id ",id," is not in the database!")
	return null


## Returns the [DroppedItem] of id, return null if not found
func get_dropped_item(id : int) -> PackedScene:
	for item_data in items:
		if item_data.id == id:
			return item_data.dropped_item
	printerr("id ",id," is not in the database!")
	return null
