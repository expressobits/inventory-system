class_name ToolHandItem3D
extends HandItem3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")


@export var use_action : InteractAction


func get_interact_actions(_interactor : Interactor) -> Array:
	return [use_action]


func interact(character : Node, _action_code : int = 0):
	character.get_node("UseItemsExample").use_item_selected_in_hotbar()
