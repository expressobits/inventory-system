class_name NetworkedObjectPlacer
extends ObjectPlacer


@export_node_path var dropped_spawner_path : NodePath = "../../../../DroppedItemSpawner"
@onready var dropped_spawner : DropItemSpawner = get_node(dropped_spawner_path)


func place_item(item : SlotItem, position : Vector3, rotation : Vector3):
	var item_id = item.definition.id
	if item_id < InventoryItem.NONE:
		return false
	if not multiplayer.is_server():
		place_item_rpc.rpc_id(1, item_id, position, rotation)
	else:
		place_item_rpc(item_id, position, rotation)
	return true


@rpc("any_peer")
func place_item_rpc(item_id : int, position : Vector3, rotation : Vector3):
	if not multiplayer.is_server():
		return
	var item = get_item_from_id(item_id)
	if item == null:
		return
	super.place_item(item, position, rotation)


func _instantiate_object(dropped_item : PackedScene, position : Vector3, rotation : Vector3):
	var obj = dropped_spawner.spawn([position, rotation, dropped_item.resource_path])
#	dropped.emit(obj)
