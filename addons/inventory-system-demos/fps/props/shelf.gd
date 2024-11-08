class_name Shelf
extends BoxInventory

@onready var visual_inventory_3d : VisualInventory = $VisualInventory3D


var slot_index : int = -1
var actions_shelf : Array

func _on_open():
	pass


func _on_close():
	pass


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

func interact_with_slot(character : Node, action_index : int = 0, actual_slot_index := 0):
	if openable.is_open:
		return
	var interactor = character.character_inventory_system.interactor
	var item_id = interactor.hotbar.get_selected_item()
	var definition = interactor.hotbar.database.get_item(item_id)
	var char_inventory = character.character_inventory_system.main_inventory
	var char_slot_index = interactor.hotbar.selection_index
	if action_index == 0:
		super.interact(character, action_index)
	if action_index == 1:
		var shelf_item_id = get_actual_item(actual_slot_index)
		var shelf_item_definition = interactor.hotbar.database.get_item(shelf_item_id)
		if shelf_item_definition != null:
			inventory.transfer_at(actual_slot_index, char_inventory, char_slot_index)
		return
	if action_index == 2:
		if definition != null:
			char_inventory.transfer_at(char_slot_index, inventory, actual_slot_index)
		return

func interact(character : Node, action_index : int = 0):
	interact_with_slot(character, action_index, slot_index)


func get_actual_item(actual_stack_index):
	if actual_stack_index != -1:
		return inventory.stacks[actual_stack_index].item_id
	return null


func get_interact_actions(interactor : Interactor) -> Array:
	actions_shelf.clear()
	if openable.is_open:
		return actions_shelf
	actions_shelf.append_array(actions)
	var shelf_item_id = get_actual_item(slot_index)
	var shelf_definition = inventory.database.get_item(shelf_item_id)
	if shelf_item_id != null and shelf_definition != null:
		var action = InteractAction.new()
		action.description = "Get " + shelf_definition.name
		action.input = "item_pickup"
		action.code = 1
		actions_shelf.append(action)
	var item_id : String = interactor.hotbar.get_selected_item()
	var definition = inventory.database.get_item(item_id)
	if item_id != "" and definition != null and (shelf_definition == null or shelf_definition == definition):
		var action = InteractAction.new()
		action.description = "Place " + definition.name
		action.input = "item_place"
		action.code = 2
		actions_shelf.append(action)
	return actions_shelf
