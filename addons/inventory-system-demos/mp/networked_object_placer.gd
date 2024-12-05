extends "../character/object_placer.gd"
class_name NetworkedObjectPlacer


@export_node_path var spawner_path : NodePath = NodePath("../../../../PlaceableItemSpawner")
@onready var spawner : PlaceableItemSpawner = get_node(spawner_path)


func place_item(item_id : String, position : Vector3, rotation : Vector3):
	if item_id.is_empty():
		return
	if not multiplayer.is_server():
		place_item_rpc.rpc_id(1, item_id, position, rotation)
	else:
		place_item_rpc(item_id, position, rotation)
	return


@rpc("any_peer")
func place_item_rpc(item_id : String, position : Vector3, rotation : Vector3):
	if not multiplayer.is_server():
		return
	super.place_item(item_id, position, rotation)


func _instantiate_object(dropped_item : PackedScene, position : Vector3, rotation : Vector3):
	var obj = spawner.spawn([position, rotation, dropped_item.resource_path])
#	dropped.emit(obj)
