class_name UseItemsExample3D
extends Node3D

@export var hotbar : Hotbar

func use_item_selected_in_hotbar():
	var item = hotbar.get_selected_item()
	var slot_index = hotbar.selection_index
	if item != null and item.definition != null:
		item.properties["durability"] = item.properties["durability"] - 2.0
		if item.properties["durability"] <= 0.0:
			hotbar.get_inventory().remove_at(slot_index, item.duplicate())
		else:
			hotbar.get_inventory().update_slot(slot_index)
