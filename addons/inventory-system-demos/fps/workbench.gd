class_name Workbench
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")


@export var actions : Array
@onready var craft_station : CraftStation = $CraftStation
@onready var openable : Openable = $Openable
@onready var input_inventory: GridInventory = $InputInventory

var character

func _ready() -> void:
	input_inventory.request_drop_obj.connect(_on_request_drop_obj)


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	if openable.is_open:
		return []
	return actions


func interact(character : Node, action_index : int = 0):
	#craft_station.load_valid_recipes()
	if action_index == 0 and !openable.is_open:
		open(character)
		character.character_inventory_system.open_station(craft_station)
	else:
		self.rotate(Vector3.UP, 90)
	
func open(character : Node):
	openable.open(character)
	
func close(character : Node):
	openable.close(character)
	
	
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
