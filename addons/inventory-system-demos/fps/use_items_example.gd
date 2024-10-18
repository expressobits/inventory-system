class_name UseItemsExample3D
extends Node3D

@export var hotbar : Hotbar

func use_item_selected_in_hotbar():
	var item_id : String = hotbar.get_selected_item()
	var definition = hotbar.database.get_item(item_id)
	var stack_index = hotbar.selection_index
	var item_stack = hotbar.get_inventory().items[stack_index]
	if definition != null:
		if item_stack.properties.has("durability"):
			item_stack.properties["durability"] = item_stack.properties["durability"] - 2.0
			if item_stack.properties["durability"] <= 0.0:
				hotbar.get_inventory().remove_at(stack_index, item_id)
			else:
				hotbar.get_inventory().update_stack(stack_index)
