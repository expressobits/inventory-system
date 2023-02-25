extends MultiplayerSpawner

func _init():
	spawn_function = _spawn_dropped_item


func _spawn_dropped_item(data):
	if data.size() != 3 or typeof(data[0]) != TYPE_VECTOR3 or typeof(data[1]) != TYPE_VECTOR3 or typeof(data[2]) != TYPE_STRING:
		print("data error!")
		return null
	var obj = load(data[2]).instantiate()
	obj.position = data[0]
	obj.rotation = data[1]
	return obj
