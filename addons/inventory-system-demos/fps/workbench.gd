class_name Workbench
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")


@export var actions : Array
@onready var craft_station : CraftStation = $CraftStation
@onready var openable : Openable = $Openable
@onready var input_inventory: GridInventory = $InputInventory

var character


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	if openable.is_open:
		return []
	return actions


func interact(_character : Node, action_index : int = 0):
	#craft_station.load_valid_recipes()
	if action_index == 0 and !openable.is_open:
		open(_character)
		_character.character_inventory_system.open_station(craft_station)
	else:
		self.rotate(Vector3.UP, 90)
	
func open(_character : Node):
	openable.open(_character)
	
func close(_character : Node):
	openable.close(_character)
