extends BoxInventory
class_name Shelf

@onready var visual_inventory_3d : VisualInventory = $VisualInventory3D


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


func interact(interactor : InventoryInteractor):
	if inventory.is_open:
		return
	var item = interactor.hotbar.get_selected_item()
	super.interact(interactor)
	if Input.is_action_just_pressed("item_pickup"):
		var shelf_item = get_actual_item()
		if shelf_item != null:
			var amount = interactor.inventory_handler.move_between_inventories_at(inventory, slot_index, 1, interactor.inventory_handler.inventory, interactor.hotbar.selection_index)
			if amount > 0:
				interactor.inventory_handler.move_between_inventories(inventory, slot_index, 1, interactor.inventory_handler.inventory)
		return
	if Input.is_action_just_pressed("item_place"):
		if item != null:
			interactor.inventory_handler.move_between_inventories_at(interactor.inventory_handler.inventory, interactor.hotbar.selection_index, 1, inventory, slot_index)
		return


func get_actual_item():
	if slot_index != -1:
		return inventory.slots[slot_index].item
	return null


func get_interact_preview_message(interactor : InventoryInteractor) -> String:
	var message = super.get_interact_preview_message(interactor)
	if inventory.is_open:
		return message
	var shelf_item = get_actual_item()
	if shelf_item != null:
		message += "\n [Mouse Left] Get " + shelf_item.name
	var item = interactor.hotbar.get_selected_item()
	if item != null and (shelf_item == null or shelf_item == item):
		message += "\n [Mouse Right] Place " + item.name +" on Shelf"
	return message
