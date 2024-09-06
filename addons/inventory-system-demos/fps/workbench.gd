class_name Workbench
extends Node3D

@export var actions : Array[InteractAction]
@onready var craft_station : CraftStation = $CraftStation
@onready var openable : Openable = $Openable

var character


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array[InteractAction]:
	if openable.is_open:
		return []
	return actions


func interact(character : Node, action_index : int = 0):
	if action_index == 0 and !openable.is_open:
		open(character)
		character.character_inventory_system.open_station(craft_station)
	else:
		self.rotate(Vector3.UP, 90)
	
func open(character : Node):
	openable.open(character)
	
func close(character : Node):
	openable.close(character)
