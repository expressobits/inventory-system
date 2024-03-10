class_name Shelf
extends BoxInventory

@onready var visual_inventory_3d : VisualInventory = $VisualInventory3D


var slot_index : int = -1
var actions_shelf : Array[InteractAction]

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


func interact(character : Node, action_index : int = 0):
	if inventory.is_open:
		return
	var interactor = character.character_inventory_system.interactor
	var item = interactor.get_hotbar().get_selected_item()
	if action_index == 0:
		super.interact(character, action_index)
	if action_index == 1:
		var shelf_item = get_actual_item()
		if shelf_item != null and shelf_item.definition != null:
			character.character_inventory_system.move_between_inventories_at(inventory, slot_index, 1, interactor.get_inventory_handler().get_inventory(0), interactor.get_hotbar().selection_index)
		return
	if action_index == 2:
		if item != null and item.definition != null:
			character.character_inventory_system.move_between_inventories_at(interactor.get_inventory_handler().get_inventory(0), interactor.get_hotbar().selection_index, 1, inventory, slot_index)
		return


func get_actual_item():
	if slot_index != -1:
		return inventory.slots[slot_index].item
	return null


func get_interact_actions(interactor : Interactor) -> Array[InteractAction]:
	actions_shelf.clear()
	if inventory.is_open:
		return actions_shelf
	actions_shelf.append_array(actions)
	var shelf_item = get_actual_item()
	if shelf_item != null and shelf_item.definition != null:
		var action = InteractAction.new()
		action.description = "Get " + shelf_item.definition.name
		action.input = "item_pickup"
		action.code = 1
		actions_shelf.append(action)
	var item : Item = interactor.get_hotbar().get_selected_item()
	if item != null and item.definition != null and (shelf_item.definition == null or shelf_item.definition == item.definition):
		var action = InteractAction.new()
		action.description = "Place " + item.definition.name
		action.input = "item_place"
		action.code = 2
		actions_shelf.append(action)
	return actions_shelf
