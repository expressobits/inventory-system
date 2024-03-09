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


func interact(interactor : Interactor, action_index : int = 0):
	if action_index == 0:
		open_station(interactor)
	else:
		self.rotate(Vector3.UP, 90)

func open_station(interactor : Interactor):
	if multiplayer.is_server():
		interactor.get_parent().open_station(craft_station)
	else:
		open_station_rpc.rpc_id(1, interactor.get_path())


@rpc("any_peer")
func open_station_rpc(interactor_path : NodePath):
	if not multiplayer.is_server():
		return
	var interactor = get_node(interactor_path)
	interactor.get_parent().open_station(craft_station)
