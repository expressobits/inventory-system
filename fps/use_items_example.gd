class_name UseItemsExample3D
extends Node3D

@export var hotbar : Hotbar

func use_item_selected_in_hotbar():
	var item = hotbar.get_selected_item()
	var slot_index = hotbar.selection_index
	if item != null:
		item.properties["durability"] = item.properties["durability"] - 2.0
		if item.properties["durability"] < 0.0:
			hotbar.inventory.remove_at(slot_index, item)
		else:
			hotbar.inventory.update_slot(slot_index)