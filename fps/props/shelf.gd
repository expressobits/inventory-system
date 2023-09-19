extends BoxInventory
class_name Shelf

@onready var visual_inventory_3d : VisualInventory = $VisualInventory3D
@onready var inventory = $Inventory


var slot_index : int = -1

func get_interaction_position(interaction_point : Vector3) -> Vector3:
	var near_distance : float = 2500
	var near_position : Vector3 = global_position
	slot_index = -1
	for i in visual_inventory_3d.visual_slots.size():
		var visual_slot = visual_inventory_3d.visual_slots[i]
		var p = interaction_point.distance_squared_to(visual_slot.global_position)
		if p < near_distance:
			near_distance = p
			near_position = visual_slot.global_position
			slot_index = i
	return near_position


func interact():
	pass

func get_actual_item():
	if slot_index != -1:
		return inventory.slots[slot_index].item
	return null


func get_interact_message() -> String:
	var item = get_actual_item()
	if item != null:
		return "E to Open Shelf\n F Pegar "+item.name
	return "E to Open Shelf"
