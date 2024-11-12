extends Dropper


@export_node_path var drop_item_spawner_path := NodePath("../../DroppedItemSpawner")
@onready var drop_item_spawner = get_node(drop_item_spawner_path)


func _ready():
	if is_multiplayer_authority():
		super._ready()


func _on_request_drop_obj(dropped_item : String, item_id : String, amount : int, properties : Dictionary):
	if(drop_item_spawner == null):
		printerr("Spawner is null!")
		return
	var packed_scene : PackedScene = load(dropped_item)
	var data : Array
	data.append(global_position)
	data.append(global_rotation)
	data.append(packed_scene.resource_path)
	data.append(amount)
	data.append(properties)
	var obj = drop_item_spawner.spawn(data)
	dropped.emit(obj)
