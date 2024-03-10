class_name Workbench
extends Node3D

@export var actions : Array[InteractAction]
@onready var craft_station : CraftStation = $CraftStation


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array[InteractAction]:
	if craft_station.is_open:
		return []
	return actions


func interact(character : Node, action_index : int = 0):
	if action_index == 0:
		character.character_inventory_system.open_station(craft_station)
	else:
		self.rotate(Vector3.UP, 90)

