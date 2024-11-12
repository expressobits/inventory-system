class_name DropItemSpawner
extends MultiplayerSpawner

## Class that customizes the way the spawner generates objects on the network, 
## placing information on the [PackedScene] of the dropped item, position and rotation.

@export var database : InventoryDatabase

func _init():
	spawn_function = _spawn_custom


func _spawn_custom(data : Array):
	if data.size() != 5 and data.size() != 4:
		print("data size error!")
		return null
	if typeof(data[0]) != TYPE_VECTOR3 or typeof(data[1]) != TYPE_VECTOR3 or typeof(data[2]) != TYPE_STRING:
		print("data incompatible!")
		return null
		
	var obj = load(data[2]).instantiate()
	obj.position = data[0]
	obj.rotation = data[1]
	var item_id : String = obj.item_id
	if data.size() > 3 and typeof(data[3]) == TYPE_INT:
		obj.amount = data[3]
	if data.size() > 4  and typeof(data[4]) == TYPE_DICTIONARY:
		obj.item_properties = data[4]
	obj.item_id = item_id
	return obj
