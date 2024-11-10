class_name BoxInventory
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@onready var inventory : Inventory = $Inventory
@export var actions : Array
@onready var openable : Openable = $Openable

func _ready():
	inventory.request_drop_obj.connect(_on_request_drop_obj)
	openable.closed.connect(_on_openable_closed)
	openable.opened.connect(_on_openable_opened)

func get_inventory() -> Inventory:
	return $Inventory


func _on_inventory_opened():
	_on_open()


func _on_open():
	$box.visible = false
	$boxOpen.visible = true
	$Open.play()


func _on_close():
	$box.visible = true
	$boxOpen.visible = false
	$Close.play()


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	if openable.is_open:
		return []
	return actions


func interact(character : Node, _action_index : int = 0):
	if !openable.is_open:
		character.character_inventory_system.open_inventory(inventory)
		open(character)


func open(character : Node):
	openable.open(character)


func close(character : Node):
	openable.close(character)


func _on_openable_opened(character: Node) -> void:
	_on_open()


func _on_openable_closed(character: Node) -> void:
	_on_close()


func _on_request_drop_obj(dropped_item: String, item_id: String, amount: int, properties: Dictionary):
	var packed_scene : PackedScene = load(dropped_item)
	var node = packed_scene.instantiate()
	get_parent().add_child(node)
	node.set("item_id", item_id)
	node.set("amount", amount)
	node.set("position", get("position") + Vector3(0, 1, 0))
	node.set("rotation", get("rotation"))
	node.set("item_properties", properties)
	#dropped.emit(node)
