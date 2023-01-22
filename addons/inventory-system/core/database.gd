extends Resource
class_name InventoryDatabase

@export var items : Array

# TODO Create loading pickables items and items with folder

#var item_data_example = {
#	"item" : resource,
#	"id" : 0,
#	"pickable_item", packedScene,
#	"hand_item", packedScene,
#}

func get_id_from_item(item : Item) -> int:
	for item_data in items:
		if item_data.item == item:
			return item_data.id
	printerr("item ",item," is not in the database!")
	return -1
	
func get_id_from_pickable_item(pickable_item : PickableItem) -> int:
	for item_data in items:
		if item_data.pickable_item == pickable_item:
			return item_data.id
	printerr("pickable_item ",pickable_item," is not in the database!")
	return -1
	
func get_item(id : int) -> Item:
	for item_data in items:
		if item_data.id == id:
			return item_data.item
	printerr("id ",id," is not in the database!")
	return null
	
func get_pickable_item(id : int) -> PackedScene:
	for item_data in items:
		if item_data.id == id:
			return item_data.pickable_item
	printerr("id ",id," is not in the database!")
	return null
