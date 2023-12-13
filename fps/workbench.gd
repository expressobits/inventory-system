extends Node3D
class_name Workbench

@export var actions : Array[InteractAction]
@onready var craft_station : CraftStation = $CraftStation


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_actions(_interactor : InventoryInteractor) -> Array[InteractAction]:
	if craft_station.is_open:
		return []
	return actions


func interact(interactor : InventoryInteractor, action_index : int = 0):
	if action_index == 0:
		interactor.get_parent().open_station(craft_station)
	else:
		self.rotate(Vector3.UP, 90)
