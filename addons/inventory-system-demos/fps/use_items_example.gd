class_name UseItemsExample3D
extends Node3D

@export var hotbar : Hotbar

func use_item_selected_in_hotbar():
	var stack : ItemStack = hotbar.get_stack_on_selection()
	if stack == null:
		return
	var stack_index = hotbar.get_inventory().stacks.find(stack)
	if stack_index == -1:
		return
	var definition = hotbar.database.get_item(stack.item_id)
	if definition != null:
		if stack.properties.has("durability"):
			stack.properties["durability"] = stack.properties["durability"] - 2.0
			if stack.properties["durability"] <= 0.0:
				hotbar.get_inventory().remove_at(stack_index, stack.item_id)
			else:
				hotbar.get_inventory().update_stack(stack_index)
			print(stack.properties["durability"])
