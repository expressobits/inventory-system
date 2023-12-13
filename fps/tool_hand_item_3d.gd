class_name ToolHandItem3D
extends HandItem3D


@export var use_action : InteractAction


func get_interact_actions(_interactor : Interactor) -> Array[InteractAction]:
	return [use_action]


func interact(interactor : Interactor, _action_code : int = 0):
	interactor.get_parent().get_parent().get_node("UseItemsExample").use_item_selected_in_hotbar()
