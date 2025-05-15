extends Node3D
class_name Dropper

signal dropped(node : Node)

@export var inventories: Array[Inventory]
@export var database: InventoryDatabase
@export var dropped_item_property_name: String = "dropped_item"


func _ready() -> void:
	for inventory in inventories:
		inventory.request_drop_item.connect(drop)


func drop(item: String, amount:int = 1, properties: Dictionary = {}):
	var def = database.get_item(item)
	if def == null:
		return
	if not def.properties.has(dropped_item_property_name):
		return
	var obj = def.properties[dropped_item_property_name]
	if obj == null:
		return
	_on_request_drop_obj(obj, item, amount, properties)


func _on_request_drop_obj(dropped_item : String, item_id: String, amount: int, properties: Dictionary):
	var database: InventoryDatabase = inventories[0].database
	var packed_scene : PackedScene = load(dropped_item)
	var node = packed_scene.instantiate()
	get_parent().get_parent().add_child(node)
	node.set("item_id", item_id)
	node.set("amount", amount)
	node.position = global_position
	node.set("rotation", global_rotation)
	node.set("item_properties", properties)
	dropped.emit(node)
