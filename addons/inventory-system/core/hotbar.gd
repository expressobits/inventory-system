@tool
@icon("res://addons/inventory-system/icons/hotbar.svg")
extends NodeInventorySystemBase
class_name Hotbar

signal on_change_selection(selection_index)
signal on_update_selection_slot

@export var inventory : Inventory
@export var slots_in_hot_bar := 8

var selection_index := 0

func _ready():
	super._ready()
	if inventory != null:
		inventory.updated_slot.connect(_on_updated_slot.bind())


func change_selection(index : int):
	if index < 0 or index >= slots_in_hot_bar:
		return


func next_item():
	set_selection_index(selection_index - 1)


func previous_item():
	set_selection_index(selection_index + 1)


func set_selection_index(new_index : int):
	if new_index >= slots_in_hot_bar:
		new_index -= slots_in_hot_bar
	if new_index < 0:
		new_index += slots_in_hot_bar
	selection_index = new_index
	on_change_selection.emit(selection_index)


func has_valid_item_id() -> bool:
	if selection_index >= inventory.slots.size():
		return false
	var slot = inventory.slots[selection_index]
	if slot == null:
		return false
	return slot.has_valid()


func has_item_on_selection() -> bool:
	if not has_valid_item_id():
		return false
	return true


func get_selected_item() -> SlotItem:
	if not has_valid_item_id():
		return null
	return inventory.slots[selection_index].item


func _on_updated_slot(slot_index : int):
	if slot_index == selection_index:
		on_update_selection_slot.emit()
