class_name UseItemsExample3D
extends Node3D

@export var hotbar : Hotbar

func use_item_selected_in_hotbar():
	var item_id : String = hotbar.get_selected_item()
	var definition = hotbar.database.get_item(item_id)
	var slot_index = hotbar.selection_index
	var slot = hotbar.get_inventory().slots[slot_index]
	if definition != null:
		if slot.properties.has("durability"):
			slot.properties["durability"] = slot.properties["durability"] - 2.0
			if slot.properties["durability"] <= 0.0:
				hotbar.get_inventory().remove_at(slot_index, item_id)
			else:
				hotbar.get_inventory().update_slot(slot_index)
