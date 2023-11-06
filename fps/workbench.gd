extends Node3D
class_name Workbench


func get_station() -> CraftStation:
	return $CraftStation


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_preview_message(_interactor : InventoryInteractor) -> String:
	if $CraftStation.is_open:
		return ""
	return "[E] to Open Station"


func interact(interactor : InventoryInteractor):
	if Input.is_action_just_pressed("interact"):
		interactor.get_parent().open_station($CraftStation)
