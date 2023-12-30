class_name NetworkedObjectPlacer
extends ObjectPlacer


@export_node_path var place_spawner_path : NodePath = "../../../../PlaceItemSpawner"
@onready var place_spawner : PlaceItemSpawner = get_node(place_spawner_path)


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
	var definition = get_item_from_id(item_id)
	if definition == null:
		return
	var item = SlotItem.new();
	item.definition = definition
	super.place_item(item, position, rotation)


func _instantiate_object(place_item : PackedScene, position : Vector3, rotation : Vector3):
	var obj = place_spawner.spawn([position, rotation, place_item.resource_path])
	dropped.emit(obj)
