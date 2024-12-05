class_name PlaceableItemSpawner
extends MultiplayerSpawner

## Class that customizes the way the spawner generates objects on the network, 
## placing information on the [PackedScene] of the dropped item, position and rotation.

func _init():
	spawn_function = _spawn_custom


func _spawn_custom(data : Array):
	if data.size() != 3:
		print("data size error!")
		return null
	if typeof(data[0]) != TYPE_VECTOR3 or typeof(data[1]) != TYPE_VECTOR3 or typeof(data[2]) != TYPE_STRING:
		print("data incompatible!")
		return null
		
	var obj = load(data[2]).instantiate()
	obj.position = data[0]
	obj.rotation = data[1]
	return obj
