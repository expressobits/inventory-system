extends Node3D
class_name Dropper

signal dropped(node : Node)

@export var inventories: Array[Inventory]


func _ready() -> void:
	for inventory in inventories:
		inventory.request_drop_obj.connect(_on_request_drop_obj)


func _on_request_drop_obj(dropped_item: String, item_id: String, amount: int, properties: Dictionary):
	var packed_scene : PackedScene = load(dropped_item)
	var node = packed_scene.instantiate()
	get_parent().get_parent().add_child(node)
	node.set("item_id", item_id)
	node.set("amount", amount)
	node.position = global_position
	node.set("rotation", global_rotation)
	node.set("item_properties", properties)
	dropped.emit(node)
